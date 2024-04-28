#ifndef CORE_HPP
#define CORE_HPP

#include <array>
#include <cctype>
#include <cmath>
#include <vector>

namespace core {
struct Location {
  int x;
  int y;
};

namespace offsets {
auto const rook{std::array<int, 4>{-8, -1, 1, 8}};
auto const bishop{std::array<int, 4>{-9, -7, 7, 9}};
auto const knight{std::array<Location, 8>{
    Location{-2, -1}, Location{-1, -2}, Location{1, -2}, Location{2, -1},
    Location{-2, 1}, Location{-1, 2}, Location{1, 2}, Location{2, 1}}};
auto const king{std::array<Location, 8>{
    Location{-1, -1}, Location{0, -1}, Location{1, -1}, Location{-1, 0},
    Location{1, 0}, Location{-1, 1}, Location{0, 1}, Location{1, 1}}};
auto const whitePawn{std::array<Location, 4>{
    Location{0, -1}, Location{0, -2}, Location{-1, -1}, Location{1, -1}}};
auto const blackPawn{std::array<Location, 4>{Location{0, 1}, Location{0, 2},
                                             Location{-1, 1}, Location{1, 1}}};
// white first, then black
}  // namespace offsets

struct Move {
  char current;
  char target;

  bool operator==(const Move m) {
    return current == m.current && target == m.target;
  }
};

// lascio questo enum perché potrebbe servire in seguito
// enum Piece_table { P, p, N, n, B, b, R, r, Q, q, K, k };

struct Board {
  std::array<char, 64> position{'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r',  //
                                'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p',  //
                                0,   0,   0,   0,   0,   0,   0,   0,    //
                                0,   0,   0,   0,   0,   0,   0,   0,    //
                                0,   0,   0,   0,   0,   0,   0,   0,    //
                                0,   0,   0,   0,   0,   0,   0,   0,    //
                                'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',  //
                                'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'};

  // std::array<char, 64> position{r, n, b, q, k, b, n, r,  //
  //                               p, p, p, p, p, p, p, p,  //
  //                               0, 0, 0, 0, 0, 0, 0, 0,  //
  //                               0, 0, 0, 0, 0, 0, 0, 0,  //
  //                               0, 0, 0, 0, 0, 0, 0, 0,  //
  //                               0, 0, 0, 0, 0, 0, 0, 0,  //
  //                               P, P, P, P, P, P, P, P,  //
  //                               R, N, B, Q, K, B, N, R};

  std::array<bool, 2> whiteCastling{true, true};
  std::array<bool, 2> blackCastling{true, true};
  char enPassant{64};
  // is en passant available? initialized to 64 because it's not a valid index

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

void nonSlidingLoop(const Board&, std::vector<Move>&, int,
                    const std::array<Location, 8>&);

void pawnLoop(Board&, std::vector<Move>&, int, const std::array<Location, 4>&);

void generateMoves(Board&, std::vector<Move>&, int);

}  // namespace core

#endif