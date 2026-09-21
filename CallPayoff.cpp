#include "CallPayoff.h"

#include <algorithm>
#include <memory>

CallPayoff::CallPayoff(double strike) :
	strike_{ strike } { }


double CallPayoff::payoff(double price) const
{
	return std::max(price - strike_, 0.0);
}

std::unique_ptr<Payoff> CallPayoff::clone() const {
	return std::make_unique<CallPayoff>(*this);
}