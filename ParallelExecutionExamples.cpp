#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <execution>

//int main()
//{
//	// Generate large number of normal variates:
//	std::mt19937_64 mt(25);
//	std::normal_distribution<> nd;
//	auto next_norm = [&mt, &nd]()
//		{
//			return nd(mt);
//		};
//	const unsigned n = 500;
//	std::vector<double> norms(n);
//	std::generate(norms.begin(), norms.end(), next_norm);
//
//
//	auto max_norm = std::max_element(std::execution::par,
//		norms.begin(), norms.end());
//
//	auto min_norm = std::min_element(std::execution::par,
//		norms.begin(), norms.end());
//
//
//	double mean_parallel = 1.0 / norms.size()
//		* std::reduce(std::execution::par, norms.begin(), norms.end(), 0.0);
//
//	std::vector<int> v{ 4, 5, 6 };
//	std::vector<int> w{ 1, 2, 3 };
//
//	double dot_prod = std::transform_reduce(std::execution::par,
//		v.begin(), v.end(), w.begin(), 0);
//
//	int sum_diff = std::transform_reduce(v.begin(), v.end(), w.begin(), 0,
//		std::plus<int>{}, std::minus<int>{});
//
//	int num_elements = 10'000;
//	std::mt19937_64 mtre{ 100 };
//	std::normal_distribution<long double> nd;
//	std::vector<long double> v(num_elements);
//
//	auto next_norm = [&mtre, &nd](double x)
//		{
//			return nd(mtre);
//		};
//
//	std::transform(v.begin(), v.end(), v.begin(), next_norm);
//	auto u = v;
//
//	auto exp_series = [n](double x) {
//		double num = x;
//		// A standard normal variate
//        double den = 1.0;
//		double res = 1.0 + x;
//		for (unsigned k = 2; k < n; ++k)
//		{
//			num *= x;
//			den *= static_cast<double>(k);
//			res += num / den;
//		}
//		return res;
//		};
//
//	std::transform(u.begin(), u.end(), u.begin(), exp_series);
//	std::transform(std::execution::par, v.begin(), v.end(), v.begin(), exp_series);
//}
//
