#include "../grid/square_grid_with_weights.h"
#include "../utils.h"

#include "lpa_star.h"

int main() {

  auto grid = make_weighted_grid<GridLocation<int>, SquareGridWithWeights>();
  GridLocation<int> start{0, 8};
  GridLocation<int> goal{7, 2};

  draw_grid<GridLocation<int>>(grid, nullptr, nullptr, nullptr, &start, &goal);

  LPA_star<GridLocation<int>, SquareGridWithWeights> lpa_star(grid, start, goal);

  auto path = lpa_star();

  draw_grid<GridLocation<int>>(grid, nullptr, nullptr, &path, &start, &goal);
  return 0;
}