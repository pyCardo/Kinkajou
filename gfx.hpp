#ifndef GFX_HPP
#define GFX_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>

#include "core.hpp"

namespace graphics {
auto const COLOR_LIGHT = sf::Color(255, 255, 229, 255);
auto const COLOR_DARK = sf::Color(173, 115, 29, 255);
auto const COLOR_HIGHLIGHT = sf::Color(238, 75, 43, 255);
float const PNG_SIZE = 2048;
float const WINDOW_DIMENSION = 700;
float const SQUARE_SIZE = WINDOW_DIMENSION / 8;
float const SCALE_FACTOR = SQUARE_SIZE / PNG_SIZE;
}  // namespace graphics

void showBoard(Board& board, sf::RenderWindow& window) {
  sf::Texture piece_gfx;
  int i{0};
  for (auto it = board.position.begin(); it != board.position.end(); ++it) {
    if (*it != 0) {
      std::string path{"pieces/" + std::string(1, *it) + ".png"};
      piece_gfx.loadFromFile(path);

      sf::Sprite piece_sprite;
      piece_sprite.setTexture(piece_gfx);
      piece_sprite.setScale(graphics::SCALE_FACTOR, graphics::SCALE_FACTOR);
      piece_sprite.setPosition(
          graphics::SQUARE_SIZE * static_cast<float>(i % 8),
          graphics::SQUARE_SIZE * static_cast<float>(i / 8));

      window.draw(piece_sprite);
    }
    ++i;
  }
}

char detect_square(int x, int y) {
  int a = x / graphics::SQUARE_SIZE;
  int b = y / graphics::SQUARE_SIZE;  // rounding to closest integer
  return static_cast<char>(a + b * 8);
}

void highlight_square() {}

/*Qui ci sarebbe da scegliere: preferiamo avere questa funzione come void e
 * passarle un rect già creato, oppure vogliamo che lei lo crei e lo
 * restituisca? La prima opzione sembra più efficiente nell'uso di memoria*/
void sprite_setup(sf::RectangleShape& rect, int x, int y, sf::Color color) {
  rect.setPosition(
      (x / static_cast<int>(graphics::SQUARE_SIZE)) * graphics::SQUARE_SIZE,
      (y / static_cast<int>(graphics::SQUARE_SIZE)) * graphics::SQUARE_SIZE);
  rect.setFillColor(color);
}

#endif