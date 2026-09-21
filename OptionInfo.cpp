#include "Payoff.h"
#include "OptionInfo.h"

#include <memory>
#include <utility>

OptionInfo::OptionInfo(std::unique_ptr<Payoff> payoff, double time_to_exp):
	payoff_{std::move(payoff)},
	time_to_exp_{time_to_exp} {}


double OptionInfo::option_payoff(double spot) const {
	return payoff_->payoff(spot);
}
