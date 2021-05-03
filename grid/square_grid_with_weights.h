#ifndef SQUARE_GRID_WITH_WEIGHTS_H
#define SQUARE_GRID_WITH_WEIGHTS_H

#include <utility>
#include "square_grid_interface.h"

template<typename Location>
class SquareGridWithWeights : public SquareGrid<Location> {
public:
  SquareGridWithWeights(int width, int height) : SquareGrid<Location>(width, height) {}
  double cost(Location dest_node) {
    return (forests_.find(dest_node) != forests_.end()) ? 5 : 1;
  }

  void add_forests(std::unordered_set<Location> forests) {
    forests_ = std::move(forests);
  }

private:
  std::unordered_set<Location> forests_;
};

#endif // SQUARE_GRID_WITH_WEIGHTS_H