#include "LinearInterpCurve.h"


LinearInterpYieldCurve::LinearInterpYieldCurve(const ChronoDate& settle_date,
    const std::vector<ChronoDate>& maturity_dates,
    const std::vector<double>& unit_prices) :YieldCurve{ settle_date }
{
    using std::size_t;
    if (maturity_dates.size() != unit_prices.size())
        throw std::invalid_argument{
            "Maturity_dates and spot_discount_factors different lengths" };
    if (maturity_dates.front() < this->settle_date())
        throw std::invalid_argument{ "First maturity date before settle date" };
    // Prevent vector memory reallocation (reserve(.)):
    maturities_.reserve(maturity_dates.size());
    yields_.reserve(maturity_dates.size());
    // Assume maturity dates in are in ascending order
    for (size_t i = 0; i < maturity_dates.size(); i++)
    {
        double t = act_365().year_fraction(this->settle_date(), maturity_dates[i]);
        maturities_.push_back(t);
        yields_.push_back(-std::log(unit_prices[i]) / t);
    }
}

double LinearInterpYieldCurve::yield_curve_(const double t) const
{
    using std::size_t;
    // interp_yield called from discount_factor, so maturities.front() <= t
    if (t >= maturities_.back())
    {
        return yields_.back();
    }
    // Now know maturities.front() <= t < maturities_.back()
    size_t idx = 0;
    while (maturities_[idx + 1] < t)
    {
        ++idx;
    }
    return yields_[idx] + (yields_[idx + 1] - yields_[idx])
        / (maturities_[idx + 1] - maturities_[idx]) * (t - maturities_[idx]);
}