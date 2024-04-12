#ifndef GFX_HPP
#define GFX_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>

#include "core.hpp"

namespace graphics {
auto const COLOR_LIGHT = sf::Color(255, 255, 229, 255);
auto const COLOR_DARK = sf::Color(173, 115, 29, 255);
float const PNG_SIZE = 2048;
float const WINDOW_DIMENSION{700};
float SQUARE_SIZE = WINDOW_DIMENSION / 8;
}  // namespace graphics

void showBoard(Board& board, sf::RenderWindow& window) {
  const float scale_factor{
      graphics::SQUARE_SIZE /
      graphics::PNG_SIZE};  // non dovrebbe essere dichiarato ad ogni
                            // iterazione
  sf::Texture piece_gfx;
  int i{0};
  for (auto it = board.board.begin(); it != board.board.end(); ++it) {
    if (*it != 0) {
      std::string path{"pieces/" + std::string(1, *it) + ".png"};
      piece_gfx.loadFromFile(path);

      sf::Sprite piece_sprite;
      piece_sprite.setTexture(piece_gfx);
      piece_sprite.setScale(scale_factor, scale_factor);
      piece_sprite.setPosition(graphics::SQUARE_SIZE * static_cast<float>(i % 8),
                               graphics::SQUARE_SIZE * static_cast<float>(i / 8));

      window.draw(piece_sprite);
    }
    ++i;
  }
}

#endif