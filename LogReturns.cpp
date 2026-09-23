#include <iostream>
#include <format>

#include <vector>
#include <deque>
#include <ranges>

#include <algorithm>
#include <numeric>

#include <functional>

//int main()
//{
//	std::vector<double> prices
//	{
//	25.5, 28.0, 30.5, 31.0, 27.5, 31.0, 29.5, 28.5, 37.5,
//	33.5, 25.5, 31.5, 26.5, 29.5, 32.5, 34.5, 28.5, 35.5,
//	28.5, 29.0, 32.0, 23.5, 27.5, 33.5, 28.0, 28.0, 32.5,
//	31.5, 29.0, 33.0, 32.5, 29.5, 34.5
//	};
//	// First / Intuition Method
//	std::adjacent_difference(prices.begin(), prices.end(), prices.begin(), std::divides<double>());
//	std::deque<double> log_rtrns(prices.size());
//
//	std::ranges::transform(prices.begin(), prices.end(), log_rtrns.begin(), [](double x) 
//		{
//		return std::log(x);
//		});
//	log_rtrns.pop_front();
//
//	for (double log_rt : log_rtrns) {
//		std::cout << log_rt << " ";
//	}
//}