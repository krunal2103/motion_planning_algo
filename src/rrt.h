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

  bool init(Location root, Location destination) {
    for (auto const &obstacle : obstacles_) {
      if (point_inside_polygon(root, obstacle) ||
          point_inside_polygon(destination, obstacle)) {
        return false;
      }
    }
    points_.push_back(root);
    points_.back().cost = 0;
    points_.back().parent_index = 0;
    destination_ = destination;
    return true;
  }

  void operator()() {
    while (!extend(sample_point())) {
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

  std::tuple<Location, int> nearest_neighbor(Location new_point) {
    int nearest_index;
    auto nearest_point = points_[0];
    for (int i = 0; i < points_.size(); i++) {
      if (path_found_ && random(0.0, 1.0) < 0.25) {
        points_[i].cost = points_[points_[i].parent_index].cost +
                          points_[points_[i].parent_index].distance(points_[i]);
      }
      auto point = points_[i];
      auto next = point.steer(new_point, random(0.3, 1.0) * jump_size_);
      if ((point.distance(new_point) <= nearest_point.distance(new_point)) &&
          is_edge_obstacle_free(point, next)) {
        nearest_point = point;
        next_point_ = next;
        nearest_index = i;
      }
    }
    return {nearest_point, nearest_index};
  }

  bool extend(Location new_point) {
    // nearest_neighbor
    auto [nearest_point, nearest_index] = nearest_neighbor(new_point);

    if (!is_edge_obstacle_free(nearest_point, next_point_))
      return false;

    if (type_ == "RRT") {
      next_point_.parent_index = nearest_index;
      next_point_.cost =
          points_[nearest_index].cost + nearest_point.distance(next_point_);
      points_.push_back(next_point_);
      if (!path_found_)
        check_if_destination_reached();
      return true;
    }

    std::vector<int> nearby_point_indices;

    for (int i = 0; i < points_.size(); i++) {
      if ((points_[i].distance(next_point_) - jump_size_) <=
              std::numeric_limits<double>::epsilon() &&
          is_edge_obstacle_free(points_[i], next_point_)) {
        nearby_point_indices.push_back(i);
      }
    }

    int p = nearest_index;
    auto min_cost = points_[p].cost + points_[p].distance(next_point_);
    for (auto const &i : nearby_point_indices) {
      if (((points_[i].cost + points_[i].distance(next_point_)) - min_cost) <=
          std::numeric_limits<double>::epsilon()) {
        min_cost = points_[i].cost + points_[i].distance(next_point_);
        p = i;
      }
    }

    next_point_.cost = min_cost;
    next_point_.parent_index = p;
    points_.push_back(next_point_);
    if (!path_found_)
      check_if_destination_reached();
    rewire(nearby_point_indices);
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
    if (check_near_location(points_[points_.back().parent_index],
                            points_.back(), destination_)) {
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
        int p_old = points_[cur].parent_index;
        points_[cur].parent_index = p;
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
  Location next_point_;
};