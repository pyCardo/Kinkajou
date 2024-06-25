#include <cassert>
#include <iostream>

#include "gfx.hpp"

int main() {
  try {
    std::string fen =
        "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq -";
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
    std::array<gfx::Piece, 8> promotionList = {Q, R, B, N, q, r, b, n};

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

    while (window.isOpen()) {
      // event handling
      sf::Event event;
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          window.close();
        }
      }

      if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && notClicking) {
        notClicking = false;

        auto location = sf::Mouse::getPosition(window);
        auto size = window.getSize();
        bool inBounds = location.x > 0 &&
                        location.x < static_cast<int>(size.x) &&
                        location.y > 0 && location.y < static_cast<int>(size.y);

        if (inBounds) {
          // to prevent any action if user's interaction with option bar
          char pressed = gfx::detectSquare(location.x, location.y);

          if (!isMoving) {
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
          } else {
            move.target = pressed;
            move.promotion = 0;

            long int occurrences{std::count(moves.begin(), moves.end(), move)};

            isMoving = false;
            gfx::setColorMap(colorMap);

            if (occurrences != 0) {    // move is legal
              if (occurrences != 1) {  // promotion
                sf::RenderWindow option(
                    sf::VideoMode(gfx::OPTION_WINDOW_WIDTH,
                                  gfx::OPTION_WINDOW_HEIGHT),
                    "Promotion", sf::Style::Titlebar);
                option.setPosition(sf::Vector2i(
                    static_cast<int>(sf::VideoMode::getDesktopMode().width -
                                     gfx::OPTION_WINDOW_WIDTH) /
                        2,
                    static_cast<int>(sf::VideoMode::getDesktopMode().height -
                                     gfx::OPTION_WINDOW_HEIGHT) /
                        2));
                bool optNotClicking{false};

                while (option.isOpen()) {
                  while (option.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                      option.close();
                      break;
                    }
                  }

                  gfx::drawOptionWindow(option, square, promotionList,
                                        board.whiteToMove);

                  if (sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
                      optNotClicking) {
                    optNotClicking = false;

                    auto optLocation = sf::Mouse::getPosition(option);
                    auto optSize = option.getSize();
                    bool optInBounds =
                        optLocation.x > 0 &&
                        optLocation.x < static_cast<int>(optSize.x) &&
                        optLocation.y > 0 &&
                        optLocation.y < static_cast<int>(optSize.y);

                    if (optInBounds) {
                      char optPressed =
                          gfx::detectSquare(optLocation.x, optLocation.y);
                      switch (optPressed) {
                        case 0:
                          move.promotion = 'q';
                          break;
                        case 1:
                          move.promotion = 'r';
                          break;
                        case 2:
                          move.promotion = 'b';
                          break;
                        case 3:
                          move.promotion = 'n';
                          break;
                        default:
                          move.promotion = 0;
                          break;
                      }
                      option.close();
                    }
                  } else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    optNotClicking = true;
                  }

                  option.display();
                }
                if (move.promotion != 0) {
                  board.makeMove(move);
                }
              } else {
                board.makeMove(move);
              }
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