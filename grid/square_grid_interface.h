#ifndef SQUARE_GRID_H_
#define SQUARE_GRID_H_

#pragma once

#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <unordered_set>

#include <vector>

template <typename Location> class SquareGrid {
public:
  SquareGrid(int width, int height) : width_(width), height_(height) {}

  bool in_bounds(Location id) const {
    return 0 <= id.x && id.x < width_ && 0 <= id.y && id.y < height_;
  }

  bool passable(Location id) const { return walls_.find(id) == walls_.end(); }

  std::vector<Location> neighbors(Location id) const {
    std::vector<Location> results;

    for (Location move : moves_) {
      Location next{id.x + move.x, id.y + move.y};
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

  void add_wall(Location loc) { walls_.insert(loc); }

  int width() { return width_; }
  int height() { return height_; }

  bool is_wall(Location id) { return walls_.find(id) != walls_.end(); }
  virtual bool is_forest(Location id) { return false; }

private:
  static const std::array<Location, 4> moves_;
  int width_, height_;
  std::unordered_set<Location> walls_;
};

template <typename Location>
const std::array<Location, 4> SquareGrid<Location>::moves_ = {
    /* East, West, North, South */
    Location{1, 0}, Location{-1, 0}, Location{0, -1}, Location{0, 1}};

#endif // SQUARE_GRID_H_