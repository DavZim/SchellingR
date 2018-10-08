#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <vector>

/// Structure of the agents
struct Agent {
    Agent() : id(0), group(0) {}
    Agent(int id, int group) : id(id), group(group) {}
    ~Agent() {}

    int id, group;
};

/// Structure of results for one round (aggregated)
struct RoundResults {
  std::vector<int> round;
  std::vector<int> nHappyAgents;
  std::vector<int> nUnhappyAgents;
  std::vector<int> nMoves;
  /// Reserves n elements in each vector
  /// \param n number of elements to reserve
  void reserve(int n) {
    round.reserve(n);
    nHappyAgents.reserve(n);
    nUnhappyAgents.reserve(n);
    nMoves.reserve(n);
  }
};

/// Structure of results for one round per agent
struct DetailedResults {
  std::vector<int> round;
  std::vector<int> x;
  std::vector<int> y;
  std::vector<int> id;
  std::vector<int> group;
  std::vector<int> nSameNeighbors;
  std::vector<int> nDifferentNeighbors;
  std::vector<int> happy;
  /// Reserves n elements in each vector
  /// \param n number of elements to reserve
  void reserve(int n) {
    round.reserve(n);
    x.reserve(n);
    y.reserve(n);
    id.reserve(n);
    group.reserve(n);
    nSameNeighbors.reserve(n);
    nDifferentNeighbors.reserve(n);
    happy.reserve(n);
  }
};


#endif //STRUCTURES_H
