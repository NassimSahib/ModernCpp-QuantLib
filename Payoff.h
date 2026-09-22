#pragma once
#include <memory>

class Payoff
{
public:
	virtual double payoff(double price) const = 0;
	virtual std::unique_ptr<Payoff> clone() const = 0;
	virtual ~Payoff() = default;
};