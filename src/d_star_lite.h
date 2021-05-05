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
class D_star_lite {
public:
  D_star_lite(Graph<Location> graph, Location start, Location goal)
      : graph_(graph), start_(start), goal_(goal), km_(0) {
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
    start_.cost = S_[start_].second;
    path.push_back(start_);
    while (path[0] != goal_) {
      auto current = path[0];
      for (auto &s : graph_.neighbors(current)) {
        s.cost = S_[s].second;
        if (S_[s].second <= current.cost) {
          path.push_back(s);
          std::sort(path.begin(), path.end(),
                    [](Location a, Location b) { return a.cost < b.cost; });
          break;
        }
      }
    }
    auto reversed_path = path;
    double start_cost = reversed_path.back().cost;
    for (auto &n : reversed_path) {
      n.cost = start_cost - n.cost;
    }
    std::reverse(reversed_path.begin(), reversed_path.end());
    return reversed_path;
  }

private:
  void init() {
    U_.clear();
    km_ = 0;
    for (auto &[s, p] : S_) {
      p.first = std::numeric_limits<double>::infinity();
      p.second = std::numeric_limits<double>::infinity();
    }
    S_[goal_].second = 0;
    U_.push_back({goal_, {heuristic(goal_), 0}});
    std::make_heap(U_.begin(), U_.end(), KeyComparator<Location>());
  }

  double heuristic(Location a) {
    return std::abs(a.x - start_.x) + std::abs(a.y - start_.y);
  }

  std::pair<double, double> calculate_key(Location a) {
    return {std::min(S_[a].first, S_[a].second) + heuristic(a) + km_,
            std::min(S_[a].first, S_[a].second)};
  }

  double C(Location id) {
    if (graph_.is_wall(id))
      return std::numeric_limits<double>::infinity();
    else if (graph_.is_forest(id))
      return 5;
    else
      return 1;
  }

  void update_vertex(Location a) {
    auto it = std::find_if(U_.begin(), U_.end(),
                           [&](const auto &p) { return p.first == a; });
    if (S_[a].first != S_[a].second) {
      if (it != U_.end()) {
        it->second = calculate_key(a);
      } else {
        U_.push_back({a, calculate_key(a)});
        std::push_heap(U_.begin(), U_.end(), KeyComparator<Location>());
      }
    } else if (it != U_.end()) {
      U_.erase(it);
      std::push_heap(U_.begin(), U_.end(), KeyComparator<Location>());
    }
  }

  bool compare_key(const std::pair<double, double> &p1,
                   const std::pair<double, double> &p2) {
    return p1.first < p2.first ||
           (p1.first == p2.first && p1.second < p2.second);
  }

  void compute_shortest_path() {
    while (compare_key(U_[0].second, calculate_key(start_)) ||
           S_[start_].second > S_[start_].first) {
      auto u = U_[0].first;
      auto k_old = U_[0].second;
      auto k_new = calculate_key(u);
      if (k_old < k_new) {
        U_[0].second = k_new;
      } else if (S_[u].first > S_[u].second) {
        S_[u].first = S_[u].second;
        std::pop_heap(U_.begin(), U_.end(), KeyComparator<Location>());
        U_.pop_back();
        for (auto const &s : graph_.neighbors(u)) {
          if (s != goal_) {
            S_[s].second = std::min(S_[s].second, C(s) + S_[u].first);
            update_vertex(s);
          }
        }
      } else {
        auto g_old = S_[u].first;
        S_[u].first = std::numeric_limits<double>::infinity();
        for (auto const &s : graph_.neighbors(u)) {
          if (S_[s].second == C(s) + g_old) {
            if (s != goal_) {
              double min = std::numeric_limits<double>::infinity();
              for (auto &s_prime : graph_.neighbors(s)) {
                min = std::min(min, C(s_prime) + S_[s_prime].first);
              }
              S_[s].second = min;
            }
          }
          update_vertex(s);
        }
        if (S_[u].second == C(u) + g_old) {
          if (u != goal_) {
            double min = std::numeric_limits<double>::infinity();
            for (auto &s_prime : graph_.neighbors(u)) {
              min = std::min(min, C(s_prime) + S_[s_prime].first);
            }
            S_[u].second = min;
          }
        }
        update_vertex(u);
      }
    }
  }

  Location start_, goal_;
  Graph<Location> graph_;
  std::unordered_map<Location, std::pair<double, double>> S_;
  std::vector<std::pair<Location, std::pair<double, double>>> U_;
  double km_;
};