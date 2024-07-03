#ifndef GFX_HPP
#define GFX_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <filesystem>
#include <string>

#include "core.hpp"

namespace gfx {

auto const COLOR_LIGHT = sf::Color(241, 217, 192, 255);
auto const COLOR_DARK = sf::Color(150, 100, 75, 255);
auto const COLOR_HIGHLIGHT_LIGHT = sf::Color(230, 122, 110, 255);
auto const COLOR_HIGHLIGHT_DARK = sf::Color(185, 65, 45, 255);
auto const COLOR_HIGHLIGHT_SELECTED = sf::Color(237, 151, 52, 255);
int const PNG_SIZE = 2048;
int const WINDOW_DIMENSION = 600;
int const SQUARE_SIZE_I = WINDOW_DIMENSION / 8;
int const OPTION_WINDOW_WIDTH = 4 * SQUARE_SIZE_I;
int const OPTION_WINDOW_HEIGHT = SQUARE_SIZE_I;
int const BOARD_SIZE = 64;
float const SQUARE_SIZE_F =
    WINDOW_DIMENSION /
    8.f;  // two different attributes to perform both integer and float division
float const SCALE_FACTOR = SQUARE_SIZE_F / PNG_SIZE;

class Piece {
  char c_;
  std::filesystem::path path_;
  sf::Texture texture_;

 public:
  sf::Sprite sprite;
  Piece(std::string id) {
    path_.assign("pieces/");
    path_ /= id + ".png";

    if (!std::filesystem::exists(path_)) {
      throw std::filesystem::filesystem_error(
          "Can't load texture", path_,
          std::make_error_code(std::errc::no_such_file_or_directory));
    }

    texture_.loadFromFile(path_.native());
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

  for (u32 i{0}; i < BOARD_SIZE; ++i) {
    int x = static_cast<int>(i % 8);
    int y = static_cast<int>(i / 8);

    spriteSetup(square, x * SQUARE_SIZE_I, y * SQUARE_SIZE_I, colorMap[i]);
    window.draw(square);

    if (board.accessBoard(i) != 0) {
      char id = board.accessBoard(i);
      charToPiece.at(id).toScreen(window, x, y);
    }
  }
}

char detectSquare(int x, int y) {
  int a = x / SQUARE_SIZE_I;
  int b = y / SQUARE_SIZE_I;  // rounding to closest integer
  return static_cast<char>(a + b * 8);
}

void setColorMap(std::array<sf::Color, 64>& colorMap) {
  for (u32 i{0}; i < BOARD_SIZE; ++i) {
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
  if (colorMap[static_cast<u32>(index)] == COLOR_DARK) {
    colorMap[static_cast<u32>(index)] = COLOR_HIGHLIGHT_DARK;
  } else if (colorMap[static_cast<u32>(index)] == COLOR_LIGHT) {
    colorMap[static_cast<u32>(index)] = COLOR_HIGHLIGHT_LIGHT;
  }
}

void drawOptionWindow(sf::RenderWindow& window, sf::RectangleShape& square,
                      std::array<gfx::Piece, 8> list, bool side) {
  window.clear();
  for (int i{0}; i < 4; i++) {
    auto color = (i % 2) == 0 ? gfx::COLOR_LIGHT : gfx::COLOR_DARK;
    spriteSetup(square, i*gfx::SQUARE_SIZE_I, 0, color);
    window.draw(square);
  }

  for (int i{0}; i < 4; i++) {
    int offset = (!side) * 4;
    list[static_cast<u32>(i + offset)].toScreen(window, i, 0);
  }
}

}  // namespace gfx

#endif