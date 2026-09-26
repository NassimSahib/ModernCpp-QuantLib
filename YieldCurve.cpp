#include "YieldCurve.h"
#include "DayCounts.h"
#include <stdexcept>
#include <cmath>

double YieldCurve::discount_factor(const ChronoDate& d1,
    const ChronoDate& d2) const
{
    using std::exp;
    if (d2 < d1)
    {
        throw std::invalid_argument{
            "discount_factor(.): invalid inequality: d2 < d1" };
    }
    if (d1 < settle_date() || d2 < settle_date())
    {
        throw std::invalid_argument{
            "discount_factor(.): dates must fall on or after settle date" };
    }
    if (d1 == d2)
    {
        return 1.0;        // exp(0.0)
    }
    // P(t1, t2) = exp( -(t2-t1) * f(t1, t2) )
    double t2 = act_365().year_fraction(settle_date(), d2);
    double y2 = yield_curve_(t2);
    // if d1 == settle_ then P(t1,t2) = P(0,t2) = exp(-t2 * y2 )
    if (d1 == settle_date())
    {
        return exp(-t2 * y2);
    }
    double t1 = act_365().year_fraction(settle_date(), d1);
    double y1 = yield_curve_(t1);
    // (t2-t1) f(t1,t2) = t2 * y2 - t1 * y1
    return exp(t1 * y1 - t2 * y2);
}

YieldCurve::YieldCurve(ChronoDate settle_date) :
    settle_{ std::move(settle_date) } {
}

ChronoDate YieldCurve::settle_date() const
{
    return settle_;
}

Act365 YieldCurve::act_365() const
{
    return act_365_;
}