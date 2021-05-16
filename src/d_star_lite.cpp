#include "../grid/square_grid_with_weights.h"
#include "../utils/utils.h"

#include "d_star_lite.h"

int main() {

  auto grid = make_weighted_grid<GridLocation<int>, SquareGridWithWeights>();
  GridLocation<int> start{0, 8};
  GridLocation<int> goal{7, 2};

  D_star_lite<GridLocation<int>, SquareGridWithWeights> d_star_lite(grid, start, goal);

  auto path = d_star_lite();

  draw_grid<GridLocation<int>>(grid, nullptr, nullptr, &path, &start, &goal);
  return 0;
}