#include "ChronoDate.h"
#include "YieldCurve.h"
#include <cmath>
#include <vector>
#include <string>

class Bond
{
public:
    Bond(const std::string& bond_id, const ChronoDate& dated_date,
        const ChronoDate& first_coupon_date,
        const ChronoDate& penultimate_coupon_date,
        const ChronoDate& maturity_date,
        int coupon_frequency, double coupon_rate, double face_value);
    double discounted_value(const ChronoDate& bond_settle_date,
        const YieldCurve& yield_curve);
    std::string bond_id() const;
private:
    std::string bond_id_;
    // Dates on which payments are due, whether business days or not:
    std::vector<ChronoDate> due_dates_;
    // Business dates on which payments are made:
    std::vector<ChronoDate> payment_dates_;
    // Coupon and redemption payments,
    // assume redemption_payment = face_value:
    std::vector<double> payment_amounts_;
    void calculate_pmt_schedule_(const ChronoDate& first_coupon_date,
        const ChronoDate& penultimate_coupon_date,
        int months_in_regular_coupon_period, double regular_coupon_payment);
    void amend_initial_irregular_dates_and_pmts_(const ChronoDate& dated_date,
        const ChronoDate& first_coupon_date,
        const int months_in_regular_coupon_period,
        const double regular_coupon_payment);
    void amend_final_irregular_dates_and_pmts_(
        const ChronoDate& penultimate_coupon_date,
        const ChronoDate& maturity_date,
        const int months_in_regular_coupon_period,
        const double regular_coupon_payment, double face_value);
};