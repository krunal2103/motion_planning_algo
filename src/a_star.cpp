#include "../grid/grid_location.hpp"
#include "../grid/square_grid_with_weights.h"
#include "../utils.h"

#include <queue>
#include <thread>
#include <unordered_map>
#include <vector>

template<typename Location>
double heuristic(Location a, Location b) {
  return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

template <typename Location, template <typename L> typename Graph>
std::tuple<std::unordered_map<Location, Location>,
    std::unordered_map<Location, double>>
a_star(Graph<Location> graph, Location start, Location goal) {
  typedef std::pair<double, Location> PQElement;
  std::priority_queue<PQElement, std::vector<PQElement>,
      std::greater<PQElement>>
      frontier;
  frontier.push({0, start});

  std::unordered_map<Location, Location> came_from;
  std::unordered_map<Location, double> total_cost;
  came_from[start] = start;
  total_cost[start] = 0;

  while (!frontier.empty()) {
    auto current = frontier.top();
    frontier.pop();

    if (current.second == goal)
      break;

    for (Location next : graph.neighbors(current.second)) {
      double cost = total_cost[current.second] + graph.cost(next);
      if (total_cost.find(next) == total_cost.end() ||
          cost < total_cost[next]) {
        total_cost[next] = cost;
        double priority = cost + heuristic(next, goal);
        came_from[next] = current.second;
        frontier.emplace(priority, next);
      }
    }
    draw_grid<GridLocation>(graph, &total_cost, nullptr, nullptr, &start, &goal);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
  return {came_from, total_cost};
}

int main() {

  auto grid = make_diagram4<GridLocation, SquareGridWithWeights>();
  GridLocation start{1, 4};
  GridLocation goal{8, 3};

  auto [arrows, cost] = a_star(grid, start, goal);
  auto path = generate_path(start, goal, arrows);

  draw_grid<GridLocation>(grid, nullptr, &arrows, &path, &start, &goal);
  return 0;
}