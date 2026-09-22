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

double OptionInfo::time_to_expiration() const {
	return time_to_exp_;
}

OptionInfo::OptionInfo(const OptionInfo& rhs) :
	payoff_{ rhs.payoff_->clone() },
	time_to_exp_{rhs.time_to_expiration()} { }

void OptionInfo::swap(OptionInfo& rhs) noexcept {
	using std::swap;
	swap(payoff_, rhs.payoff_);
	swap(time_to_exp_, rhs.time_to_exp_);
}

OptionInfo& OptionInfo::operator=(const OptionInfo& rhs) {
	OptionInfo{ rhs }.swap(*this);
	return *this;
}