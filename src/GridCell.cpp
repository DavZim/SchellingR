#include "GridCell.h"

/// Creates a grid-cell
/// \param threshold the threshold below which an agent feels unhappy
GridCell::GridCell(double threshold) : threshold(threshold) {
    neighbors = std::vector<std::shared_ptr<GridCell>> (8, nullptr);
}
/// Destructor
GridCell::~GridCell() {}

/// Returns if the occupant of this grid-cell is happy
/// \return
bool GridCell::isHappy() {
    return occupant->group != 0 && happy;
}

/// updates the happiness information of this grid-cell
void GridCell::updateHappiness() {
    nSame = 0;
    nDifferent = 0;
    if (occupant->group == 0) {
        // if cell is empty, its happy and doesnt want to move!
        happy = true;
        return;
    }

    int total = 0;
    for (auto pt : neighbors) {
        if (pt != nullptr && pt->occupant->group != 0) {
            total++;
            if (pt->occupant->group == this->occupant->group) nSame++;
        }
    }
    nDifferent = total - nSame;
    double ratio = (double) nSame / total;
    happy = ratio >= threshold;
}
