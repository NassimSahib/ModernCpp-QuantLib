#include "PutPayoff.h"

#include <algorithm>
#include <memory>


PutPayoff::PutPayoff(double strike): strike_{strike} {}

double PutPayoff::payoff(double price) const {
	return std::max(strike_ - price, 0.0);
}

std::unique_ptr<Payoff> PutPayoff::clone() const {
	return std::make_unique<PutPayoff>(*this);
}