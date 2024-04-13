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
    if ((x + y) % 2 == 0) {  // first square starts from 0
      sprite_setup(square, x*graphics::SQUARE_SIZE, y*graphics::SQUARE_SIZE, graphics::COLOR_LIGHT);
    } else {
      sprite_setup(square, x*graphics::SQUARE_SIZE, y*graphics::SQUARE_SIZE, graphics::COLOR_DARK);
    }
    window.draw(square);
  }

  showBoard(board, window);

  window.setFramerateLimit(60);

  while (window.isOpen()) {
    // event handling
    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          window.close();
          break;
        case sf::Event::MouseButtonPressed: {
          auto location = sf::Mouse::getPosition(window);
          char pressed = detect_square(location.x, location.y);
          sprite_setup(square, location.x, location.y,
                       graphics::COLOR_HIGHLIGHT);
          window.draw(square);
          showBoard(board, window);
        } break;
        /*detect_square; check if it's empty; highlight_square; when clicked
         * again, move and clear starting square*/
        default:
          break;
      }
    }

    window.display();
  }
}