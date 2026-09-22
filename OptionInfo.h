#include <memory>

#include "Payoff.h"

class OptionInfo
{
public:
	OptionInfo(std::unique_ptr<Payoff> payoff, double time_to_exp);
	double option_payoff(double spot) const;
	double time_to_expiration() const;
	void swap(OptionInfo& rhs) noexcept;

	OptionInfo(const OptionInfo& rhs);
	OptionInfo& operator=(const OptionInfo& rhs);

	OptionInfo(OptionInfo&& rhs) = default;
	OptionInfo& operator=(OptionInfo&& rhs) = default;

	~OptionInfo() = default;

private:
	std::unique_ptr<Payoff> payoff_;
	double time_to_exp_;


};