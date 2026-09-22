#include "BlackScholes.h"
#include "ImpliedVolatilitySecant.h"

#include <iostream>
#include <format>

//int main()
//{
//	double strike = 95.0;
//	double mkt_opt_price = 6.2;
//	auto payoff_type = PayoffType::Call;
//	double spot = 100.0;
//	double rate = 0.05;
//	double dividend = 0.07;
//	double time_to_exp = 0.25;
//	
//	BlackScholes bsc_impl_vol{ strike, spot, time_to_exp, payoff_type, rate };
//
//	double init_vol_guess_1 = 0.1;
//	double init_vol_guess_2 = 0.15;
//
//	double tol = 1e-6;
//	unsigned max_iter = 1000;
//
//	double impl_vol = implied_volatility_with_lambda(bsc_impl_vol, mkt_opt_price, init_vol_guess_1, init_vol_guess_2,
//		                                             tol, max_iter);
//
//	if (!std::isnan(impl_vol)) {
//		std::cout << std::format("Call ITM, time to expiration = {}, ", time_to_exp);
//		std::cout << std::format("Implied vol = {}\n", impl_vol);
//		double opt_val = bsc_impl_vol(impl_vol);
//		std::cout << std::format("Value of option at implied vol = {}, ", opt_val);
//		std::cout << std::format("Market option price = {}\n", mkt_opt_price);
//
//	}
//	else {
//		std::cout << "No convergence to implied volatility\n";
//	}
//}