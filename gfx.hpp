#ifndef GFX_HPP
#define GFX_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>

#include "core.hpp"

namespace gfx {

auto const COLOR_LIGHT = sf::Color(240, 218, 181, 255);
auto const COLOR_DARK = sf::Color(180, 136, 99, 255);
auto const COLOR_HIGHLIGHT_LIGHT = sf::Color(230, 122, 110, 255);
auto const COLOR_HIGHLIGHT_DARK = sf::Color(211, 92, 71, 255);
int const PNG_SIZE = 2048;
int const WINDOW_DIMENSION = 700;
int const SQUARE_SIZE_I = WINDOW_DIMENSION / 8;
int const BOARD_SIZE = 64;
float const SQUARE_SIZE_F =
    WINDOW_DIMENSION /
    8.f;  // two different attributes to perform both integer and float division
float const SCALE_FACTOR = SQUARE_SIZE_F / PNG_SIZE;

class Piece {
  char c_;
  std::string path_;
  sf::Texture texture_;

 public:
  sf::Sprite sprite;
  Piece(char id) : c_{id} {
    path_ = "pieces/" + std::string(1, c_) +
            ".png";  // andrebbe rimossa la dipendenza dalla working directory
    texture_.loadFromFile(path_);
    sprite.setTexture(texture_);
    sprite.setScale(SCALE_FACTOR, SCALE_FACTOR);
  }
  void toScreen(sf::RenderWindow&, int, int);
};

void Piece::toScreen(sf::RenderWindow& window, int x, int y) {
  sprite.setPosition(SQUARE_SIZE_F * static_cast<float>(x),
                     SQUARE_SIZE_F * static_cast<float>(y));
  window.draw(sprite);
}

/* Qui ci sarebbe da scegliere: preferiamo avere questa funzione come void e
 * passarle un rect già creato, oppure vogliamo che lei lo crei e lo
 * restituisca? La prima opzione sembra più efficiente nell'uso di memoria*/
void spriteSetup(sf::RectangleShape& rect, int x, int y, sf::Color color) {
  rect.setPosition(static_cast<float>(x / SQUARE_SIZE_I) * SQUARE_SIZE_F,
                   static_cast<float>(y / SQUARE_SIZE_I) * SQUARE_SIZE_F);
  rect.setFillColor(color);
}

void displayBoard(const core::Board& board, sf::RectangleShape& square,
                  sf::RenderWindow& window,
                  const std::array<sf::Color, 64>& colorMap,
                  std::unordered_map<char, Piece>& charToPiece) {
  window.clear();

  for (unsigned long int i{0}; i < BOARD_SIZE; ++i) {
    int x = static_cast<int>(i % 8);
    int y = static_cast<int>(i / 8);

    if ((x + y) % 2 == 0) {  // first square starts from 0
      spriteSetup(square, x * SQUARE_SIZE_I, y * SQUARE_SIZE_I, colorMap[i]);
    } else {
      spriteSetup(square, x * SQUARE_SIZE_I, y * SQUARE_SIZE_I, colorMap[i]);
    }
    window.draw(square);
    // memory leak risolto se le linee "window.draw(square);" e
    // "charToPiece.at(id).toScreen(window, x, y);" sono rimosse

    if (board.position[i] != 0) {
      char id = board.position[i];
      // only compiles using map.at() instead of operator[]
      charToPiece.at(id).toScreen(window, x, y);
    }
  }
}

char detectSquare(int x, int y) {
  int a = x / SQUARE_SIZE_I;
  int b = y / SQUARE_SIZE_I;  // rounding to closest integer
  return static_cast<char>(a + b * 8);
}

// Location pxlCoordinates(int i) {
//   int x = static_cast<int>(i % 8);
//   int y = static_cast<int>(i / 8);

//   return Location{x * SQUARE_SIZE_I, y * SQUARE_SIZE_I};
// }

void setColorMap(std::array<sf::Color, 64>& colorMap) {
  for (unsigned long int i{0}; i < BOARD_SIZE; ++i) {
    int x = static_cast<int>(i % 8);
    int y = static_cast<int>(i / 8);

    if ((x + y) % 2 == 0) {  // first square starts from 0
      colorMap[i] = COLOR_LIGHT;
    } else {
      colorMap[i] = COLOR_DARK;
    }
  }
}

void highlightSquare(std::array<sf::Color, 64>& colorMap, int index) {
  if (colorMap[static_cast<unsigned long int>(index)] == COLOR_DARK) {
    colorMap[static_cast<unsigned long int>(index)] = COLOR_HIGHLIGHT_DARK;
  } else if (colorMap[static_cast<unsigned long int>(index)] == COLOR_LIGHT) {
    colorMap[static_cast<unsigned long int>(index)] = COLOR_HIGHLIGHT_LIGHT;
  }
}

}  // namespace gfx

#endif