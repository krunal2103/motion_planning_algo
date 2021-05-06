#ifndef SQUARE_GRID_GRAPHICS_H_
#define SQUARE_GRID_GRAPHICS_H_

#pragma once

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <unordered_map>

#include <vector>

constexpr int WIDTH = 800 ;
constexpr int HEIGHT = 600 ;
constexpr int SQUARE_SIZE = 50;

template <typename Location>
class SquareGridGraphics {
public:
  SquareGridGraphics(int width, int height) {
    width_ = width / SQUARE_SIZE;
    height_ = height / SQUARE_SIZE;
    for (int y = 0; y != height_; ++y) {
      for (int x = 0; x != width_; ++x) {
        Location id{x, y};
        std::array<std::pair<int, int>, 4> boundary;
        boundary[0].first = x * SQUARE_SIZE;
        boundary[0].second = y * SQUARE_SIZE;
        boundary[2].first = boundary[0].first + SQUARE_SIZE;
        boundary[2].second = boundary[0].second;
        boundary[1].first = boundary[0].first;
        boundary[1].second = boundary[0].second + SQUARE_SIZE;
        boundary[3].first = boundary[0].first + SQUARE_SIZE;
        boundary[3].second = boundary[0].second + SQUARE_SIZE;
        squares_[id] = boundary;
      }
    }
  }

  void set_start(Location start) {
    start_.setSize(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
    start_.setFillColor(sf::Color(208, 0, 240));
    auto boundary = squares_[start];
    start_.setPosition(boundary[0].first, boundary[0].second);
  }

  void set_goal(Location goal) {
    goal_.setSize(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
    goal_.setFillColor(sf::Color::Yellow);
    auto boundary = squares_[goal];
    goal_.setPosition(boundary[0].first, boundary[0].second);
  }

  void set_obstacles(std::vector<std::vector<Location>> obstacles) {
    int total_points = 0;
    for (auto const &obs : obstacles) {
      total_points += obs.size();
    }
    obstacles_.resize(total_points);

    int i = 0;
    for (auto const &obs : obstacles) {
      for (auto const &pt : obs) {
        auto boundary = squares_[pt];
        obstacles_[i].setSize(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
        obstacles_[i].setFillColor(sf::Color(89, 87, 98));
        obstacles_[i].setPosition(boundary[0].first, boundary[0].second);
        i++;
      }
    }
  }

  bool in_bounds(Location id) const {
    return 0 <= id.x && id.x < width_ && 0 <= id.y && id.y < height_;
  }

  bool passable(Location id) const {
    for (auto const &polygon : walls_) {
      auto it = std::find(polygon.begin(), polygon.end(), id);
      if (it != polygon.end())
        return false;
    }
    return true;
  }

  std::vector<Location> neighbors(Location id) const {
    std::vector<Location> results;

    for (Location move : moves_) {
      Location next{id.x + move.x, id.y + move.y};
      if (in_bounds(next) && passable(next)) {
        results.push_back(next);
      }
    }

    //    if ((id.x + id.y) % 2 == 0) {
    //      // see "Ugly paths" section for an explanation:
    //      std::reverse(results.begin(), results.end());
    //    }

    return results;
  }

  void add_wall(std::vector<Location> rect) { walls_.push_back(rect); }

  std::vector<std::vector<Location>> get_obstacles() { return walls_; }

  int width() { return width_; }
  int height() { return height_; }

  bool is_wall(Location id) {
    for (auto const &polygon : walls_) {
      auto it = std::find(polygon.begin(), polygon.end(), id);
      if (it != polygon.end())
        return true;
    }
    return false;
  }
  virtual bool is_forest(Location id) { return false; }

  auto draw_grid_square(Location id) {
    auto boundary = squares_[id];
    sf::RectangleShape square(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
    square.setFillColor(sf::Color::Blue);
    square.setPosition(boundary[0].first, boundary[0].second);
    return square;
  }

  virtual void draw(sf::RenderWindow &window,
                    const std::vector<Location> &nodes, std::vector<int> parent,
                    bool path_found) {
    sf::Vertex line[2];

    // Draw obstacles
    for (auto &poly : obstacles_)
      window.draw(poly);

//    for (auto const& id : nodes) {
//      window.draw(draw_grid_square(id));
//    }

    // Draw edges between nodes
    //      for(int i = nodes.size() - 1; i; i--) {
    //        Location par = nodes[parent[i]];
    //        line[0] = sf::Vertex(sf::Vector2f(par.x, par.y));
    //        line[1] = sf::Vertex(sf::Vector2f(nodes[i].x, nodes[i].y));
    //        window.draw(line, 2, sf::Lines);
    //      }

    // start and end
    window.draw(start_);
    window.draw(goal_);

    // If destination is reached then path is retraced and drawn
    //      if(path_found) {
    //        auto node = nodes.size() - 1;
    //        while(parent[node] != node) {
    //          int par = parent[node];
    //          line[0] = sf::Vertex(sf::Vector2f(nodes[par].x, nodes[par].y));
    //          line[1] = sf::Vertex(sf::Vector2f(nodes[node].x,
    //          nodes[node].y)); line[0].color = line[1].color = sf::Color::Red;
    //          // orange color window.draw(line, 2, sf::Lines); node = par ;
    //        }
    //      }
  }

private:
  static const std::array<Location, 4> moves_;
  int width_, height_;
  std::vector<std::vector<Location>> walls_;
  std::vector<sf::RectangleShape> obstacles_;
  sf::RectangleShape start_, goal_;
  std::unordered_map<Location, std::array<std::pair<int, int>, 4>> squares_;
};

template <typename Location>
const std::array<Location, 4> SquareGridGraphics<Location>::moves_ = {
    /* East, West, North, South */
    Location{1, 0}, Location{-1, 0}, Location{0, -1}, Location{0, 1}};

#endif // SQUARE_GRID_GRAPHICS_H_