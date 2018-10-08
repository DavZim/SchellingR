#ifndef GRIDCELL_H
#define GRIDCELL_H

#include <memory>
#include "Structures.h"

class GridCell{
public:
    GridCell(double threshold = 0.5);
    ~GridCell();
    bool isHappy();

    std::shared_ptr<Agent> occupant;
    // nw, n, ne, e, se, s, sw, w;
    std::vector<std::shared_ptr<GridCell>> neighbors;

    void updateHappiness();
    int nSame, nDifferent;
private:
    const double threshold;
    bool happy = false;

};


#endif //GRIDCELL_H
