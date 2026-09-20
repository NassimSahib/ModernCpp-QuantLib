#include <cmath>
#include "BlackScholes.h"

double implied_volatility_with_lambda(const BlackScholes& bsc,
	                                  double opt_mkt_price,
	                                  double x0,
	                                  double x1,
	                                  double tol,
	                                  unsigned max_iter);