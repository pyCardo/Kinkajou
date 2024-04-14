#include <cassert>
#include <iostream>

#include "core.hpp"
#include "gfx.hpp"

int main() {
  Board board;
  Piece b('b');
  Piece B('B');
  Piece k('k');
  Piece K('K');
  Piece n('n');
  Piece N('N');
  Piece p('p');
  Piece P('P');
  Piece q('q');
  Piece Q('Q');
  Piece r('r');
  Piece R('R');
  std::unordered_map<char, Piece> charToPiece = {
      {'b', b}, {'B', B}, {'k', k}, {'K', K}, {'n', n}, {'N', N},
      {'p', p}, {'P', P}, {'q', q}, {'Q', Q}, {'r', r}, {'R', R},
  };

  sf::RenderWindow window(
      sf::VideoMode(graphics::WINDOW_DIMENSION, graphics::WINDOW_DIMENSION),
      "Chess");
  window.setFramerateLimit(60);

  sf::RectangleShape square;
  square.setSize(
      sf::Vector2f(graphics::SQUARE_SIZE_F, graphics::SQUARE_SIZE_F));

  // generate board
  {
    int i{0};
    for (auto it = board.position.begin(); it != board.position.end(); ++it) {
      int x = i % 8;
      int y = i / 8;

      if ((x + y) % 2 == 0) {  // first square starts from 0
        sprite_setup(square, x * graphics::SQUARE_SIZE_I,
                     y * graphics::SQUARE_SIZE_I, graphics::COLOR_LIGHT);
      } else {
        sprite_setup(square, x * graphics::SQUARE_SIZE_I,
                     y * graphics::SQUARE_SIZE_I, graphics::COLOR_DARK);
      }
      window.draw(square);

      if (*it != 0) {
        char id = *it;
        // only compiles using map.at() instead of operator[]
        charToPiece.at(id).toScreen(window, x, y);
      }
      ++i;
    }
  }

  sf::Event event;
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