#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <array>
#include <cassert>
#include <iostream>
#include <string>

namespace constants {
auto const COLOR_LIGHT = sf::Color(199, 157, 59, 255);
auto const COLOR_DARK = sf::Color(245, 232, 201, 255);
int const len{800};
int squareSize = len / 8;
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
  const float scale_factor{0.05};

  int i{0};
  for (auto it = board.board.begin(); it != board.board.end(); ++it) {
    if (*it != 0) {
      sf::Texture piece_gfx;
      std::string path{"pieces/" + *it};
      path += ".png";
      piece_gfx.loadFromFile(path);

      sf::Sprite piece_sprite;
      piece_sprite.setTexture(piece_gfx);
      piece_sprite.setScale(scale_factor, scale_factor);
      piece_sprite.setPosition(constants::squareSize * i % 8,
                               constants::squareSize * i / 8);

      window.draw(piece_sprite);
    }

    ++i;
  }
}

int main() {
  BoardState board;

  sf::RenderWindow window(sf::VideoMode(constants::len, constants::len),
                          "Chess");
  sf::RectangleShape square;
  sf::Event event;
  square.setSize(sf::Vector2f(constants::squareSize, constants::squareSize));

  // generate board
  for (int i{0}; i < 64; i++) {
    int x = i % 8;
    int y = i / 8;
    square.setPosition(x * constants::squareSize, y * constants::squareSize);
    if ((x + y) % 2 == 0) {
      square.setFillColor(constants::COLOR_DARK);
    } else {
      square.setFillColor(constants::COLOR_LIGHT);
    }
    window.draw(square);
  }

  showBoard(board, window);

  window.setFramerateLimit(10);

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