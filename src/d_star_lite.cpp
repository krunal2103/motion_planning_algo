#include "../grid/square_grid_with_weights.h"
#include "../utils.h"

#include "d_star_lite.h"

int main() {

  auto grid = make_diagram4<GridLocation, SquareGridWithWeights>();
  GridLocation start{0, 8};
  GridLocation goal{7, 2};

  D_star_lite<GridLocation, SquareGridWithWeights> d_star_lite(grid, start, goal);

  auto path = d_star_lite();

  draw_grid<GridLocation>(grid, nullptr, nullptr, &path, &start, &goal);
  return 0;
}