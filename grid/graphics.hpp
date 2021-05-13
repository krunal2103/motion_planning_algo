#include <SFML/Graphics.hpp>

#include <unordered_set>
#include <vector>

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;
constexpr int RADIUS = 10;

template <typename Location> class GridGraphics {
public:
  GridGraphics() = default;

  virtual void set_start(Location start) {
    start_.setRadius(RADIUS);
    start_.setFillColor(sf::Color::Red);
    start_.setPosition(start.x, start.y);
  }

  virtual void set_goal(Location goal) {
    goal_.setRadius(RADIUS);
    goal_.setFillColor(sf::Color::Yellow);
    goal_.setPosition(goal.x, goal.y);
  }

  virtual void set_obstacles(std::vector<std::vector<Location>> obstacles) {
    obstacles_.resize(obstacles.size());
    for (int i = 0; i < obstacles.size(); i++) {
      obstacles_[i].setPointCount(obstacles[i].size());
      obstacles_[i].setFillColor(sf::Color(102, 0, 51));
      for (int j = 0; j < obstacles[i].size(); j++) {
        obstacles_[i].setPoint(
            j, sf::Vector2f(obstacles[i][j].x, obstacles[i][j].y));
      }
    }
  }

  virtual void draw(sf::RenderWindow &window,
                    const std::vector<Location> &nodes, bool path_found,
                    int goal_index) {
    sf::Vertex line[2];

    // Draw obstacles
    for (auto &poly : obstacles_)
      window.draw(poly);

    // Draw edges between nodes
    for (int i = nodes.size() - 1; i; i--) {
      Location par = nodes[nodes[i].parent_index];
      line[0] = sf::Vertex(sf::Vector2f(par.x, par.y));
      line[1] = sf::Vertex(sf::Vector2f(nodes[i].x, nodes[i].y));
      window.draw(line, 2, sf::Lines);
    }

    // start and end
    window.draw(start_);
    window.draw(goal_);

    // If destination is reached then path is retraced and drawn
    if (path_found) {
      auto node_index = goal_index;
      while (nodes[node_index].parent_index != node_index) {
        int par = nodes[node_index].parent_index;
        line[0] = sf::Vertex(sf::Vector2f(nodes[par].x, nodes[par].y));
        line[1] =
            sf::Vertex(sf::Vector2f(nodes[node_index].x, nodes[node_index].y));
        line[0].color = line[1].color = sf::Color::Green;
        window.draw(line, 2, sf::Lines);
        node_index = par;
      }
    }
  }

private:
  std::vector<sf::ConvexShape> obstacles_;
  sf::CircleShape start_, goal_;
};