#include "Grid.h"

/// Initialises a grid
/// \param percEmpty what percentage of the grid should be empty?
/// \param threshold below what similarity should an agent feel uncomfortable and move
/// \param nRows number of rows in the grid
/// \param nColumns number of columns in the grid
/// \param nGroups number of groups of agents
/// \param seed seed for the pseudo-random-number generator
Grid::Grid(double percEmpty, double threshold, int nRows, int nColumns, int nGroups, int seed) :
        percEmpty(percEmpty), threshold(threshold), nRows(nRows), nColumns(nColumns), nGroups(nGroups), seed(seed) {
    rnd.setseed(seed);
    initiateGrid();
}

/// Destructor
Grid::~Grid() {}

/// Simulates the Grid until maxRound is reached or all agents are happy
/// \param maxRound the maximum number of rounds to simulate
/// \param roundRes round results reference
/// \param detailedRes detailed results reference
void Grid::simulate(int maxRound, RoundResults& roundRes, DetailedResults& detailedRes) {
  roundRes.reserve(maxRound);
  detailedRes.reserve(maxRound * totalAgents);

  addRoundResults(roundRes);
  addDetailedResults(detailedRes);

  while (round < maxRound && totalAgents != happyAgents) {
    update();
    addRoundResults(roundRes);
    addDetailedResults(detailedRes);
  }
}

/// Advances the grid by one step
void Grid::update() {
    round++;
    moveUnhappy();
    // printRoundResults();
}

/// Prints the structure of the grid
void Grid::print() {
    for (auto r : grid) {
        for (auto gc : r) {
            if (gc->occupant->group == 0) {
                printf("%5s | ", "");
            } else {
                printf("%5i | ", gc->occupant->group);
            }
        }
        printf("\n");
    }
}

/// Adds agent-level detailed summary statistics for the current round to an element
/// \param res the element the results are added to
void Grid::addDetailedResults(DetailedResults& res) {
  for (int r = 0; r < nRows; r++) {
    for (int c = 0; c < nColumns; c++) {
      res.round.push_back(round);
      // to have (1, 1) at the lower left corner and indexed at 1
      res.x.push_back(c + 1);
      res.y.push_back(nRows - r);
      res.id.push_back(grid[r][c]->occupant->id);
      res.group.push_back(grid[r][c]->occupant->group);
      res.nSameNeighbors.push_back(grid[r][c]->nSame);
      res.nDifferentNeighbors.push_back(grid[r][c]->nDifferent);
      res.happy.push_back(grid[r][c]->isHappy());
    }
  }
}

/// Adds aggregated summary statistics for the current round to an element
/// \param res the element the results are added to
void Grid::addRoundResults(RoundResults &res) {
  res.round.push_back(round);
  res.nHappyAgents.push_back(happyAgents);
  res.nUnhappyAgents.push_back(totalAgents - happyAgents);
  res.nMoves.push_back(nMoves);
}


/// Prints statistics of the current round to the console
void Grid::printRoundResults() {
    printf("Round: %5i, nHappy: %5i, nUnhappy %5i, percHappy %.2f%%\n",
           round, happyAgents, totalAgents - happyAgents, ((double) happyAgents / totalAgents) * 100);
}

/// Initiates the grid
void Grid::initiateGrid() {
  //printf("Initialising a grid of sice %ix%i, with %i groups and a goal of %.2f%% empty places\n",
  //       nRows, nColumns, nGroups, percEmpty * 100);

  grid.resize(nRows);
  int id = 0;
  std::shared_ptr<Agent> emptyAgent(new Agent());

  // sample the groups
  std::vector<int> groups (nRows * nColumns);
  for (int& group : groups) {
    group = rnd.runif(1, nGroups);
  }
  std::vector<int> emptyCells = rnd.sample(nRows * nColumns, (int) nRows * nColumns * percEmpty, false);
  for (int i : emptyCells) {
    groups[i] = 0;
  }

  int counter = 0;
  for (int r = 0; r < nRows; r++) {
    grid[r].resize(nColumns);
    for (int c = 0; c < nColumns; c++) {
      std::shared_ptr<GridCell> gc(new GridCell(threshold));
      if (groups[counter++] == 0) {
        gc->occupant = emptyAgent;
        emptyPlaces.push_back(gc);
        nEmpty++;
      } else {
        std::shared_ptr<Agent> ag(new Agent(id++, groups[counter - 1]));
        gc->occupant = ag;
      }

      grid[r][c] = gc;

      // connect the agents
      // North East
      if (r > 0 && c > 0) {
        grid[r][c]->neighbors[0] = grid[r - 1][c - 1];
        // connect NE to current (south west)
        grid[r - 1][c - 1]->neighbors[6] = grid[r][c];
      }
      // North
      if (r > 0) {
        grid[r][c]->neighbors[1] = grid[r - 1][c];
        // connect N to current (south)
        grid[r - 1][c]->neighbors[5] = grid[r][c];;
      }
      // North West
      if (r > 0 && c < nColumns - 1) {
        grid[r][c]->neighbors[2] = grid[r - 1][c + 1];
        // connect NW to current (south east)
        grid[r - 1][c + 1]->neighbors[4] = grid[r][c];;
      }
      // East
      if (c > 0) {
        grid[r][c]->neighbors[7] = grid[r][c - 1];
        // connect E to current (W)
        grid[r][c - 1]->neighbors[3] = grid[r][c];;
      }
    }
  }

  totalAgents = id;
  updateAgentInformation();
  //printf("Number of total places: %i, number of occupants %i, actual perc empty %.2f%%, perc happy: %.2f%%\n",
  //       nRows * nColumns, totalAgents, (1. - (double) (totalAgents) / (nRows * nColumns)) * 100,
  //       (double) happyAgents / totalAgents * 100);
}

/// Counts the number of happy agents
/// \return the number of happy agents
int Grid::countHappy() {
    int res = 0;
    for (auto r : grid) {
        for (auto gc : r) {
            res += gc->isHappy() ? 1 : 0;
        }
    }
    return res;
}

/// Moves unhappy agents to a different (random) location
void Grid::moveUnhappy() {
    nMoves = 0;
    if (nEmpty == 0) {
        // printf("Unable to move unhappy agents, as no free place exists\n");
        return;
    }
    for (auto r : grid) {
        for (std::shared_ptr<GridCell> gc : r) {
            if (gc->occupant->group != 0 && !gc->isHappy()) {
                int moveTo = rnd.runif(0, nEmpty - 1);
                std::shared_ptr<Agent> emptyAgent = emptyPlaces[moveTo]->occupant;
                // move occupant to new place
                emptyPlaces[moveTo]->occupant = gc->occupant;
                // free the current location
                gc->occupant = emptyAgent;
                // register the current location (gc) as empty
                emptyPlaces[moveTo] = gc;
                nMoves++;
            }
        }
    }
    updateAgentInformation();
}

/// updates the information for each agent
void Grid::updateAgentInformation() {
    for (auto r : grid) {
        for (std::shared_ptr<GridCell> gc : r) {
            gc->updateHappiness();
        }
    }
    happyAgents = countHappy();
}
