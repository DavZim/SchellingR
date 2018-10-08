#include <Rcpp.h>
#include "Grid.h"

/// executes the schelling model in rcpp. see Grid class for more details
// [[Rcpp::export]]
Rcpp::List run_schelling_rcpp(int size, double percent_empty, double threshold,
                             int number_of_groups, int max_rounds, int seed) {
  RoundResults roundRes;
  DetailedResults detailedRes;

  Grid grid(percent_empty, threshold, size, size, number_of_groups, seed);
  grid.simulate(max_rounds, roundRes, detailedRes);


  Rcpp::DataFrame detailed = Rcpp::DataFrame::create(
    Rcpp::_["round"] = detailedRes.round,
    Rcpp::_["x"] = detailedRes.x,
    Rcpp::_["y"] = detailedRes.y,
    Rcpp::_["id"] = detailedRes.id,
    Rcpp::_["group"] = detailedRes.group,
    Rcpp::_["n_same"] = detailedRes.nSameNeighbors,
    Rcpp::_["n_different"] = detailedRes.nDifferentNeighbors,
    Rcpp::_["happy"] = detailedRes.happy
  );
  Rcpp::DataFrame round = Rcpp::DataFrame::create(
    Rcpp::_["round"] = roundRes.round,
    Rcpp::_["happy_agents"] = roundRes.nHappyAgents,
    Rcpp::_["unhappy_agents"] = roundRes.nUnhappyAgents,
    Rcpp::_["number_moves"] = roundRes.nMoves
  );

  return Rcpp::List::create(
    Rcpp::_["round"] = round,
    Rcpp::_["detailed"] = detailed
  );
}
