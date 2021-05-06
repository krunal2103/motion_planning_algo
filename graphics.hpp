#include <SFML/Graphics.hpp>

#include <unordered_set>
#include <vector>

const int WIDTH = 800 ;
const int HEIGHT = 600 ;
const int RADIUS = 5 ;

template <typename Location> class GridGraphics {
public:
  GridGraphics(Location start, Location goal, std::vector<std::vector<Location>> obstacles) {
    start_.setRadius(RADIUS);
    start_.setFillColor(sf::Color(208, 0, 240));
    start_.setPosition(start.x, start.y);
    start_.setOrigin(RADIUS / 2, RADIUS / 2);

    goal_.setRadius(RADIUS);
    goal_.setFillColor(sf::Color::Blue);
    goal_.setPosition(goal.x, goal.y);
    goal_.setOrigin(RADIUS / 2, RADIUS / 2);

    obstacles_.resize(obstacles.size());
    for (int i = 0; i < obstacles.size(); i++) {
      obstacles_[i].setPointCount(obstacles[i].size());
      obstacles_[i].setFillColor(sf::Color(89, 87, 98));
      for (int j = 0; j < obstacles[i].size(); j++) {
        obstacles_[i].setPoint(j, sf::Vector2f(obstacles[i][j].x, obstacles[i][j].y));
      }
    }
  }

  void draw(sf::RenderWindow &window, const std::vector<Location> &nodes, std::vector<int> parent, bool path_found = false) {
    sf::Vertex line[2];

    // Draw obstacles
    for(auto& poly : obstacles_) window.draw(poly);

    // Draw edges between nodes
    for(int i = nodes.size() - 1; i; i--) {
      Location par = nodes[parent[i]];
      line[0] = sf::Vertex(sf::Vector2f(par.x, par.y));
      line[1] = sf::Vertex(sf::Vector2f(nodes[i].x, nodes[i].y));
      window.draw(line, 2, sf::Lines);
    }

    // start and end
    window.draw(start_);
    window.draw(goal_);

    // If destination is reached then path is retraced and drawn
    if(path_found) {
      auto node = nodes.size() - 1;
      while(parent[node] != node) {
        int par = parent[node];
        line[0] = sf::Vertex(sf::Vector2f(nodes[par].x, nodes[par].y));
        line[1] = sf::Vertex(sf::Vector2f(nodes[node].x, nodes[node].y));
        line[0].color = line[1].color = sf::Color::Red; // orange color
        window.draw(line, 2, sf::Lines);
        node = par ;
      }
    }
  }

private:
  std::vector<sf::ConvexShape> obstacles_;
  sf::CircleShape start_, goal_;
};