#ifndef CORE_HPP
#define CORE_HPP

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>

using u32 = unsigned long int;
using u64 = unsigned long long int;

namespace core {
struct Delta {  // standing for "change"
  int x;
  int y;
};

namespace offsets {
auto const rook{std::array<int, 4>{-8, -1, 1, 8}};
auto const bishop{std::array<int, 4>{-9, -7, 7, 9}};
auto const knight{std::array<Delta, 8>{Delta{-2, -1}, Delta{-1, -2},
                                       Delta{1, -2}, Delta{2, -1}, Delta{-2, 1},
                                       Delta{-1, 2}, Delta{1, 2}, Delta{2, 1}}};
auto const king{std::array<Delta, 8>{Delta{-1, -1}, Delta{0, -1}, Delta{1, -1},
                                     Delta{-1, 0}, Delta{1, 0}, Delta{-1, 1},
                                     Delta{0, 1}, Delta{1, 1}}};
auto const whitePawn{std::array<Delta, 4>{Delta{0, -1}, Delta{0, -2},
                                          Delta{-1, -1}, Delta{1, -1}}};
auto const blackPawn{
    std::array<Delta, 4>{Delta{0, 1}, Delta{0, 2}, Delta{-1, 1}, Delta{1, 1}}};
// white first, then black
}  // namespace offsets

struct Move {
  char current;
  char target;

  bool operator==(const Move m) {
    return current == m.current && target == m.target;
  }

  std::string algebraic() {
    char current_x{static_cast<char>(current % 8 + 97)};
    char current_y{static_cast<char>(-(current / 8) + 56)};
    char target_x{static_cast<char>(target % 8 + 97)};
    char target_y{static_cast<char>(-(target / 8) + 56)};
    // ASCII

    return std::string({current_x, current_y, target_x, target_y});
  }
};

// lascio questo enum perché potrebbe servire in seguito
// enum Piece_table { P, p, N, n, B, b, R, r, Q, q, K, k };

struct Board {
  std::array<char, 64> position{0};

  // std::array<char, 64> position{r, n, b, q, k, b, n, r,  //
  //                               p, p, p, p, p, p, p, p,  //
  //                               0, 0, 0, 0, 0, 0, 0, 0,  //
  //                               0, 0, 0, 0, 0, 0, 0, 0,  //
  //                               0, 0, 0, 0, 0, 0, 0, 0,  //
  //                               0, 0, 0, 0, 0, 0, 0, 0,  //
  //                               P, P, P, P, P, P, P, P,  //
  //                               R, N, B, Q, K, B, N, R};

  char whiteKing{60};
  char blackKing{4};
  std::array<bool, 2> whiteCastling{false, false};
  std::array<bool, 2> blackCastling{false, false};
  // to be read: left, right, according to player view

  bool whiteToMove{true};
  char enPassant{64};
  // is en passant available? initialized to 64 as it's not a valid index

  Board(const Board& copy) {
    position = copy.position;
    whiteKing = copy.whiteKing;
    blackKing = copy.blackKing;
    whiteCastling = copy.whiteCastling;
    blackCastling = copy.blackCastling;
    whiteToMove = copy.whiteToMove;
    enPassant = copy.enPassant;
  }  // copy constructor

  Board(const std::string& = std::string(
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -"));
  // FEN constructor, with default standard position

  char& accessBoard(u32);
  char& accessBoard(int);
  char accessBoard(u32) const;
  char accessBoard(int) const;
  // overloaded for conversion purposes

  void makeMove(Move);
};

bool isWhite(int, const Board&);

bool sameColor(int, int, const Board&);

bool oppositeColor(int, int, const Board&);

std::array<int, 2> checkLimits(int, int);

void movesInLimits(const Board&, std::vector<Move>&, const std::array<int, 2>&,
                   int, int);

void slidingLoop(const Board&, std::vector<Move>&, int,
                 const std::array<int, 4>&);

void castle(const Board&, std::vector<Move>&, int);

void nonSlidingLoop(const Board&, std::vector<Move>&, int,
                    const std::array<Delta, 8>&);

void pawnLoop(const Board&, std::vector<Move>&, int,
              const std::array<Delta, 4>&);

bool isCheck(Board&);

void generateMoves(Board&, std::vector<Move>&, int);

}  // namespace core

namespace test {

u64 perft(int, core::Board&);

}

#endif