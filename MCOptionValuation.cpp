#include "MCOptionValuation.h"
#include "EquityPriceGenerator.h"

#include <random>
#include <utility>            // std::move
#include <cmath>
#include <vector>
#include <numeric>   // std::accumulate

MCOptionValuation::MCOptionValuation(OptionInfo&& opt, int time_steps,
    double vol, double int_rate, double div_rate) :
    opt_{ std::move(opt) }, time_steps_{ time_steps },
    vol_{ vol }, int_rate_{ int_rate }, div_rate_{ div_rate } {
}
double MCOptionValuation::calc_price(double spot, int num_scenarios,
    unsigned unif_start_seed)
{
    if (opt_.time_to_expiration() > 0.0)
    {
        using std::vector;
        std::mt19937_64 mt_unif{ unif_start_seed };
        std::uniform_int_distribution<unsigned> unif_int_dist{};
        vector<double> discounted_payoffs;
        discounted_payoffs.reserve(num_scenarios);
        const double disc_factor =
            std::exp(-int_rate_ * opt_.time_to_expiration());
        for (int i = 0; i < num_scenarios; ++i)
        {
            EquityPriceGenerator epg{ spot, time_steps_,
                opt_.time_to_expiration(), vol_, int_rate_, div_rate_ };
            // (unif_int_dist(mt_unif) provides the next seed):
            vector scenario = epg(unif_int_dist(mt_unif));
            discounted_payoffs.push_back(disc_factor
                * opt_.option_payoff(scenario.back()));
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