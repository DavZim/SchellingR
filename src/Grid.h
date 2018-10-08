#ifndef GRID_H
#define GRID_H

#include <memory>
#include "Structures.h"
#include "RNG.h"
#include "GridCell.h"


class Grid {
public:
     Grid(double percEmpty = 0.1, double threshold = 0.5, int nRows = 10,
          int nColumns = 10, int nGroups = 2, int seed = 127864);;
    ~Grid();;


    void simulate(int maxRound, RoundResults &roundRes,
                  DetailedResults &detailedRes);
    void update();
    void print();

private:
    std::vector<std::vector<std::shared_ptr<GridCell>>> grid;
    std::vector<std::shared_ptr<GridCell>> emptyPlaces;

    RNG rnd;
    const int nRows, nColumns, nGroups, seed;
    const double threshold, percEmpty;
    int nEmpty = 0, nMoves = 0, round = 0, totalAgents, happyAgents;

    void addDetailedResults(DetailedResults &res);
    void addRoundResults(RoundResults &res);
    void printRoundResults();
    void initiateGrid();
    int countHappy();
    void moveUnhappy();
    void updateAgentInformation();
};


#endif //GRID_H
