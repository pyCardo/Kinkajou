#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <array>
#include <cassert>
#include <iostream>
#include <string>

namespace constants {
auto const COLOR_LIGHT = sf::Color(255, 255, 229, 255);
auto const COLOR_DARK = sf::Color(173, 115, 29, 255);
float const PNG_SIZE = 2048;
float const WINDOW_DIMENSION{700};
float SQUARE_SIZE = WINDOW_DIMENSION / 8;
}  // namespace constants

struct BoardState {
  std::array<char, 64> board{'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r',  //
                             'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p',  //
                             0,   0,   0,   0,   0,   0,   0,   0,    //
                             0,   0,   0,   0,   0,   0,   0,   0,    //
                             0,   0,   0,   0,   0,   0,   0,   0,    //
                             0,   0,   0,   0,   0,   0,   0,   0,    //
                             'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',  //
                             'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'};

  std::array<bool, 2> white_can_castle{true, true};
  std::array<bool, 2> black_can_castle{true, true};
};

struct Move {
  char start_square;
  char land_square;
};

void makeMove(BoardState& board, Move move) {
  auto start = board.board.begin() + move.start_square;
  auto land = board.board.begin() + move.land_square;

  *land = *start;
  *start = 0;
}

void showBoardCLI(BoardState& board) {
  for (auto it = board.board.begin(); it != board.board.end(); ++it) {
    std::cout << *it << ' ';
    if ((it - board.board.begin() + 1) % 8 == 0) {
      std::cout << '\n';
    }
  }
}

void showBoard(BoardState& board, sf::RenderWindow& window) {
  const float scale_factor{
      constants::SQUARE_SIZE /
      constants::PNG_SIZE};  // non dovrebbe essere dichiarato ad ogni
                             // iterazione
  int i{0};
  for (auto it = board.board.begin(); it != board.board.end(); ++it) {
    if (*it != 0) {
      sf::Texture
          piece_gfx;  // non dovrebbe essere dichiarato ad ogni iterazione
      std::string path{"../../pieces/" + std::string(1, *it) + ".png"};
      piece_gfx.loadFromFile(path);

      sf::Sprite piece_sprite;
      piece_sprite.setTexture(piece_gfx);
      piece_sprite.setScale(scale_factor, scale_factor);
      piece_sprite.setPosition(constants::SQUARE_SIZE * (i % 8),
                               constants::SQUARE_SIZE * (i / 8));

      window.draw(piece_sprite);
    }
    ++i;
  }
}

int main() {
  BoardState board;

  sf::RenderWindow window(
      sf::VideoMode(constants::WINDOW_DIMENSION, constants::WINDOW_DIMENSION),
      "Chess");
  sf::RectangleShape square;
  sf::Event event;
  square.setSize(sf::Vector2f(constants::SQUARE_SIZE, constants::SQUARE_SIZE));

  // generate board
  for (int i{0}; i < 64; i++) {
    int x = i % 8;
    int y = i / 8;
    square.setPosition(x * constants::SQUARE_SIZE, y * constants::SQUARE_SIZE);
    if ((x + y) % 2 == 0) {
      square.setFillColor(
          constants::COLOR_LIGHT);  // first square starts from 0
    } else {
      square.setFillColor(constants::COLOR_DARK);
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