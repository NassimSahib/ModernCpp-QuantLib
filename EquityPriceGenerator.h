#pragma once

#include <vector>

class EquityPriceGenerator
{
public:
    EquityPriceGenerator(double spot, unsigned num_time_steps,
        double time_to_expiration, double volatility, double rf_rate,
        double div_rate);

    // Returns the simulated random path of equity share prices
    std::vector<double> operator()(unsigned seed) const;


private:
    double spot_;
    unsigned num_time_steps_;
    double time_to_expiration_;
    double volatility_;
    double rf_rate_;            // Continuous risk-free rate
    double div_rate_;
    double dt_;

};