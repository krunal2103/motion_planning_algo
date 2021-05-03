#ifndef GRID_LOCATION_HPP
#define GRID_LOCATION_HPP

#include <tuple>
#include <iostream>

#pragma once

struct GridLocation {
  int x, y;
};

namespace std {

/* implement hash function so we can put GridLocation into an unordered_set */
template <> struct hash<GridLocation> {
  std::size_t operator()(const GridLocation &id) const noexcept {
    return std::hash<int>()(id.x ^ (id.y << 4));
  }
};

} // namespace std

inline bool operator==(GridLocation a, GridLocation b) {
  return a.x == b.x && a.y == b.y;
}

inline bool operator!=(GridLocation a, GridLocation b) { return !(a == b); }

inline bool operator<(GridLocation a, GridLocation b) {
  return std::tie(a.x, a.y) < std::tie(b.x, b.y);
}

inline std::basic_iostream<char>::basic_ostream &
operator<<(std::basic_iostream<char>::basic_ostream &out,
           const GridLocation &loc) {
  out << '(' << loc.x << ',' << loc.y << ')';
  return out;
}

#endif // GRID_LOCATION_HPP