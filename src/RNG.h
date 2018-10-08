#ifndef RNG_H
#define RNG_H

#include <random>

class RNG {
public:
	RNG(int seed = (int) time(0));
	~RNG();
	void setseed(int newSeed);
	void printseed();

	// Uniform Distributions
	int runif(int minNum, int maxNum);
	double runif(double minNum, double maxNum);

	// Normal Distributions
	double rnorm(double mu, double sd);

	// Binomial Distributions (same as Normal for Integers) ?! De Moivre–Laplace theorem
	int rnorm(int mu, int sd);

	// Laplace Distribution
	double rlap(double mu, double sd);

	// Log-normal Distribution
	double rlognorm(double logmean, double logsd);

private:
	int seed;
	std::mt19937 re {};
	using distUnifInt = std::uniform_int_distribution<int>;
	using distUnifDbl = std::uniform_real_distribution<double>;

	distUnifInt dui {};
	distUnifDbl dud {};
	using distNormDbl = std::normal_distribution<double>;
	distNormDbl dnd {};	
};

#endif //RNG_H
