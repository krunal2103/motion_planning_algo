#include "../grid/grid_location.hpp"

#include <queue>
#include <random>
#include <unordered_map>
#include <vector>

double random(double low, double high) {
  std::random_device random_device;
  std::mt19937 engine{random_device()};
  std::uniform_real_distribution<double> dist(low, high);
  return dist(engine);
}

template <typename Location> class RRT {
public:
  RRT(std::vector<std::vector<Location>> obstacles, Location start,
      Location goal, int threshold)
      : obstacles_(obstacles), start_(start), goal_(goal),
        threshold_(threshold) {
    jump_size_ = (800 / 100.0 * 600 / 100.0) / 1.5;
    points_.push_back(start_);
    parent_.push_back(0);
  }

  void operator()() {
    bool updated = false;
    Location next_point;
    int nearest_index = 0;
    while (!updated) {
      auto new_point = sample_point();
      auto nearest_point = points_[0];
      nearest_index = 0;
      for (int i = 0; i < points_.size(); i++) {
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

      if (!path_found_) {
        updated = true;
        points_.push_back(next_point);
        parent_.push_back(nearest_index);
        check_if_destination_reached();
      }
    }
  }

  auto get_points() { return points_; }
  auto get_parent() { return parent_; }
  auto is_goal_reached() { return path_found_; }

private:
  Location sample_point() {
    if ((random(0.0, 1.0) - 0.05) <= std::numeric_limits<double>::epsilon() &&
        !path_found_)
      return goal_ + Location{5, 5};
    return Location{random(0.0, 800), random(0.0, 600)};
  }

  bool is_edge_obstacle_free(Location a, Location b) {
    for (auto const &polygon : obstacles_)
      if (line_segment_intersects_polygon(a, b, polygon))
        return false;
    return true;
  }

  bool check_near_goal(Location line_start, Location line_end,
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
    if (check_near_goal(points_[parent_[points_.size() - 1]], points_.back(),
                        goal_)) {
      path_found_ = true;
      std::cout << "Reached!!!!!!\n";
    }
  }

  std::vector<std::vector<Location>> obstacles_;
  Location start_, goal_;
  std::vector<Location> points_;
  std::vector<int> parent_;
  bool path_found_ = false;
  double jump_size_;
  int threshold_;
};