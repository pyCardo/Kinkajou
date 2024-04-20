#include <cassert>
#include <iostream>

#include "core.hpp"
#include "gfx.hpp"

int main() {
  core::Board board;
  core::Move move;
  std::vector<core::Move> moves;

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

  std::array<sf::Color, 64> colorMap;
  gfx::SetColorMap(colorMap);

  sf::RenderWindow window(
      sf::VideoMode(gfx::WINDOW_DIMENSION, gfx::WINDOW_DIMENSION), "Chess");
  window.setFramerateLimit(60);

  sf::RectangleShape square;
  square.setSize(sf::Vector2f(gfx::SQUARE_SIZE_F, gfx::SQUARE_SIZE_F));

  sf::Event event;
  bool isMoving{false};
  char pressed;

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

            move.target = pressed;
            bool possibleMove{std::find(moves.begin(), moves.end(), move) !=
                              moves.end()};
            std::cout << int(move.target) << std::endl;

            if (isMoving && possibleMove) {
              board.makeMove(move);
              gfx::SetColorMap(colorMap);

            } else {

              moves.clear();
              gfx::SetColorMap(colorMap);

              if (board.position[static_cast<unsigned long int>(pressed)] !=
                  0) {
                move.current = pressed;

                core::GenerateMoves(board, moves, pressed);

                for (auto possible_move : moves) {
                  colorMap[static_cast<unsigned long int>(
                      possible_move.target)] =
                      gfx::COLOR_HIGHLIGHT;  // highlight possible move
                }
              } else {
                isMoving = !isMoving;
              }
            }
            isMoving = !isMoving;
          }
          break;

        default:
          break;
      }
    }
    gfx::displayBoard(board, square, window, colorMap, charToPiece);
    window.display();
  }
}