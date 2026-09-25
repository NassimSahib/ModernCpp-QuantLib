#include "MCOptionValuation.h"
#include "EquityPriceGenerator.h"

#include <random>
#include <utility>            // std::move
#include <cmath>
#include <vector>
#include <numeric>   // std::accumulate

#include <future>

MCOptionValuation::MCOptionValuation(OptionInfo&& opt, unsigned time_steps,
    double vol, double int_rate, double div_rate, BarrierType barrier_type,
    double barrier_value) :
    opt_{ std::move(opt) }, time_steps_{ time_steps },
    vol_{ vol }, int_rate_{ int_rate }, div_rate_{ div_rate },
    barrier_type_{ barrier_type }, barrier_value_{barrier_value} {}

double MCOptionValuation::calc_price(double spot, unsigned num_scenarios,
    unsigned unif_start_seed)
{
    bool barrier_hit =
        (barrier_type_ == BarrierType::up_and_out && spot >= barrier_value_) ||
        (barrier_type_ == BarrierType::down_and_out && spot <= barrier_value_);
    if (barrier_hit) return 0.0;    // Option is worthless 
    // Case where barrier has not (yet) been crossed
    if (opt_.time_to_expiration() > 0)
    {
        std::mt19937_64 mt_unif{ unif_start_seed };
        std::uniform_int_distribution<unsigned> unif_int_dist{};
        const double disc_factor =
            std::exp(-int_rate_ * opt_.time_to_expiration());
        using std::vector;
        vector<double> discounted_payoffs;
        discounted_payoffs.reserve(num_scenarios);
        // Iteration starts with barrier_hit = false
        for (unsigned i = 0; i < num_scenarios; ++i)
        {
            EquityPriceGenerator epg{ spot, time_steps_,
                opt_.time_to_expiration(), vol_, int_rate_, div_rate_ };
            vector scenario = epg(unif_int_dist(mt_unif));
            switch (barrier_type_)
            {
            case BarrierType::none: break;
            case BarrierType::up_and_out:
            {
                auto barrier_hit_pos = std::find_if(scenario.cbegin(),
                    scenario.cend(),
                    [this](double sim_eq)
                    {return sim_eq >= barrier_value_;});
                if (barrier_hit_pos != scenario.cend())
                {
                    barrier_hit = true;
                }
            }
            break;
            case BarrierType::down_and_out:
            {
                auto barrier_hit_pos = std::ranges::find_if(scenario,
                    [this](double sim_eq)
                    {return sim_eq <= barrier_value_;});
                if (barrier_hit_pos != scenario.cend())
                {
                    barrier_hit = true;
                }
            }
            break;
            }    // end of switch statement
            if (barrier_hit)
            {
                discounted_payoffs.push_back(0.0);
            }
            else
            {
                discounted_payoffs.push_back(disc_factor
                    * opt_.option_payoff(scenario.back()));
            }
            barrier_hit = false;
        }
        // Option value = mean of discounted payoffs
        return (1.0 / num_scenarios)
            * std::accumulate(discounted_payoffs.cbegin(),
                discounted_payoffs.cend(), 0.0);
    }
    else
    {
        // barrier_hit == false, at expiration
        return opt_.option_payoff(spot);
    }
}

double MCOptionValuation::calc_price_par(double spot,
    unsigned num_scenarios, unsigned unif_start_seed)
{
    bool barrier_hit =
        (barrier_type_ == BarrierType::up_and_out && spot >= barrier_value_) ||
        (barrier_type_ == BarrierType::down_and_out && spot <= barrier_value_);
    if (barrier_hit) return 0.0;

    if (opt_.time_to_expiration() > 0)
    {
        using std::vector;
        std::mt19937_64 mt_unif{ unif_start_seed };
        std::uniform_int_distribution<unsigned> unif_int_dist{};
        vector<double> discounted_payoffs;
        discounted_payoffs.reserve(num_scenarios);
        const double disc_factor =
            std::exp(-int_rate_ * opt_.time_to_expiration());
        vector<std::future<vector<double>>> ftrs;
        ftrs.reserve(num_scenarios);
        if (barrier_hit != true)
        {
            for (unsigned i = 0; i < num_scenarios; ++i)
            {
                EquityPriceGenerator epg{ spot, time_steps_,
                    opt_.time_to_expiration(), vol_, int_rate_, div_rate_ };
                // Each scenario is now asynchronous:
                ftrs.push_back(std::async(epg, unif_int_dist(mt_unif)));
            }
            for (auto& ftr : ftrs)
            {
                vector scenario = ftr.get();   // (Also note we can use CTAD) 
                switch (barrier_type_)
                {
                case BarrierType::none: break;
                case BarrierType::up_and_out:
                {
                    auto barrier_hit_pos = std::find_if(scenario.cbegin(),
                        scenario.cend(),
                        [this](double sim_eq)
                        {return sim_eq >= barrier_value_;});
                    if (barrier_hit_pos != scenario.cend())
                    {
                        barrier_hit = true;
                    }
                }
                break;
                case BarrierType::down_and_out:
                {
                    auto barrier_hit_pos = std::ranges::find_if(scenario,
                        [this](double sim_eq)
                        {return sim_eq <= barrier_value_;});
                    if (barrier_hit_pos != scenario.cend())
                    {
                        barrier_hit = true;
                    }
                }
                break;
                }    // end of switch statement
                if (barrier_hit)
                {
                    discounted_payoffs.push_back(0.0);
                }
                else
                {
                    discounted_payoffs.push_back(disc_factor
                        * opt_.option_payoff(scenario.back()));
                }
                barrier_hit = false;
            }
        }
        return (1.0 / num_scenarios)
            * std::accumulate(discounted_payoffs.cbegin(),
                discounted_payoffs.cend(), 0.0);
    }
    else
    {
        return opt_.option_payoff(spot);
    }
}