#include "../grid/square_grid_with_weights.h"
#include "../utils.h"

#include "lpa_star.h"

int main() {

  auto grid = make_diagram4<GridLocation, SquareGridWithWeights>();
  GridLocation start{1, 4};
  GridLocation goal{8, 1};

  draw_grid<GridLocation>(grid, nullptr, nullptr, nullptr, &start, &goal);

  LPA_star<GridLocation, SquareGridWithWeights> lpa_star(grid, start, goal);

  lpa_star();
  auto path = lpa_star.generate_path();

  draw_grid<GridLocation>(grid, nullptr, nullptr, &path, &start, &goal);
  return 0;
}