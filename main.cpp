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

  sf::Event event;
  Move move;
  bool isMoving{false};
  char pressed;

  displayBoard(board, square, window,
               charToPiece);  // board is displayed on execution, then updated
                              // when a move is detected

  while (window.isOpen()) {
    // event handling

    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          window.close();
          break;

        case sf::Event::MouseButtonPressed:
          if (event.mouseButton.button == sf::Mouse::Left) {
            auto location = sf::Mouse::getPosition(window);
            pressed = detect_square(location.x, location.y);

            if (isMoving) {
              move.target = pressed;
              board.makeMove(move);

              window.clear();
              displayBoard(board, square, window, charToPiece);
            } else {
              board.position[static_cast<unsigned long int>(pressed)] != 0
                  ? move.current = pressed
                  : isMoving = !isMoving;
            }
            isMoving = !isMoving;
          }
          break;

        default:
          break;
      }
    }

    window.display();
  }
}