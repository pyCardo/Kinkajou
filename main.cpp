#include <array>
#include <iostream>

struct BoardState {
  std::array<int, 64> board{-4, -2, -3, -5, -6, -3, -2, -4,  //
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

void showBoard(BoardState& board) {
  for (auto it = board.board.begin(); it != board.board.end(); ++it) {
    std::cout << *it << ' ';
    if ((it - board.board.begin() + 1) % 8 == 0) {
      std::cout << '\n';
    }
  }
}

int main() {
  BoardState board;
  makeMove(board, Move{0, 26});
  showBoard(board);
}