#include "Bond.h"
#include <iterator>

Bond::Bond(const std::string& bond_id, const ChronoDate& dated_date,
    const ChronoDate& first_coupon_date,
    const ChronoDate& penultimate_coupon_date,
    const ChronoDate& maturity_date, int coupon_frequency, double coupon_rate,
    double face_value) : bond_id_{ bond_id }
{
    //  Number of months in coupon period:
    const int months_in_regular_coupon_period = 12 / coupon_frequency;
    //  Regular coupon payment:
    const double regular_coupon_payment = coupon_rate
        * face_value / coupon_frequency;

    calculate_pmt_schedule_(first_coupon_date, penultimate_coupon_date,
        months_in_regular_coupon_period, regular_coupon_payment);
    amend_initial_irregular_dates_and_pmts_(dated_date, first_coupon_date,
        months_in_regular_coupon_period, regular_coupon_payment);
    amend_final_irregular_dates_and_pmts_(penultimate_coupon_date, maturity_date,
        months_in_regular_coupon_period, regular_coupon_payment, face_value);
    //  Maturity date is a due date which falls on a business day 
    due_dates_.push_back(maturity_date);
    payment_dates_.push_back(maturity_date);
}
void Bond::calculate_pmt_schedule_(const ChronoDate& first_coupon_date,
    const ChronoDate& penultimate_coupon_date,
    int months_in_regular_coupon_period, double regular_coupon_payment)
{
    //  Generate vectors containing due dates, payment dates, 
    // and regular coupon payment amounts:
    for (ChronoDate regular_due_date{ first_coupon_date };
        regular_due_date <= penultimate_coupon_date;
        regular_due_date.add_months(months_in_regular_coupon_period))
    {
        // The due and payment Dates
        due_dates_.push_back(regular_due_date);
        ChronoDate payment_date{ regular_due_date };
        // Roll any due dates falling on a weekend and
        // store as payment dates: 
        payment_dates_.push_back(payment_date.weekend_roll());
        // Assume all coupons are regular;
        // deal with any irregular first or last periods later:
        payment_amounts_.push_back(regular_coupon_payment);
    }
}
void Bond::amend_initial_irregular_dates_and_pmts_(const ChronoDate& dated_date,
    const ChronoDate& first_coupon_date,
    const int months_in_regular_coupon_period,
    const double regular_coupon_payment)
{
    //  If first coupon is irregular, amend the coupon payment 
    ChronoDate first_prior{ first_coupon_date };
    first_prior.add_months(-months_in_regular_coupon_period);
    if (first_prior != dated_date) // if true then irregular coupon
    {
        if (first_prior < dated_date) // if true then short coupon period
        {
            double coupon_fraction =
                static_cast<double>(first_coupon_date - dated_date) /
                static_cast<double>(first_coupon_date - first_prior);
            payment_amounts_[0] *= coupon_fraction;
        }
        else // dated_date < first_prior, so long coupon period
        {
            // long_first_coupon = regular_coupon + extra_interest
            // Calculate the second_prior,
            // the last regular date before the first_prior:
            ChronoDate second_prior{ first_prior };
            second_prior.add_months(-months_in_regular_coupon_period);
            double coupon_fraction =
                static_cast<double>(first_prior - dated_date) /
                static_cast<double>(first_prior - second_prior);
            payment_amounts_[0] += coupon_fraction * regular_coupon_payment;
        }
    }
}
void Bond::amend_final_irregular_dates_and_pmts_(
    const ChronoDate& penultimate_coupon_date,
    const ChronoDate& maturity_date, const int months_in_regular_coupon_period,
    const double regular_coupon_payment, double face_value)
{
    //  If final coupon period is irregular, amend the coupon payment.
    ChronoDate maturity_regular_date{ penultimate_coupon_date };
    //  Calculate maturity_regular_date, the first regular date 
    // after penultimate_coupon_date:
    maturity_regular_date.add_months(months_in_regular_coupon_period);
    double final_coupon{ regular_coupon_payment };
    // If true then irregular coupon period 
    if (maturity_regular_date != maturity_date)
    {
        // If true, adjust for short coupon period:
        if (maturity_date < maturity_regular_date)
        {
            double coupon_fraction =
                static_cast<double>(maturity_date - penultimate_coupon_date)
                / static_cast<double>(
                    maturity_regular_date - penultimate_coupon_date);
            final_coupon *= coupon_fraction;
        }
        // maturity_regular_date < maturity_date,
        // so have long coupon period adjustment:
        else
        {
            // final_coupon = regular_coupon_amount + extra_interest.
            // Calculate the next_regular_date, the first regular date
            // after the maturity_regular_date:
            ChronoDate next_regular_date{ maturity_regular_date };
            next_regular_date.add_months(months_in_regular_coupon_period);
            double extra_coupon_fraction =
                static_cast<double>(maturity_date - maturity_regular_date) /
                static_cast<double>(next_regular_date - maturity_regular_date);
            final_coupon += extra_coupon_fraction * regular_coupon_payment;
        }
    }
    //  Calculate final payment:
    payment_amounts_.push_back(face_value + final_coupon);
}