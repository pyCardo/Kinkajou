#ifndef CORE_HPP
#define CORE_HPP

#include <array>

struct Board {
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

void makeMove(Board& board, Move move) {
  auto start = board.board.begin() + move.start_square;
  auto land = board.board.begin() + move.land_square;

  *land = *start;
  *start = 0;
}

#endif