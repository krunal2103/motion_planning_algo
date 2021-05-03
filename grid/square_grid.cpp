#include "square_grid.h"

const std::array<GridLocation, 4> SquareGrid::moves_ = {
    /* East, West, North, South */
    GridLocation{1, 0}, GridLocation{-1, 0}, GridLocation{0, -1},
    GridLocation{0, 1}};

bool SquareGrid::in_bounds(GridLocation id) const {
  return 0 <= id.x && id.x < width_ && 0 <= id.y && id.y < height_;
}

bool SquareGrid::passable(GridLocation id) const {
  return walls_.find(id) == walls_.end();
}

std::vector<GridLocation> SquareGrid::neighbors(GridLocation id) const {
  std::vector<GridLocation> results;

  for (GridLocation move : moves_) {
    GridLocation next{id.x + move.x, id.y + move.y};
    if (in_bounds(next) && passable(next)) {
      results.push_back(next);
    }
  }

//    if ((id.x + id.y) % 2 == 0) {
//      // see "Ugly paths" section for an explanation:
//      std::reverse(results.begin(), results.end());
//    }

  return results;
}

bool SquareGrid::is_wall(GridLocation id) {
  return walls_.find(id) != walls_.end();
}