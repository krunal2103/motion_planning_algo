#ifndef SQUARE_GRID_WITH_WEIGHTS_H
#define SQUARE_GRID_WITH_WEIGHTS_H

#include "square_grid_interface.h"
#include <utility>

template <typename Location>
class SquareGridWithWeights : public SquareGrid<Location> {
public:
  SquareGridWithWeights(int width, int height)
      : SquareGrid<Location>(width, height) {}
  double cost(Location dest_node) {
    return (std::find(forests_.begin(), forests_.end(), dest_node) !=
            forests_.end())
               ? 5
               : 1;
  }

  void add_forests(std::vector<Location> forests) {
    forests_ = std::move(forests);
  }

  bool is_forest(Location id) {
    return std::find(forests_.begin(), forests_.end(), id) != forests_.end();
  }

private:
  std::vector<Location> forests_;
};

#endif // SQUARE_GRID_WITH_WEIGHTS_H