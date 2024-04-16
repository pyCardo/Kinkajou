#ifndef CORE_HPP
#define CORE_HPP

#include <array>

struct Move {
  char current;
  char target;
};
struct Board {
  std::array<char, 64> position {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r',  //
                             'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p',  //
                             0,   0,   0,   0,   0,   0,   0,   0,    //
                             0,   0,   0,   0,   0,   0,   0,   0,    //
                             0,   0,   0,   0,   0,   0,   0,   0,    //
                             0,   0,   0,   0,   0,   0,   0,   0,    //
                             'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',  //
                             'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'};

  std::array<bool, 2> white_can_castle{true, true};
  std::array<bool, 2> black_can_castle{true, true};
  void makeMove(Move);
};

void Board::makeMove(Move move) {
  Board::position[static_cast<unsigned long int>(move.target)] = Board::position[static_cast<unsigned long int>(move.current)]; 
  Board::position[static_cast<unsigned long int>(move.current)] = 0;
}

#endif