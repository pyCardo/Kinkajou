#include <cassert>
#include <iostream>

#include "core.hpp"
#include "gfx.hpp"

int main() {
  core::Board board;
  core::Move move;
  gfx::Piece b('b');
  gfx::Piece B('B');
  gfx::Piece k('k');
  gfx::Piece K('K');
  gfx::Piece n('n');
  gfx::Piece N('N');
  gfx::Piece p('p');
  gfx::Piece P('P');
  gfx::Piece q('q');
  gfx::Piece Q('Q');
  gfx::Piece r('r');
  gfx::Piece R('R');

  std::unordered_map<char, gfx::Piece> charToPiece = {
      {'b', b}, {'B', B}, {'k', k}, {'K', K}, {'n', n}, {'N', N},
      {'p', p}, {'P', P}, {'q', q}, {'Q', Q}, {'r', r}, {'R', R},
  };

  sf::RenderWindow window(
      sf::VideoMode(gfx::WINDOW_DIMENSION, gfx::WINDOW_DIMENSION),
      "Chess");
  window.setFramerateLimit(60);

  sf::RectangleShape square;
  square.setSize(
      sf::Vector2f(gfx::SQUARE_SIZE_F, gfx::SQUARE_SIZE_F));

  sf::Event event;
  bool isMoving{false};
  char pressed;
  // board is displayed on execution, then updated when a move is detected

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
            pressed = gfx::detectSquare(location.x, location.y);

            if (isMoving) {
              move.target = pressed;
              board.makeMove(move);
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
    gfx::displayBoard(board, square, window, charToPiece);
    window.display();
  }
}