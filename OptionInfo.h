#include <memory>

#include "Payoff.h"

class OptionInfo
{
public:
	OptionInfo(std::unique_ptr<Payoff> payoff, double time_to_exp);
	double option_payoff(double spot) const;
	double time_to_expiration() const;

private:
	std::unique_ptr<Payoff> payoff_;
	double time_to_exp_;
};