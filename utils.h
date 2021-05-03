#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <vector>

constexpr char left[]   = "\u2190";
constexpr char up[]     = "\u2191";
constexpr char right[]  = "\u2192";
constexpr char down[]   = "\u2193";

#define BLACK "\x1b[1;30m"
#define RED "\x1b[1;31m"
#define GREEN "\x1b[1;32m"
#define YELLOW "\x1b[1;33m"
#define BLUE "\x1b[1;34m"
#define MAGENTA "\x1b[1;35m"
#define CYAN "\x1b[1;36m"
#define WHITE "\x1b[1;37m"
#define RESET "\x1b[1;0m"

// This outputs a grid. Pass in a distances map if you want to print
// the distances, or pass in a point_to map if you want to print
// arrows that point to the parent location, or pass in a path vector
// if you want to draw the path.
template <typename Location, typename Graph>
void draw_grid(
    Graph &graph,
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
        std::cout << MAGENTA << std::string(field_width, '#') << RESET;
      } else if (start && id == *start) {
        std::cout << RED << " A " << RESET;
      } else if (goal && id == *goal) {
        std::cout << YELLOW << " Z " << RESET;
      } else if (path != nullptr &&
                 find(path->begin(), path->end(), id) != path->end()) {
        std::cout << GREEN << " @ " << RESET;
      } else if (point_to != nullptr && point_to->count(id)) {
        Location next = (*point_to)[id];
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

template<typename Location, typename Graph>
void add_rect(Graph &grid, int x1, int y1, int x2, int y2) {
  for (int x = x1; x < x2; ++x) {
    for (int y = y1; y < y2; ++y) {
      grid.add_wall(Location{x, y});
    }
  }
}

template<typename Location, typename Graph>
auto make_diagram1() {
  Graph grid(30, 15);
  add_rect<Location>(grid, 3, 3, 5, 12);
  add_rect<Location>(grid, 13, 4, 15, 15);
  add_rect<Location>(grid, 21, 0, 23, 7);
  add_rect<Location>(grid, 23, 5, 26, 7);
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