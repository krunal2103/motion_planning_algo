#include <algorithm>
#include <array>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <queue>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

constexpr char* left = "\u2190";
constexpr char* up = "\u2191";
constexpr char* right = "\u2192";
constexpr char* down = "\u2193";

#define BLACK "\x1b[1;30m"
#define RED "\x1b[1;31m"
#define GREEN "\x1b[1;32m"
#define YELLOW "\x1b[1;33m"
#define BLUE "\x1b[1;34m"
#define MAGENTA "\x1b[1;35m"
#define CYAN "\x1b[1;36m"
#define WHITE "\x1b[1;37m"
#define RESET "\x1b[1;0m"

struct GridLocation {
  int x, y;
};

namespace std {

/* implement hash function so we can put GridLocation into an unordered_set */
template <> struct hash<GridLocation> {
  typedef GridLocation argument_type;
  typedef std::size_t result_type;
  std::size_t operator()(const GridLocation &id) const noexcept {
    return std::hash<int>()(id.x ^ (id.y << 4));
  }
};

} // namespace std

class SquareGrid {
public:
  SquareGrid(int width, int height) : width_(width), height_(height) {}

  bool in_bounds(GridLocation id) const {
    return 0 <= id.x && id.x < width_ && 0 <= id.y && id.y < height_;
  }

  bool passable(GridLocation id) const {
    return walls_.find(id) == walls_.end();
  }

  std::vector<GridLocation> neighbors(GridLocation id) const {
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

  void add_wall(GridLocation loc) { walls_.insert(loc); }

  int width() { return width_; }
  int height() { return height_; }

  bool is_wall(GridLocation id) {
    return walls_.find(id) != walls_.end();
  }

private:
  static std::array<GridLocation, 4> moves_;
  int width_, height_;
  std::unordered_set<GridLocation> walls_;
};

std::array<GridLocation, 4> SquareGrid::moves_ = {
    /* East, West, North, South */
    GridLocation{1, 0}, GridLocation{-1, 0}, GridLocation{0, -1},
    GridLocation{0, 1}};

// Helpers for GridLocation

bool operator==(GridLocation a, GridLocation b) {
  return a.x == b.x && a.y == b.y;
}

bool operator!=(GridLocation a, GridLocation b) { return !(a == b); }

bool operator<(GridLocation a, GridLocation b) {
  return std::tie(a.x, a.y) < std::tie(b.x, b.y);
}

std::basic_iostream<char>::basic_ostream &
operator<<(std::basic_iostream<char>::basic_ostream &out,
           const GridLocation &loc) {
  out << '(' << loc.x << ',' << loc.y << ')';
  return out;
}

// This outputs a grid. Pass in a distances map if you want to print
// the distances, or pass in a point_to map if you want to print
// arrows that point to the parent location, or pass in a path vector
// if you want to draw the path.
template <typename Graph>
void draw_grid(
    Graph &graph,
    std::unordered_map<GridLocation, double> *distances = nullptr,
    std::unordered_map<GridLocation, GridLocation> *point_to = nullptr,
    std::vector<GridLocation> *path = nullptr, GridLocation *start = nullptr,
    GridLocation *goal = nullptr) {
  const int field_width = 3;
  printf("\033[2J");
  printf("\033[%d;%dH", 0, 0);
  std::cout << std::string(field_width * graph.width() + 2 , '_') << '\n';
  for (int y = 0; y != graph.height(); ++y) {
    std::cout << "|";
    for (int x = 0; x != graph.width(); ++x) {
      GridLocation id{x, y};
      if (graph.is_wall(id)) {
        std::cout << MAGENTA << std::string(field_width, '#') << RESET;
      } else if (start && id == *start) {
        std::cout << RED << " A " << RESET;
      } else if (goal && id == *goal) {
        std::cout << YELLOW << " Z " << RESET;
      } else if (path != nullptr &&
                 find(path->begin(), path->end(), id) != path->end()) {
        std::cout << GREEN << " @ " << RESET;
      } else if (point_to != nullptr && point_to->count(id)) {
        GridLocation next = (*point_to)[id];
        if (next.x == x + 1) {
          std::cout << BLUE << " " << right << " "  << RESET;
        } else if (next.x == x - 1) {
          std::cout << BLUE << " " << left << " "  << RESET;
        } else if (next.y == y + 1) {
          std::cout << BLUE << " " << down << " "  << RESET;
        } else if (next.y == y - 1) {
          std::cout << BLUE << " " << up << " "  << RESET;
        } else {
          std::cout << " * ";
        }
      } else if (distances != nullptr && distances->count(id)) {
        std::cout << ' ' << std::left << std::setw(field_width - 1)
                  << (*distances)[id];
      } else {
        std::cout << " . ";
      }
    }
    std::cout << "|\n";
  }
  std::cout << std::string(field_width * graph.width() + 2, '~') << '\n';
}

void add_rect(SquareGrid &grid, int x1, int y1, int x2, int y2) {
  for (int x = x1; x < x2; ++x) {
    for (int y = y1; y < y2; ++y) {
      grid.add_wall(GridLocation{x, y});
    }
  }
}

SquareGrid make_diagram1() {
  SquareGrid grid(30, 15);
  add_rect(grid, 3, 3, 5, 12);
  add_rect(grid, 13, 4, 15, 15);
  add_rect(grid, 21, 0, 23, 7);
  add_rect(grid, 23, 5, 26, 7);
  return grid;
}

template <typename Location>
std::vector<Location>
generate_path(Location start, Location goal,
              std::unordered_map<Location, Location> came_from) {
  std::vector<Location> path;

  auto current = goal;
  while (current != start) {
    path.push_back(current);
    current = came_from[current];
  }
  path.push_back(start);
  std::reverse(path.begin(), path.end());
  return path;
}