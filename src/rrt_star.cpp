#include "../utils.h"

#include "rrt.h"

#include "../grid/graphics.hpp"

int main() {
  auto obstacles = make_diagram5<GridLocation<double>>();
  GridLocation<double> start{100, 70};
  GridLocation<double> goal{600, 400};

  RRT<GridLocation<double>> rrt_star("RRT*", obstacles, RADIUS + 1);
  rrt_star.init(start, goal);

  sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "RRT");

  GridGraphics<GridLocation<double>> graphics;
  graphics.set_start(start);
  graphics.set_goal(goal);
  graphics.set_obstacles(obstacles);

  sf::Time delayTime = sf::milliseconds(5);

  int i = 0;
  while (window.isOpen()) {
    sf::Event event{};
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
        return 0;
      }
    }
    rrt_star();
    i++;

    if (i % 500 == 0) {
      std::cout << "Iterations: " << i << std::endl;
      if (!rrt_star.is_destination_reached())
        std::cout << "Not reached!!!\n";
      else
        std::cout << "Shortest distance: " << rrt_star.get_min_distance()
                  << std::endl;
    }

    sf::sleep(delayTime);
    window.clear();
    graphics.draw(window, rrt_star.get_points(),
                  rrt_star.is_destination_reached(),
                  rrt_star.get_destination_index());
    window.display();
  }
  return 0;
}