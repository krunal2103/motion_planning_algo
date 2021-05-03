#ifndef SQUARE_GRID_H_
#define SQUARE_GRID_H_

#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <unordered_set>
#include <vector>

struct GridLocation {
  int x, y;

  bool operator==(GridLocation b) const {
    return this->x == b.x && this->y == b.y;
  }

  bool operator!=(GridLocation b) const { return !(*this == b); }

  bool operator<(GridLocation b) {
    return std::tie(this->x, this->y) < std::tie(b.x, b.y);
  }

  std::basic_iostream<char>::basic_ostream &
  operator<<(std::basic_iostream<char>::basic_ostream &out) const {
    out << '(' << this->x << ',' << this->y << ')';
    return out;
  }
};

namespace std {

/* implement hash function so we can put GridLocation into an unordered_set */
template <> struct hash<GridLocation> {
  std::size_t operator()(const GridLocation &id) const noexcept {
    return std::hash<int>()(id.x ^ (id.y << 4));
  }
};

} // namespace std

class SquareGrid {
public:
  SquareGrid(int width, int height) : width_(width), height_(height) {}

  bool in_bounds(GridLocation id) const;

  bool passable(GridLocation id) const;

  std::vector<GridLocation> neighbors(GridLocation id) const;

  void add_wall(GridLocation loc) { walls_.insert(loc); }

  int width() { return width_; }
  int height() { return height_; }

  bool is_wall(GridLocation id);

private:
  static const std::array<GridLocation, 4> moves_;
  int width_, height_;
  std::unordered_set<GridLocation> walls_;
};

#endif // SQUARE_GRID_H_