#include "ImpliedVolatilitySecant.h"
#include <cmath>

double implied_volatility_with_lambda(const BlackScholes& bsc,
	double opt_mkt_price,
	double x0,
	double x1,
	double tol,
	unsigned max_iter) {

	auto diff = [&bsc, opt_mkt_price](double x)
		{
			return bsc(x) - opt_mkt_price;
		};

	double y0 = diff(x0);
	double y1 = diff(x1);

	double impl_vol = 0.0;
	unsigned count_iter = 0;
	for (count_iter = 0; count_iter <= max_iter; ++count_iter)
	{
		if (std::abs(x1 - x0) > tol)
		{
			impl_vol = x1 - (x1 - x0) * y1 / (y1 - y0);

			x0 = x1;
			x1 = impl_vol;
			y0 = y1;

			y1 = diff(x1);
		}
		else
		{
			return x1;
		}

	}
	return std::nan("");
}