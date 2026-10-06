#include "BinomialLatticePricer.h"

#include <algorithm>
#include <limits>
#include <utility>        // std::move

BinomialLatticePricer::BinomialLatticePricer(OptionInfo opt,
    double vol, double int_rate, int time_steps, double div_rate) :
    opt_{ std::move(opt) }, time_points_{ time_steps + 1 }, div_rate_{ div_rate }
{
    double dt{ opt_.time_to_expiration() / time_steps };
    u_ = std::exp(vol * std::sqrt(dt));
    d_ = 1.0 / u_;
    p_ = 0.5 * (1.0 + (int_rate - div_rate - 0.5 * vol * vol)
        * std::sqrt(dt) / vol);
    disc_fctr_ = std::exp(-int_rate * dt);
    grid_.resize(boost::extents[time_points_][time_points_]);
}

double BinomialLatticePricer::calc_price(double spot, OptType opt_type)
{
    project_underlying_prices_(spot);
    return calculate_node_payoffs_(opt_type);
}

void BinomialLatticePricer::project_underlying_prices_(double spot)
{
    grid_[0][0].underlying = spot;        // Terminal node
    // j: columns, i: rows.
    // Traverse by columns, then set node in each row.
    for (int j = 1; j < time_points_; ++j)
    {
        for (int i = 0; i <= j; ++i)
        {
            if (i < j)
            {
                grid_[i][j].underlying = u_ * grid_[i][j - 1].underlying;
            }
            else    // (i == j)
            {
                grid_[i][j].underlying = d_ * grid_[i - 1][j - 1].underlying;
            }
        }
    }
}

void BinomialLatticePricer::american_payoffs_()
{
    // Start from penultimate column prior to expiration: j = time_points_ - 2
    for (int j = time_points_ - 2; j >= 0; --j)
    {
        for (int i = 0; i <= j; ++i)
        {
            grid_[i][j].payoff = std::max(disc_expected_val_(i, j),
                opt_.option_payoff(grid_[i][j].underlying));
        }
    }
}
// Step (3) -- European
void BinomialLatticePricer::european_payoffs_()
{
    // Start from penultimate column prior to expiration: j = time_points_ - 2
    for (int j = time_points_ - 2; j >= 0; --j)
    {
        for (int i = 0; i <= j; ++i)
        {
            grid_[i][j].payoff = disc_expected_val_(i, j);
        }
    }
}

double BinomialLatticePricer::calculate_node_payoffs_(OptType opt_type)
{
    // Set the terminal nodes with payoffs at expiration: j = time_points_ - 1
    for (int i = 0; i <= time_points_ - 1; ++i)
    {
        grid_[i][time_points_ - 1].payoff =
            opt_.option_payoff(grid_[i][time_points_ - 1].underlying);
    }
    if (opt_type == OptType::American)
        american_payoffs_();
    else
        european_payoffs_();      // OptType::Euro
    return grid_[0][0].payoff;    // Option value
}