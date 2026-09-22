#include "CallPayoff.h"
#include "PutPayoff.h"
#include "OptionInfo.h"

#include <memory>

//int main()
//{
//	auto call_payoff = std::make_unique<CallPayoff>(75.0);
//	OptionInfo call_opt{ std::move(call_payoff), 0.5 };
//
//	OptionInfo call_mov{ std::move(call_opt) };
//
//}