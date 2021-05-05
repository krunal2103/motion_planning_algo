#include "../grid/grid_location.hpp"

#include <queue>
#include <random>
#include <unordered_map>
#include <vector>

template <typename T> // Returns a random number in [low, high]
T random(T low, T high) {
  std::random_device random_device;
  std::mt19937 engine{random_device()};
  std::uniform_real_distribution<double> dist(low, high);
  return dist(engine);
}

template <typename Location, template <typename L> typename Graph> class RRT {
public:
  RRT(Graph<Location> graph, Location start, Location goal, int threshold)
      : graph_(graph), start_(start), goal_(goal), threshold_(threshold) {
    obstacles_ = graph_.get_obstacles();
    jump_size_ = (graph_.width() / 100.0 * graph_.height() / 100.0) / 1.5;
  }

  Location pick_random_point() {
    return Location{random(0, graph_.width()), random(0, graph_.height())};
  }

  bool is_edge_obstacle_free(Location a, Location b) {
    for (auto const &polygon : obstacles_)
      if (line_segment_intersects_polygon(a, b, polygon))
        return false;
    return true;
  }

  Location nearest_node(Location &new_node) {
    Location nearest_node {-1, -1};
    double dist = std::numeric_limits<double>::max();
    int index = 0;
    for (int i = 0; i < points_.size(); i++) {
      if (points_[i] == new_node)
        continue;
      auto new_dist = distance(points_[i], new_node);
      if (new_dist > threshold_)
        continue;
      if (!is_edge_obstacle_free(points_[i], new_node))
        continue;
      if (new_dist > dist)
        continue;
      dist = new_dist;
      index = i;
    }
    if (dist != std::numeric_limits<double>::max()) {
      nearest_node = points_[index];
      new_node.cost = nearest_node.cost + dist;
    }
    return nearest_node;
  }

  bool goal_visible(const Location &id) {
    if (is_edge_obstacle_free(id, goal_)) {
      if (distance(id, goal_) <= threshold_) {
        goal_.cost = distance(id, goal_) + id.cost;
        points_.push_back(goal_);
        return true;
      }
    }
    return false;
  }

  auto operator()(int max_iterations) {
    points_.clear();
    points_.push_back(start_);
    auto new_node = start_;
    if (goal_visible(new_node))
      return points_;
    int iteration = 0;
    while (iteration <= max_iterations) {
      iteration++;
      new_node = pick_random_point();
      auto nearest_n = nearest_node(new_node);
      points_.push_back(new_node);
      if (goal_visible(new_node))
        return points_;
    }
    points_.clear();
    return points_;
  }

  std::vector<Location> generate_path() {
  }

private:
  Location start_, goal_;
  Graph<Location> graph_;
  std::unordered_set<std::vector<Location>> obstacles_;
  std::vector<Location> points_;
  int threshold_;
  bool path_found_ = false;
  double jump_size_;
};