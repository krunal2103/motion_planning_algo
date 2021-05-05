#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <vector>

constexpr char left[]   = "\u2190";
constexpr char up[]     = "\u2191";
constexpr char right[]  = "\u2192";
constexpr char down[]   = "\u2193";

#define BLACK "\x1b[1;90m"
#define RED "\x1b[1;91m"
#define GREEN "\x1b[1;92m"
#define YELLOW "\x1b[1;93m"
#define BLUE "\x1b[1;94m"
#define MAGENTA "\x1b[1;95m"
#define CYAN "\x1b[1;96m"
#define WHITE "\x1b[1;97m"
#define RESET "\x1b[1;0m"

#define BLACK_FG "\x1b[1;100m"
#define RED_FG "\x1b[1;41m"
#define GREEN_FG "\x1b[1;42m"
#define YELLOW_FG "\x1b[1;43m"
#define BLUE_FG "\x1b[1;44m"
#define MAGENTA_FG "\x1b[1;45m"
#define CYAN_FG "\x1b[1;46m"
#define WHITE_FG "\x1b[1;47m"

// This outputs a grid. Pass in a distances map if you want to print
// the distances, or pass in a point_to map if you want to print
// arrows that point to the parent location, or pass in a path vector
// if you want to draw the path.
template <typename Location, template<typename L> typename Graph>
void draw_grid(
    Graph<Location> &graph,
    std::unordered_map<Location, double> *distances = nullptr,
    std::unordered_map<Location, Location> *point_to = nullptr,
    std::vector<Location> *path = nullptr, Location *start = nullptr,
    Location *goal = nullptr) {
  const int field_width = 3;
  printf("\033[2J");
  printf("\033[%d;%dH", 0, 0);
  std::cout << std::string(field_width * graph.width() + 2 , '_') << '\n';
  for (int y = 0; y != graph.height(); ++y) {
    std::cout << "|";
    for (int x = 0; x != graph.width(); ++x) {
      Location id{x, y};
      if (graph.is_wall(id)) {
        std::cout << MAGENTA_FG << std::string(field_width, ' ') << RESET;
      } else if (start && id == *start) {
        std::cout << RED << " A " << RESET;
      } else if (goal && id == *goal) {
        std::cout << YELLOW << " Z " << RESET;
      } else if (path != nullptr &&
                 find(path->begin(), path->end(), id) != path->end()) {
        if (graph.is_forest(id)) {
          std::cout << BLACK_FG;
        }
        std::cout << GREEN << " @ " << RESET;
      } else if (point_to != nullptr && point_to->count(id)) {
        Location next = (*point_to)[id];
        if (graph.is_forest(id)) {
          std::cout << BLACK_FG;
        }
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
        if (graph.is_forest(id)) {
          std::cout << BLACK_FG;
        }
        std::cout << BLUE << ' ' << std::left << std::setw(field_width - 1)
                  << (*distances)[id] << RESET;
      } else {
        if (graph.is_forest(id)) {
          std::cout << BLACK_FG;
        }
        std::cout << " . " << RESET;
      }
    }
    std::cout << "|\n";
  }
  std::cout << std::string(field_width * graph.width() + 2, '~') << '\n';
}

template<typename Location, typename Graph>
void add_rect(Graph &grid, int x1, int y1, int x2, int y2) {
  std::vector<Location> rect;
  for (int x = x1; x < x2; ++x) {
    for (int y = y1; y < y2; ++y) {
      rect.push_back(Location{x, y});
    }
  }
  grid.add_wall(rect);
}

template<typename Location, typename Graph>
void add_polygon(Graph &grid, std::vector<Location> &polygon, Location l) {
  polygon.push_back(l);
  grid.add_wall(polygon);
}

template<typename Location, typename Graph, typename... Points>
void add_polygon(Graph &grid, std::vector<Location> &polygon, Location l, Points... points) {
  polygon.push_back(l);
  add_polygon(grid, polygon, points...);
}

template<typename Location, template<typename L> typename Graph>
auto make_diagram1() {
  Graph<Location> grid(30, 15);
  add_rect<Location>(grid, 3, 3, 5, 12);
  add_rect<Location>(grid, 13, 4, 15, 15);
  add_rect<Location>(grid, 21, 0, 23, 7);
  add_rect<Location>(grid, 23, 5, 26, 7);
  return grid;
}

template<typename Location, template<typename L> typename Graph>
auto make_diagram4() {
  Graph<Location> grid(10, 10);
  add_rect<Location, Graph<Location>>(grid, 1, 7, 4, 9);
  add_rect<Location, Graph<Location>>(grid, 8, 5, 10, 6);
//  typedef Location L;
//  grid.add_forests(std::unordered_set<Location> {
//      L{3, 4}, L{3, 5}, L{4, 1}, L{4, 2},
//      L{4, 3}, L{4, 4}, L{4, 5}, L{4, 6},
//      L{4, 7}, L{4, 8}, L{5, 1}, L{5, 2},
//      L{5, 3}, L{5, 4}, L{5, 5}, L{5, 6},
//      L{5, 7}, L{5, 8}, L{6, 2}, L{6, 3},
//      L{6, 4}, L{6, 5}, L{6, 6}, L{6, 7},
//      L{7, 3}, L{7, 4}, L{7, 5}, L{4, 0},
//      L{8, 5}, L{9, 5}
//  });
  return grid;
}

template<typename Location, template<typename L> typename Graph>
auto make_diagram5() {
  Graph<Location> grid(800, 600);
  typedef Location L;
  std::vector<Location> polygon;
  add_polygon<Location>(grid, polygon, L{3, 3}, L{5, 12}, L{100, 500}, L{25, 325});
//  add_rect<Location>(grid, 13, 4, 15, 15);
//  add_rect<Location>(grid, 21, 0, 23, 7);
//  add_rect<Location>(grid, 23, 5, 26, 7);
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
  return path;
}