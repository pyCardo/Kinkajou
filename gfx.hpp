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
int const PNG_SIZE = 2048;
int const WINDOW_DIMENSION = 700;
int const SQUARE_SIZE_I = WINDOW_DIMENSION / 8;
float const SQUARE_SIZE_F =
    WINDOW_DIMENSION /
    8.f;  // two different attributes to perform both integer and float division
float const SCALE_FACTOR = SQUARE_SIZE_F / PNG_SIZE;
}  // namespace graphics

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
    sprite.setScale(graphics::SCALE_FACTOR, graphics::SCALE_FACTOR);
  }
  void toScreen(sf::RenderWindow&, int, int);
};

void Piece::toScreen(sf::RenderWindow& window, int x, int y) {
  sprite.setPosition(graphics::SQUARE_SIZE_F * static_cast<float>(x),
                     graphics::SQUARE_SIZE_F * static_cast<float>(y));
  window.draw(sprite);
};

void showBoard(Board& board,
               sf::RenderWindow& window) {  // funzione molto pesante in runtime
  sf::Texture piece_gfx;
  {
    int i{0};
    for (auto it = board.position.begin(); it != board.position.end(); ++it) {
      if (*it != 0) {
        std::string path{"pieces/" + std::string(1, *it) + ".png"};
        piece_gfx.loadFromFile(path);

        sf::Sprite piece_sprite;
        piece_sprite.setTexture(piece_gfx);
        piece_sprite.setScale(graphics::SCALE_FACTOR, graphics::SCALE_FACTOR);
        piece_sprite.setPosition(
            graphics::SQUARE_SIZE_F * static_cast<float>(i % 8),
            graphics::SQUARE_SIZE_F * static_cast<float>(i / 8));

        window.draw(piece_sprite);
      }
      ++i;
    }
  }
}

char detect_square(int x, int y) {
  int a = x / graphics::SQUARE_SIZE_I;
  int b = y / graphics::SQUARE_SIZE_I;  // rounding to closest integer
  return static_cast<char>(a + b * 8);
}

/*Qui ci sarebbe da scegliere: preferiamo avere questa funzione come void e
 * passarle un rect già creato, oppure vogliamo che lei lo crei e lo
 * restituisca? La prima opzione sembra più efficiente nell'uso di memoria*/
void sprite_setup(sf::RectangleShape& rect, int x, int y, sf::Color color) {
  rect.setPosition(
      static_cast<float>(x / graphics::SQUARE_SIZE_I) * graphics::SQUARE_SIZE_F,
      static_cast<float>(y / graphics::SQUARE_SIZE_I) *
          graphics::SQUARE_SIZE_F);
  rect.setFillColor(color);
}

#endif