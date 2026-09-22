#include <iostream>
#include <format>

#include <vector>

#include <algorithm>
#include <numeric>


int main()
{
	// Volumes and Prices between 10 AM to 04 PM (30 minutes Intervals)
	std::vector v
	{
	376000, 365000, 344000, 346000, 345000, 336000, 335000,
	339000, 340000, 340000, 343000, 367000, 37400
	};
	std::vector p
	{
	208.59, 206.93, 207.75, 209.21, 208.58, 208.63, 207.92,
	208.87, 208.16, 209.49, 208.53, 209.12, 209.05
	};

	double avg_day_volume = std::accumulate(v.begin(), v.end(), 0.0);
	double raw_wgt_price = std::inner_product(v.begin(), v.end(), p.begin(), 0.0);

	double VWAP = raw_wgt_price / avg_day_volume;

	std::cout << std::format("VWAP for the day is {}", VWAP);

}