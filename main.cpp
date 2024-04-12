#include <cassert>

#include "core.hpp"
#include "gfx.hpp"

int main() {
  Board board;

  sf::RenderWindow window(
      sf::VideoMode(graphics::WINDOW_DIMENSION, graphics::WINDOW_DIMENSION),
      "Chess");
  sf::RectangleShape square;
  sf::Event event;
  square.setSize(sf::Vector2f(graphics::SQUARE_SIZE, graphics::SQUARE_SIZE));

  // generate board
  for (int i{0}; i < 64; i++) {
    int x = i % 8;
    int y = i / 8;
    square.setPosition(static_cast<float>(x) * graphics::SQUARE_SIZE,
                       static_cast<float>(y) * graphics::SQUARE_SIZE);
    if ((x + y) % 2 == 0) {
      square.setFillColor(graphics::COLOR_LIGHT);  // first square starts from 0
    } else {
      square.setFillColor(graphics::COLOR_DARK);
    }
    window.draw(square);
  }

  showBoard(board, window);

  window.setFramerateLimit(60);

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