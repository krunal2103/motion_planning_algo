#ifndef GRID_LOCATION_HPP
#define GRID_LOCATION_HPP

#include <iostream>
#include <tuple>
#include <vector>
#include <cmath>

#pragma once

struct GridLocation {
  int x, y;
  int cost;
};

namespace std {

/* implement hash function so we can put GridLocation into an unordered_set */
template <> struct hash<GridLocation> {
  std::size_t operator()(const GridLocation &id) const noexcept {
    return std::hash<int>()(id.x ^ (id.y << 4));
  }
};

template <> struct hash<std::vector<GridLocation>> {
  std::size_t operator()(const std::vector<GridLocation> &v) const noexcept {
    GridLocation id{0, 0};
    for (auto const& l : v) {
      id.x += l.x;
      id.y += l.y;
    }
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

inline GridLocation operator+(GridLocation a, GridLocation b) {
  return {a.x + b.x, a.y + b.y};
}

inline GridLocation operator-(GridLocation a, GridLocation b) {
  return {a.x - b.x, a.y - b.y};
}

inline std::basic_iostream<char>::basic_ostream &
operator<<(std::basic_iostream<char>::basic_ostream &out,
           const GridLocation &loc) {
  out << '(' << loc.x << ',' << loc.y << ')';
  return out;
}

inline double distance(GridLocation a, GridLocation b) {
  auto x = a.x - b.x;
  auto y = a.y - b.y;
  return std::sqrt(x * x + y * y);
}

inline GridLocation steer(GridLocation a, GridLocation b, double delta) {
  if (distance(a, b) > delta)
    return b;
  else {
    double theta = std::atan2(b.y - a.y, b.x - a.x);
    return {int(a.x + delta * std::cos(theta)), int(a.y + delta * std::sin(theta))};
  }
}

inline int cross(GridLocation a, GridLocation b) {
  return a.x * b.y - a.y * b.x;
}

inline int cross(GridLocation l, GridLocation a, GridLocation b) {
  return cross(a - l, b - l);
}

inline bool intersect_on_line(int a, int b, int c, int d) {
  if ((a - b) > 0)
    std::swap(a, b);
  if ((c - d) > 0)
    std::swap(c, d);
  return std::max(a, c) <= std::max(b, d);
}

int sign(int x) { return x >= 0 ? x ? 1 : 0 : -1; }

bool check_intersection(const GridLocation a, const GridLocation b,
                        const GridLocation c, const GridLocation d) {
  if (cross(c, a, d) == 0 && cross(c, b, d) == 0)
    return intersect_on_line(a.x, b.x, c.x, d.x) &&
           intersect_on_line(a.y, b.y, c.y, d.y);

  return sign(cross(a, b, c)) != sign(cross(a, b, d)) &&
         sign(cross(c, d, a)) != sign(cross(c, d, b));
}

bool line_segment_intersects_polygon(GridLocation a, GridLocation b,
                                     std::vector<GridLocation> polygon) {
  for (int i = 0; i < polygon.size(); i++) {
    int next = i + 1;
    if (next == polygon.size()) next = 0;
    if (check_intersection(a, b, polygon[i], polygon[next])) return true;
  }
  return false;
}

#endif // GRID_LOCATION_HPP