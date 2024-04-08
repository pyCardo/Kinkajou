#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <array>
#include <cassert>
#include <iostream>

namespace constants {
auto const COLOR_LIGHT = sf::Color(199, 157, 59, 255);
auto const COLOR_DARK = sf::Color(245, 232, 201, 255);
}  // namespace constants

struct BoardState {
  std::array<char, 64> board{-4, -2, -3, -5, -6, -3, -2, -4,  //
                             -1, -1, -1, -1, -1, -1, -1, -1,  //
                             0,  0,  0,  0,  0,  0,  0,  0,   //
                             0,  0,  0,  0,  0,  0,  0,  0,   //
                             0,  0,  0,  0,  0,  0,  0,  0,   //
                             0,  0,  0,  0,  0,  0,  0,  0,   //
                             1,  1,  1,  1,  1,  1,  1,  1,   //
                             4,  2,  3,  5,  6,  3,  2,  4};

  std::array<bool, 2> white_can_castle{true, true};
  std::array<bool, 2> black_can_castle{true, true};
};

int main() {
  int const len{800};
  int squareSize = len / 8;
  sf::RenderWindow window(sf::VideoMode(len, len), "Chess");
  sf::RectangleShape square;
  sf::Event event;
  square.setSize(sf::Vector2f(squareSize, squareSize));

  // generate board
  for (int i{0}; i < 64; i++) {
    int x = i % 8;
    int y = i / 8;
    square.setPosition(x * squareSize, y * squareSize);
    if ((x+y)%2==0) {
      square.setFillColor(constants::COLOR_DARK);
    } else {
      square.setFillColor(constants::COLOR_LIGHT);
    }
    window.draw(square);
  }

  window.setFramerateLimit(10);

  while (window.isOpen()) {
    // event handling
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.display();
  }
}