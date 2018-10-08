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
