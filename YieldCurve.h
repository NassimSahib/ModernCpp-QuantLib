#include "ChronoDate.h"
#include "DayCounts.h"

class YieldCurve
{
public:
    double discount_factor(const ChronoDate& d1, const ChronoDate& d2) const;
    virtual ~YieldCurve() = default;
    ChronoDate settle_date() const;
protected:
    YieldCurve(ChronoDate settle_date);
    Act365 act_365() const;
private:
    // return settle_;
    // return act_365_;
    // Every derived class is responsible for setting the value of
    // settle_date_ and implementing the function yield_curve_(.).
    virtual double yield_curve_(double t) const = 0;
    ChronoDate settle_;
    inline static Act365 act_365_{};   // The yields are continuously compounded
    // with Actual/365 day count basis
};