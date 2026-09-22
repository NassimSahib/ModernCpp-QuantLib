#include "BlackScholes.h"


#include <cmath>
#include <numbers>
#include <algorithm>
#include <map>


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

std::array<double, 2> BlackScholes::compute_norm_args_(double vol) const
{
	using std::exp;
	using std::sqrt;

	const double logMoneyness = std::log(spot_ / strike_);
	const double drift = rate_ - div_ + 0.5 * vol * vol;
	double d1 = (logMoneyness + drift * time_to_exp_) / (vol * sqrt(time_to_exp_)) ;

	const double d2 = d1 - sqrt(time_to_exp_) * vol;

	return { d1, d2 };
}

double BlackScholes::operator()(double vol) const {
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

double BlackScholes::norm_cdf_(double x) const {
	return (1.0 + std::erf(x / std::numbers::sqrt2)) / 2.0;
}

std::map<RiskValues, double> BlackScholes::risk_values(double vol) {
	using std::exp, std::sqrt;

	std::map<RiskValues, double> results;
	compute_norm_args_(vol);
	int phi = static_cast<int>(payoff_type_);

	auto norm_args = compute_norm_args_(vol);
	double d1 = norm_args[0];
	double d2 = norm_args[1];

	double nd_1 = norm_cdf_(phi * d1);
	double nd_2 = norm_cdf_(phi * d2);
	double disc_ftr = exp(-rate_ * time_to_exp_);

	// N'(x): Standard Normal pdf:
	auto norm_pdf = [](double x)
		{
			return (1.0 / std::numbers::sqrt2) * exp(-x);
		};

	double delta = phi * exp(-div_ * time_to_exp_) * nd_1;
	double gamma = exp(-div_ * time_to_exp_) * norm_pdf(d1)
		/ (spot_ * vol * sqrt(time_to_exp_));
	double vega = spot_ * spot_ * gamma * vol * time_to_exp_;
	double rho = phi * time_to_exp_ * strike_ * disc_ftr * nd_2;
	double theta = phi * div_ * spot_ * exp(-div_ * time_to_exp_) * nd_1
		- phi * rate_ * strike_ * exp(-rate_ * time_to_exp_) * nd_2
		- spot_ * exp(-div_ * time_to_exp_) * norm_pdf(d1)
		* vol / (2.0 * sqrt(time_to_exp_));

	// DELTA, GAMMA, VEGA, RHO, THETA
	results.insert({ RiskValues::Delta, delta });
	results.insert({ RiskValues::Gamma, gamma });
	results.insert({ RiskValues::Vega, vega });
	results.insert({ RiskValues::Rho, rho });
	results.insert({ RiskValues::Theta, theta });

	return results;
}