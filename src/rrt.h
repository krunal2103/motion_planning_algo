#include "../grid/grid_location.hpp"

#include <queue>
#include <random>
#include <utility>
#include <vector>

constexpr double GOAL_SAMPLING_PROBABILITY = 0.05;

double random(double low, double high) {
  std::random_device random_device;
  std::mt19937 engine{random_device()};
  std::uniform_real_distribution<double> dist(low, high);
  return dist(engine);
}

template <typename Location> class RRT {
public:
  RRT(std::string type, std::vector<std::vector<Location>> obstacles,
      int threshold)
      : obstacles_(obstacles), threshold_(threshold), type_(std::move(type)),
        destination_index_(0) {
    jump_size_ = (800 / 100.0 * 600 / 100.0) / 1.5;
  }

  void init(Location root, Location destination) {
    points_.push_back(root);
    points_.back().cost = 0;
    points_.back().parent = 0;
    destination_ = destination;
  }

  void operator()() {
    bool updated = false;
    Location next_point;
    int nearest_index = 0;
    double min_cost = std::numeric_limits<double>::infinity();
    std::vector<int> nearby_point_indices;
    while (!updated) {
      auto new_point = sample_point();
      auto nearest_point = points_[0];
      nearest_index = 0;
      for (int i = 0; i < points_.size(); i++) {
        if (path_found_ && random(0.0, 1.0) < 0.25) {
          points_[i].cost = points_[points_[i].parent].cost +
                            points_[points_[i].parent].distance(points_[i]);
        }

        auto point = points_[i];
        auto next = point.steer(new_point, random(0.3, 1.0) * jump_size_);
        if ((point.distance(new_point) <= nearest_point.distance(new_point)) &&
            is_edge_obstacle_free(point, next)) {
          nearest_point = point;
          next_point = next;
          nearest_index = i;
        }
      }

      if (!is_edge_obstacle_free(nearest_point, next_point))
        continue;

      if (type_ == "RRT") {
        updated = true;
        next_point.parent = nearest_index;
        next_point.cost =
            points_[nearest_index].cost + nearest_point.distance(next_point);
        points_.push_back(next_point);
        if (!path_found_)
          check_if_destination_reached();
        continue;
      }

      for (int i = 0; i < points_.size(); i++) {
        if ((points_[i].distance(next_point) - jump_size_) <=
                std::numeric_limits<double>::epsilon() &&
            is_edge_obstacle_free(points_[i], next_point)) {
          nearby_point_indices.push_back(i);
        }
      }

      int p = nearest_index;
      min_cost = points_[p].cost + points_[p].distance(next_point);
      for (auto const &i : nearby_point_indices) {
        if (((points_[i].cost + points_[i].distance(next_point)) - min_cost) <=
            std::numeric_limits<double>::epsilon()) {
          min_cost = points_[i].cost + points_[i].distance(next_point);
          p = i;
        }
      }

      next_point.cost = min_cost;
      next_point.parent = p;
      points_.push_back(next_point);
      updated = true;
      if (!path_found_)
        check_if_destination_reached();
      rewire(nearby_point_indices);
    }
  }

  auto get_points() { return points_; }
  auto is_destination_reached() { return path_found_; }
  int get_destination_index() { return destination_index_; }
  double get_min_distance() { return points_[destination_index_].cost; }

private:
  Location sample_point() {
    if ((random(0.0, 1.0) - GOAL_SAMPLING_PROBABILITY) <=
            std::numeric_limits<double>::epsilon() &&
        !path_found_)
      return destination_ + Location{5, 5};
    return Location{random(0.0, 800), random(0.0, 600)};
  }

  bool is_edge_obstacle_free(Location a, Location b) {
    for (auto const &polygon : obstacles_)
      if (line_segment_intersects_polygon(a, b, polygon))
        return false;
    return true;
  }

  bool check_near_location(Location line_start, Location line_end,
                           Location center_loc) {
    auto ac = center_loc - line_start;
    auto ab = line_end - line_start;
    auto ab2 = ab.dot(ab);
    auto acab = ac.dot(ab);
    auto t = acab / ab2;

    if (t < 0)
      t = 0;
    else if (t > 1)
      t = 1;

    Location h;
    h.x = ((ab.x * t) + line_start.x) - center_loc.x;
    h.y = ((ab.y * t) + line_start.y) - center_loc.y;
    auto h2 = h.dot(h);
    return (h2 <= (threshold_ * threshold_));
  }

  void check_if_destination_reached() {
    if (check_near_location(points_[points_.back().parent], points_.back(),
                            destination_)) {
      path_found_ = true;
      destination_index_ = points_.size() - 1;
      std::cout << "Reached: "
                << "\tcost: " << points_.back().cost << "\n";
    }
  }

  void rewire(const std::vector<int> &nearby) {
    for (auto const &i : nearby) {
      int p = points_.size() - 1, cur = i;

      while (((points_[p].cost + points_[p].distance(points_[cur])) -
              points_[cur].cost) <= std::numeric_limits<double>::epsilon()) {
        int p_old = points_[cur].parent;
        points_[cur].parent = p;
        points_[cur].cost = points_[p].cost + points_[p].distance(points_[cur]);
        p = cur;
        cur = p_old;
      }
    }
  }

  std::vector<std::vector<Location>> obstacles_;
  Location destination_;
  std::vector<Location> points_;
  bool path_found_ = false;
  double jump_size_;
  int threshold_;
  std::string type_;
  int destination_index_;
};