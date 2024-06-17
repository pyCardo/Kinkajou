#include <cassert>
#include <iostream>

#include "gfx.hpp"

int main() {
  try {
    std::string fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -";
    core::Board board(fen);
    core::Move move;
    std::vector<core::Move> moves;

    gfx::Piece b("bB");
    gfx::Piece B("wB");
    gfx::Piece k("bK");
    gfx::Piece K("wK");
    gfx::Piece n("bN");
    gfx::Piece N("wN");
    gfx::Piece p("bP");
    gfx::Piece P("wP");
    gfx::Piece q("bQ");
    gfx::Piece Q("wQ");
    gfx::Piece r("bR");
    gfx::Piece R("wR");

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
    bool notClicking{false};
    bool reversed{false};

    while (window.isOpen()) {
      // event handling
      sf::Event event;
      while (window.pollEvent(event)) {
        switch (event.type) {
          case sf::Event::Closed:
            window.close();
            break;

          case sf::Event::KeyPressed:
            switch (event.key.code) {
              case sf::Keyboard::F:
                reversed = !reversed;
                break;
                // real time input ->
                // sf::Keyboard::isKeyPressed(sf::Keyboard::Left)

              default:
                break;
            }

          default:
            break;
        }
      }

      if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && notClicking) {
        notClicking = false;
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
              if (!moves.empty()) {
                colorMap[static_cast<u32>(pressed)] =
                    gfx::COLOR_HIGHLIGHT_SELECTED;
              }

              isMoving = true;
            }
          }
        }
      } else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        notClicking = true;
      }  // manually handling mouse button release

      gfx::displayBoard(board, square, window, colorMap, charToPiece);
      window.display();
    }
  } catch (std::filesystem::filesystem_error const& e) {
    std::cout << "Exception thrown\n" << e.what() << '\n';
  } catch (std::runtime_error const& e) {
    std::cout << "Exception thrown\n" << e.what() << '\n';
  }
}