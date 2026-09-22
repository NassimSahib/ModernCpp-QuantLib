#pragma once
#include <array>
#include <map>

enum class PayoffType
{
	Call = 1,
	Put = -1
};

// Ch 4
enum class RiskValues
{
	Delta,
	Gamma,
	Vega,
	Rho,
	Theta
};

class BlackScholes
{
public:
	// default dividend = 0
	BlackScholes(double strike,
		double spot,
		double time_to_exp,
		PayoffType payoff_type,
		double rate,
		double div = 0.0);
	
	double operator()(double vol) const ;
	 // Ch 4
	std::map<RiskValues, double> risk_values(double vol); 

private:
	std::array<double, 2> compute_norm_args_(double vol) const;
	double norm_cdf(double x) const; // Ch 4

	double strike_, spot_, time_to_exp_;
	PayoffType payoff_type_;
	double rate_, div_;
};