#include "../utils.h"

#include "rrt.h"

#include "../graphics.hpp"

int main() {
  auto obstacles = make_diagram5<GridLocation<double>>();
  GridLocation<double> start{100, 70};
  GridLocation<double> goal{600, 400};

  RRT<GridLocation<double>> rrt(obstacles, start, goal, RADIUS);

  sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "RRT");

  GridGraphics<GridLocation<double>> graphics(start, goal, obstacles);

  sf::Time delayTime = sf::milliseconds(5);

  while (window.isOpen()) {
    sf::Event event{};
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
        return 0;
      }
    }
    if (!rrt.is_goal_reached())
      rrt();
    sf::sleep(delayTime);
    window.clear();
    graphics.draw(window, rrt.get_points(), rrt.get_parent(),
                  rrt.is_goal_reached());
    window.display();
  }
  return 0;
}