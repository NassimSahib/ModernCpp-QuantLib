#include <iostream>
#include "ChronoDate.h" 

// DÉCLARATIONS DES CLASSES DE DAY COUNT
class DayCount {
public:
    virtual double year_fraction(const ChronoDate& date_01, const ChronoDate& date_02) const = 0;
    virtual ~DayCount() = default;
};

class Act365 : public DayCount {
public:
    double year_fraction(const ChronoDate& date_01, const ChronoDate& date_02) const override;
};

class Act360 : public DayCount {
public:
    double year_fraction(const ChronoDate& date_01, const ChronoDate& date_02) const override;
};

class Thirty360 : public DayCount {
public:
    double year_fraction(const ChronoDate& date_01, const ChronoDate& date_02) const override;
private:
    int date_diff_(const ChronoDate& date_01, const ChronoDate& date_02) const;
};

// === 2. DÉFINITIONS DES MÉTHODES ===
double Act365::year_fraction(const ChronoDate& date_01, const ChronoDate& date_02) const {
    return (date_02 - date_01) / 365.0;
}

double Act360::year_fraction(const ChronoDate& date_01, const ChronoDate& date_02) const {
    return (date_02 - date_01) / 360.0;
}

double Thirty360::year_fraction(const ChronoDate& date_01, const ChronoDate& date_02) const {
    return date_diff_(date_01, date_02) / 360.0;
}

int Thirty360::date_diff_(const ChronoDate& date_01, const ChronoDate& date_02) const {
    int d1 = date_01.day();
    int d2 = date_02.day();

    if (d1 == 31) d1 = 30;
    if ((d2 == 31) && (d1 == 30)) d2 = 30;

    return 360 * (date_02.year() - date_01.year()) + 30 * (date_02.month() - date_01.month()) + d2 - d1;
}

int main() {
    Act365 act_365{};
    Act360 act_360{};
    Thirty360 thirty_360{};

    ChronoDate sd_01{ 2021, 4, 26 };
    ChronoDate ed_01{ 2023, 10, 26 };
    ChronoDate sd_02{ 2022, 10, 10 };
    ChronoDate ed_02{ 2023, 4, 10 };

    double yf_act_365_01 = act_365.year_fraction(sd_01, ed_01);
    double yf_act_365_02 = act_365.year_fraction(sd_02, ed_02);
    double yf_act_360_01 = act_360.year_fraction(sd_01, ed_01);
    double yf_act_360_02 = act_360.year_fraction(sd_02, ed_02);
    double yf_thirty_01 = thirty_360.year_fraction(sd_01, ed_01);
    double yf_thirty_02 = thirty_360.year_fraction(sd_02, ed_02);


    std::cout << "Act/365 (01): " << yf_act_365_01 << " (Attendu: 2.50137)\n";
    std::cout << "Act/365 (02): " << yf_act_365_02 << " (Attendu: 0.49863)\n\n";

    std::cout << "Act/360 (01): " << yf_act_360_01 << " (Attendu: 2.53611)\n";
    std::cout << "Act/360 (02): " << yf_act_360_02 << " (Attendu: 0.505556)\n\n";

    std::cout << "30/360  (01): " << yf_thirty_01 << " (Attendu: 2.5)\n";
    std::cout << "30/360  (02): " << yf_thirty_02 << " (Attendu: 0.5)\n";

    return 0;
}
