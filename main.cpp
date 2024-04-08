#include <array>
#include <iostream>

struct BoardState {
  std::array<char, 64> board{-4, -2, -3, -5, -6, -3, -2, -4,  //
                             -1, -1, -1, -1, -1, -1, -1, -1,  //
                             0,  0,  0,  0,  0,  0,  0,  0,   //
                             0,  0,  0,  0,  0,  0,  0,  0,   //
                             0,  0,  0,  0,  0,  0,  0,  0,   //
                             0,  0,  0,  0,  0,  0,  0,  0,   //
                             1,  1,  1,  1,  1,  1,  1,  1,   //
                             4,  2,  3,  5,  6,  3,  2,  4};

  std::array<bool, 2> white_can_castle{true, true};
  std::array<bool, 2> black_can_castle{true, true};
};

int main() { BoardState board; }