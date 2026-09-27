#include "YieldCurve.h"

class LinearInterpYieldCurve final : public YieldCurve
{
public:
    LinearInterpYieldCurve(
        const ChronoDate& settle_date,
        const std::vector<ChronoDate>& maturity_dates,
        const std::vector<double>& unit_prices);
private:
    double yield_curve_(const double t) const override;
    std::vector<double> maturities_; // maturities in years
    std::vector<double> yields_;
};