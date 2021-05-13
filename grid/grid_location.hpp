#ifndef GRID_LOCATION_HPP
#define GRID_LOCATION_HPP

#include <cmath>
#include <iostream>
#include <tuple>
#include <vector>

#pragma once

template <typename T> class GridLocation {
public:
  GridLocation operator+(GridLocation b) {
    return {this->x + b.x, this->y + b.y};
  }

  GridLocation operator-(GridLocation b) {
    return {this->x - b.x, this->y - b.y};
  }

  template <typename T1> GridLocation operator/(T1 num) {
    return {this->x / num, this->y / num};
  }

  T dot(GridLocation b) { return this->x * b.x + this->y * b.y; }

  T cross(GridLocation b) const { return this->x * b.y - this->y * b.x; }

  T cross(GridLocation a, GridLocation b) const {
    return (a - *this).cross(b - *this);
  }

  T distance(GridLocation b) { return std::sqrt((*this - b).dot(*this - b)); }

  GridLocation steer(GridLocation b, double delta) {
    if ((this->distance(b) - delta) <= std::numeric_limits<double>::epsilon())
      return b;
    else {
      double theta = std::atan2(b.y - this->y, b.x - this->x);
      return {this->x + delta * std::cos(theta),
              this->y + delta * std::sin(theta)};
    }
  }

  T x, y, parent_index;
  double cost;
};

namespace std {

/* implement hash function so we can put GridLocation into an unordered_set */
template <> struct hash<GridLocation<double>> {
  std::size_t operator()(const GridLocation<double> &id) const noexcept {
    return std::hash<double>()(id.x + (id.y * 16));
  }
};

template <> struct hash<GridLocation<int>> {
  std::size_t operator()(const GridLocation<int> &id) const noexcept {
    return std::hash<int>()(id.x ^ (id.y << 4));
  }
};

} // namespace std

inline bool operator==(GridLocation<int> a, GridLocation<int> b) {
  return (a.x == b.x) && (a.y == b.y);
}

template <typename T>
inline bool operator==(GridLocation<T> a, GridLocation<T> b) {
  return (a.x - b.x) < std::numeric_limits<T>::epsilon() &&
         (a.y - b.y) < std::numeric_limits<T>::epsilon();
}

template <typename T>
inline bool operator!=(GridLocation<T> a, GridLocation<T> b) {
  return !(a == b);
}

template <typename T>
inline bool operator<(GridLocation<T> a, GridLocation<T> b) {
  return std::tie(a.x, a.y) < std::tie(b.x, b.y);
}

template <typename T>
inline std::basic_iostream<char>::basic_ostream &
operator<<(std::basic_iostream<char>::basic_ostream &out,
           const GridLocation<T> &loc) {
  out << '(' << loc.x << ',' << loc.y << ')';
  return out;
}

int sign(double x) { return x >= 0 ? x ? 1 : 0 : -1; }

inline bool intersect_on_line(double a, double b, double c, double d) {
  if ((a - b) > std::numeric_limits<double>::epsilon())
    std::swap(a, b);
  if ((c - d) > std::numeric_limits<double>::epsilon())
    std::swap(c, d);
  return std::max(a, c) <= std::max(b, d);
}

template <typename T>
bool check_intersection(const GridLocation<T> a, const GridLocation<T> b,
                        const GridLocation<T> c, const GridLocation<T> d) {
  if (c.cross(a, d) == 0 && c.cross(b, d) == 0)
    return intersect_on_line(a.x, b.x, c.x, d.x) &&
           intersect_on_line(a.y, b.y, c.y, d.y);

  return sign(a.cross(b, c)) != sign(a.cross(b, d)) &&
         sign(c.cross(d, a)) != sign(c.cross(d, b));
}

template <typename T>
bool line_segment_intersects_polygon(GridLocation<T> a, GridLocation<T> b,
                                     std::vector<GridLocation<T>> polygon) {
  for (int i = 0; i < polygon.size(); i++) {
    int next = i + 1;
    if (next == polygon.size())
      next = 0;
    if (check_intersection(a, b, polygon[i], polygon[next]))
      return true;
  }
  return false;
}

#endif // GRID_LOCATION_HPP