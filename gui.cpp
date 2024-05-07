#include <cassert>
#include <iostream>

#include "gfx.hpp"

int main() {
  try {
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
    gfx::setColorMap(colorMap);

    sf::RenderWindow window(
        sf::VideoMode(gfx::WINDOW_DIMENSION, gfx::WINDOW_DIMENSION), "Chess");
    window.setVerticalSyncEnabled(true);
    window.setPosition(
        sf::Vector2i(static_cast<int>(sf::VideoMode::getDesktopMode().width -
                                      gfx::WINDOW_DIMENSION) /
                         2,
                     static_cast<int>(sf::VideoMode::getDesktopMode().height -
                                      gfx::WINDOW_DIMENSION) /
                         2));

    sf::RectangleShape square;
    square.setSize(sf::Vector2f(gfx::SQUARE_SIZE_F, gfx::SQUARE_SIZE_F));

    bool isMoving{false};
    bool isClicking{false};

    while (window.isOpen()) {
      // event handling
      sf::Event event;
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          window.close();
        }
      }

      if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && isClicking) {
        isClicking = false;
        auto location = sf::Mouse::getPosition(window);
        char pressed = gfx::detectSquare(location.x, location.y);

        if (!isMoving) {
          if (board.accessBoard(pressed) != 0) {
            move.current = pressed;

            moves.clear();

            core::generateMoves(board, moves, move.current);

            for (auto possibleMove : moves) {
              gfx::highlightSquare(colorMap, possibleMove.target);
            }

            isMoving = true;
          }
        }

        if (isMoving) {
          move.target = pressed;
          bool moveIsPseudoLegal{std::find(moves.begin(), moves.end(), move) !=
                                 moves.end()};

          isMoving = false;
          gfx::setColorMap(colorMap);

          if (moveIsPseudoLegal) {
            board.makeMove(move);
          } else {
            moves.clear();

            if (board.accessBoard(pressed) != 0) {
              move.current = pressed;
              moves.clear();
              core::generateMoves(board, moves, move.current);

              for (auto possibleMove : moves) {
                gfx::highlightSquare(colorMap, possibleMove.target);
              }

              isMoving = true;
            }
          }
        }
      } else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        isClicking = true;
      }  // manually handling mouse button release

      gfx::displayBoard(board, square, window, colorMap, charToPiece);
      window.display();
    }
  } catch (std::filesystem::filesystem_error const& e) {
    std::cout << "Exception thrown\n" << e.what() << '\n';
  }
}