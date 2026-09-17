#include "BlackScholes.h"

#include <cmath>
#include <numbers>
#include <algorithm>


BlackScholes::BlackScholes(double strike,
	                       double spot,
	                       double time_to_exp,
	                       PayoffType payoff_type,
	                       double rate,
	                       double div):
	strike_{ strike },
	spot_{ spot },
	time_to_exp_{ time_to_exp },
	payoff_type_{ payoff_type },
	rate_{ rate },
	div_{ div } { }

std::array<double, 2> BlackScholes::compute_norm_args_(double vol)
{
	using std::exp;
	using std::sqrt;

	const double logMoneyness = std::log(spot_ / strike_);
	const double drift = rate_ - div_ + 0.5 * vol * vol;
	double d1 = (logMoneyness + drift * time_to_exp_) / (vol * sqrt(time_to_exp_)) ;

	const double d2 = d1 - sqrt(time_to_exp_) * vol;

	return { d1, d2 };
}

double BlackScholes::operator()(double vol) {
	using std::exp;
	const int phi = static_cast<int>(payoff_type_);

	if (time_to_exp_ > 0.0)
	{
		auto norm_args = compute_norm_args_(vol);
		double d1 = norm_args[0];
		double d2 = norm_args[1];

		auto norm_cdf = [](double x)
			{
				return (1.0 + std::erf(x / std::numbers::sqrt2)) / 2.0;
			};

		double nd_1 = norm_cdf(phi * d1);
		double nd_2 = norm_cdf(phi * d2);

		double disc_fctr = exp(-rate_ * time_to_exp_);

		return phi * (spot_ * exp(-div_ * time_to_exp_)
			* nd_1 - disc_fctr * strike_ * nd_2);
	}
	else
	{
		return std::max(phi * (spot_ - strike_), 0.0);
	}

}