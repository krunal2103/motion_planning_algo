#include "utils.h"

#include <queue>
#include <unordered_map>

template <typename Location, typename Graph>
std::unordered_map<Location, Location> bfs(Graph graph, Location start,
                                           Location goal) {
  std::queue<Location> frontier;
  frontier.push(start);

  std::unordered_map<Location, Location> came_from;
  came_from[start] = start;

  while (!frontier.empty()) {
    auto current = frontier.front();
    frontier.pop();

    if (current == goal)
      break;

    for (Location next : graph.neighbors(current)) {
      if (came_from.find(next) == came_from.end()) {
        frontier.push(next);
        came_from[next] = current;
      }
    }
  }
  return came_from;
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

int main() {

  auto grid = make_diagram1();
  GridLocation start{10, 10};
  GridLocation goal{1, 1};

  auto arrows = bfs(grid, start, goal);
  auto path = generate_path(start, goal, arrows);

  draw_grid(grid, nullptr, &arrows, &path, &start, &goal);
  return 0;
}