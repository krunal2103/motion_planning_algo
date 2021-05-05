#include "../grid/square_grid_with_weights.h"
#include "../utils.h"

#include "rrt.h"

#include "../graphics.hpp"

int main() {
  auto grid = make_diagram5<GridLocation, SquareGridWithWeights>();
//  GridLocation start{100, 280};
//  GridLocation goal{175, 555};

//  RRT<GridLocation, SquareGridWithWeights> rrt(grid, start, goal, 2);
//
//  auto points = rrt(100000);
//
//  std::cout << points.size() << std::endl;
//
//  draw_grid<GridLocation>(grid, nullptr, nullptr, &points, &start, &goal);

//  sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "RRT");
//
//  GridGraphics<GridLocation> graphics(start, goal);
//
//  while (window.isOpen()) {
//    sf::Event event;
//    while (window.pollEvent(event)) {
//      if (event.type == sf::Event::Closed) {
//        window.close();
//        return 0;
//      }
//      window.clear();
//      graphics.draw(window, std::vector<GridLocation>(), false);
//      window.display();
//    }
//  }
  return 0;
}