#include "../grid/square_grid_interface.h"
#include "../utils.h"

#include <queue>
#include <thread>
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
    draw_grid<GridLocation>(graph, nullptr, &came_from, nullptr, &start, &goal);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
  return came_from;
}

int main() {

  auto grid = make_diagram1<GridLocation, SquareGrid>();
  GridLocation start{10, 10};
  GridLocation goal{1, 1};

  auto arrows = bfs(grid, start, goal);
  auto path = generate_path(start, goal, arrows);

  draw_grid<GridLocation>(grid, nullptr, &arrows, &path, &start, &goal);
  return 0;
}