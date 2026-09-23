#include <iostream>

#include <random>

#include <random>


int main()
{
	std::vector<double> tdistrib(10);
	std::vector<double> poissons(10);

	std::mt19937_64 gen(100);

	std::student_t_distribution<double> student_distrib{ 3 };
	std::poisson_distribution<int> poisson_distrib{ 7.5 };

	for (double& x : tdistrib) {
		x = student_distrib(gen);
	}

	for (double& x : poissons) {
		x = poisson_distrib(gen);
	}

	std::cout << "T-Student: " << "\n";

	for (double& x : tdistrib) {
		std::cout << x << " ";
	}

	std::cout << "Poisson: " << "\n";

	for (double& x : poissons) {
		std::cout << x << " ";
	}
}