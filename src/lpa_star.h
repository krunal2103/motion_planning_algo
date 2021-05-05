#include "../grid/grid_location.hpp"

#include <queue>
#include <unordered_map>

template <typename Location> struct KeyComparator {
  int operator()(const std::pair<Location, std::pair<double, double>> &a,
                 const std::pair<Location, std::pair<double, double>> &b) {
    return a.first < b.first || (a.first == b.first && a.second < b.second);
  }
};

template <typename Location, template <typename L> typename Graph>
class LPA_star {
public:
  LPA_star(Graph<Location> graph, Location start, Location goal)
      : graph_(graph), start_(start), goal_(goal) {
    for (int y = 0; y != graph.height(); ++y) {
      for (int x = 0; x != graph.width(); ++x) {
        Location id{x, y};
        S_[id] = {std::numeric_limits<double>::infinity(),
                  std::numeric_limits<double>::infinity()};
      }
    }
  }

  auto operator()() {
    init();
    if (!std::is_heap(U_.begin(), U_.end(), KeyComparator<Location>())) {
      std::make_heap(U_.begin(), U_.end(), KeyComparator<Location>());
    }
    compute_shortest_path();
    return generate_path();
  }

  std::vector<Location> generate_path() {
    std::vector<Location> path;
    goal_.cost = S_[goal_].second;
    path.push_back(goal_);
    while (path[0] != start_) {
      auto current = path[0];
      for (auto &s : graph_.neighbors(current)) {
        s.cost = S_[s].second;
        if (S_[s].second < current.cost) {
          path.push_back(s);
          std::sort(path.begin(), path.end(), [](Location a, Location b) {
            return a.cost < b.cost;
          });
          break;
        }
      }
    }
    return path;
  }

private:
  void init() {
    U_.clear();
    for (auto &[s, p] : S_) {
      p.first = std::numeric_limits<double>::infinity();
      p.second = std::numeric_limits<double>::infinity();
    }
    S_[start_].second = 0;
    U_.push_back({start_, {heuristic(start_), 0}});
    std::make_heap(U_.begin(), U_.end(), KeyComparator<Location>());
  }

  double heuristic(Location a) {
    return std::abs(a.x - goal_.x) + std::abs(a.y - goal_.y);
  }

  std::pair<double, double> calculate_key(Location a) {
    return {std::min(S_[a].first, S_[a].second) + heuristic(a),
            std::min(S_[a].first, S_[a].second)};
  }

  double C(Location id) {
    if (graph_.is_wall(id)) return std::numeric_limits<double>::infinity();
    else if (graph_.is_forest(id)) return 5;
    else return 1;
  }

  void update_vertex(Location a) {
    if (a != start_) {
      double min = std::numeric_limits<double>::infinity();
      for (auto const &s : graph_.neighbors(a)) {
        min = std::min(min, S_[s].first + C(s));
      }
      S_[a].second = min;
    }

    auto it = std::find_if(U_.begin(), U_.end(),
                           [&](const auto &p) { return p.first == a; });
    if (it != U_.end()) {
      U_.erase(it);
      std::push_heap(U_.begin(), U_.end(), KeyComparator<Location>());
    }

    if (S_[a].first != S_[a].second) {
      U_.push_back({a, calculate_key(a)});
      std::push_heap(U_.begin(), U_.end(), KeyComparator<Location>());
    }
  }

  bool compare_key(const std::pair<double, double> &p1,
                   const std::pair<double, double> &p2) {
    return p1.first < p2.first ||
           (p1.first == p2.first && p1.second < p2.second);
  }

  void compute_shortest_path() {
    auto key = U_[0];
    while (compare_key(key.second, calculate_key(goal_)) ||
           S_[goal_].first != S_[goal_].second) {
      key = U_[0];
      std::pop_heap(U_.begin(), U_.end(), KeyComparator<Location>());
      U_.pop_back();
      if (S_[key.first].first > S_[key.first].second) {
        S_[key.first].first = S_[key.first].second;
        for (auto const &s : graph_.neighbors(key.first)) {
          update_vertex(s);
        }
      } else {
        S_[key.first].first = std::numeric_limits<double>::infinity();
        for (auto const &s : graph_.neighbors(key.first)) {
          update_vertex(s);
        }
        update_vertex(key.first);
      }
    }
  }

  Location start_, goal_;
  Graph<Location> graph_;
  std::unordered_map<Location, std::pair<double, double>> S_;
  std::vector<std::pair<Location, std::pair<double, double>>> U_;
};