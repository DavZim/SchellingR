# include "RNG.h"

/// Initiator
RNG::RNG(int seed) {
	setseed(seed);
}

/// Sets a new seed of the RNG
/// \param newSeed a new seed
void RNG::setseed(int newSeed){
	seed = newSeed;
	re.seed(seed);
	dnd.reset();
	dud.reset();
	dui.reset();
}

/// Prints the current seed of the RNG
void RNG::printseed() {
	printf("Current seed is: %i\n", seed);
}

/// Destructor
RNG::~RNG() {
}
/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
UNIFROM DISTRIBUTION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

/// Creates a single random integer from a Uniform distribution
/// \param minNum minimum number
/// \param maxNum maximum number
/// \return an integer
int RNG::runif(int minNum, int maxNum) {
	return dui(re, distUnifInt::param_type{minNum, maxNum});
}

/// Creates a single random double from a Uniform distribution
/// \param minNum minimum number
/// \param maxNum maximum number
/// \return a double
double RNG::runif(double minNum, double maxNum) {
	return dud(re, distUnifDbl::param_type{minNum, maxNum});
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
NORMAL DISTRIBUTION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

/// Creates a single random integer from a Normal distribution
/// \param minNum minimum number
/// \param maxNum maximum number
/// \return an integer
int RNG::rnorm(int mu, int sd) {
	return (int) dnd(re, distNormDbl::param_type{(double) mu, (double) sd});
}

/// Creates a single random double from a Normal distribution
/// \param minNum minimum number
/// \param maxNum maximum number
/// \return a double
double RNG::rnorm(double mu, double sd) {
	return dnd(re, distNormDbl::param_type{mu, sd});
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
LAPLACE DISTRIBUTION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

/// Creates a single random double from a Laplacian distribution
/// \param minNum minimum number
/// \param maxNum maximum number
/// \return a double
double RNG::rlap(double mu, double sd){
	const double u = runif(-0.5, 0.5);
	const double sgnU = (0.0 < u) - (0.0 > u);
	const double absU = u > 0 ? u : -u;
	return mu - sd * sgnU * log(1-2*absU);
}

/// Generates a random draw from a power-law distribution
/// \param powExp The exponent of the power-law
/// \param minNum the minimum number
/// \param maxNum the maximum number
/// \return
double RNG::rlognorm(double logmean, double logsd) {
    return exp(rnorm(logmean, logsd));
}

/// Samples n values from x with or without replacement
/// \param x a vector of integers
/// \param n number of draws from x
/// \param replace if the values in x should be replaced
/// \return a vector of random elements of x
std::vector<int> RNG::sample(std::vector<int> x, const int n, const bool replace = false) {
  std::vector<int> res;
  res.reserve(n);
  if (replace) {
    for (int i = 0; i < n; ++i) {
      res.push_back(x[runif(0, x.size() - 1)]);
    }
  } else {
    if (n > x.size()) {
      Rcpp::stop("x must be larger than n in case of replace = true");
    }
    // draw n values
    for (int i = 0; i < n; i++) {
      const int idx = runif(0, (int) x.size() - 1);
      res.push_back(x[idx]);
      x.erase(x.begin() + idx);
    }
  }
  return res;
}

/// Samples n elements in the range 0 <= x < k
/// \param k range limiter
/// \param n number of draws below k
/// \param replace if the values should be replaced
/// \return a vector of random draws below k
std::vector<int> RNG::sample(const int k, const int n, const bool replace) {
  std::vector<int> res;
  res.reserve(n);
  for (int i = 0; i < k; ++i) {
    res.push_back(i);
  }
  res = sample(res, n, replace);
  return res;
}
