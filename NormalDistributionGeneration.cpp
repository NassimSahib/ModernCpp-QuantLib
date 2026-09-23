#include <iostream>
#include <format>

#include <random>

int main()
{
	std::vector<double> nrmls(10);
	// Seed value = 100
	std::mt19937_64 gen{ 100 };

	// Normal Distribution transformation
	std::normal_distribution<double> nrml_distrib{ 0.0, 1.0 };

	for (double& x : nrmls) {
		x = nrml_distrib(gen);
	}

	for (double x : nrmls) {
		std::cout << x << " ";
	}

}