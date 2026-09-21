#include "Payoff.h"

#include <memory>

class PutPayoff : public Payoff
{
public:
	PutPayoff(double strike);
	double payoff(double price) const override;
	std::unique_ptr<Payoff> clone() const override;

private:
	double strike_;
};