#include "ChronoDate.h"

#include <stdexcept>        // std::invalid_argument


ChronoDate::ChronoDate(int year, unsigned month, unsigned day)
    : date_
    {
        std::chrono::year{year} /
        std::chrono::month{month} /
        std::chrono::day{day}
    }
{
    validate_();            // Checks whether the date is valid
}