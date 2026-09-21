#include "Payoff.h"

#include <memory>

class CallPayoff: public Payoff
{

public:
	CallPayoff(double strike);
	double payoff(double price) const override;
	std::unique_ptr<Payoff> clone() const override;
private:
	double strike_;
 
};