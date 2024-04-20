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

            if (!isMoving) {
              if (board.position[static_cast<long unsigned int>(pressed)] !=
                  0) {
                move.current = pressed;
                moves.clear();
                core::GenerateMoves(board, moves, move.current);

                for (auto possible_move : moves) {
                  gfx::HighLightSquare(colorMap, possible_move.target);
                }

                isMoving = true;
              }
            }

            if (isMoving) {
              move.target = pressed;
              bool moveIsPseudoLegal{
                  std::find(moves.begin(), moves.end(), move) != moves.end()};

              isMoving = false;
              gfx::SetColorMap(colorMap);

              if (moveIsPseudoLegal) {
                board.makeMove(move);
              } else {
                moves.clear();

                if (board.position[static_cast<long unsigned int>(pressed)] !=
                    0) {
                  move.current = pressed;
                  moves.clear();
                  core::GenerateMoves(board, moves, move.current);

                  for (auto possible_move : moves) {
                    gfx::HighLightSquare(colorMap, possible_move.target);
                  }

                  isMoving = true;
                }
              }
            }
          }

          break;

        default:
          break;
      }
      gfx::displayBoard(board, square, window, colorMap, charToPiece);
      window.display();
    }
  }
}