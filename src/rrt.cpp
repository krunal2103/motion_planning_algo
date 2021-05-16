#include "../utils/utils.h"

#include "rrt.h"

#include "../grid/graphics.hpp"

int main() {
  auto obstacles = make_rrt_grid<GridLocation<double>>();
  GridLocation<double> start{50, 200};
  GridLocation<double> goal{600, 400};

  RRT<GridLocation<double>> rrt("RRT", obstacles, RADIUS + 1);
  if (!rrt.init(start, goal)) {
    std::cout << "Start or gaol inside an obstacle!!!\n";
    return -1;
  }

  sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "RRT");

  GridGraphics<GridLocation<double>> graphics;
  graphics.set_start(start);
  graphics.set_goal(goal);
  graphics.set_obstacles(obstacles);

  sf::Time delayTime = sf::milliseconds(5);

  while (window.isOpen()) {
    sf::Event event{};
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
        return 0;
      }
    }
    if (!rrt.is_destination_reached())
      rrt();
    sf::sleep(delayTime);
    window.clear();
    graphics.draw(window, rrt.get_points(), rrt.is_destination_reached(),
                  rrt.get_destination_index());
    window.display();
  }
  return 0;
}