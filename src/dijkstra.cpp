#include "../grid/grid_location.hpp"
#include "../grid/square_grid_with_weights.h"
#include "../utils.h"

#include <queue>
#include <thread>
#include <unordered_map>
#include <vector>

template <typename Location, template <typename L> typename Graph>
std::tuple<std::unordered_map<Location, Location>,
           std::unordered_map<Location, double>>
dijsktra(Graph<Location> graph, Location start, Location goal) {
  typedef std::pair<double, Location> PQElement;
  std::priority_queue<PQElement, std::vector<PQElement>, std::greater<>>
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
        came_from[next] = current.second;
        frontier.emplace(cost, next);
      }
    }
    draw_grid<Location>(graph, &total_cost, nullptr, nullptr, &start,
                            &goal);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
  return {came_from, total_cost};
}

int main() {

  auto grid = make_diagram4<GridLocation<int>, SquareGridWithWeights>();
  GridLocation<int> start{1, 4};
  GridLocation<int> goal{8, 3};

  auto [arrows, cost] = dijsktra(grid, start, goal);
  auto path = generate_path(start, goal, arrows);

  draw_grid<GridLocation<int>>(grid, nullptr, &arrows, &path, &start, &goal);
  return 0;
}