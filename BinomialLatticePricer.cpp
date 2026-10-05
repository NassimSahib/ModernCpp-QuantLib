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