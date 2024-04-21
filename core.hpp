#ifndef CORE_HPP
#define CORE_HPP

#include <array>
#include <vector>

namespace core {
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
                                0,   'p', 'p', 'p', 0,   'p', 'p', 'p',  //
                                0,   0,   0,   0,   0,   0,   0,   0,    //
                                0,   0,   0,   0,   0,   0,   0,   0,    //
                                0,   0,   0,   0,   0,   0,   0,   0,    //
                                0,   0,   0,   0,   0,   0,   0,   0,    //
                                'P', 'P', 'P', 'P', 0,   'P', 'P', 0,    //
                                'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'};

  // std::array<char, 64> position{r, n, b, q, k, b, n, r,  //
  //                               p, p, p, p, p, p, p, p,  //
  //                               0, 0, 0, 0, 0, 0, 0, 0,  //
  //                               0, 0, 0, 0, 0, 0, 0, 0,  //
  //                               0, 0, 0, 0, 0, 0, 0, 0,  //
  //                               0, 0, 0, 0, 0, 0, 0, 0,  //
  //                               P, P, P, P, P, P, P, P,  //
  //                               R, N, B, Q, K, B, N, R};

  std::array<bool, 2> white_can_castle{true, true};
  std::array<bool, 2> black_can_castle{true, true};
  void makeMove(Move);
};

void Board::makeMove(Move move) {
  Board::position[static_cast<unsigned long int>(move.target)] =
      Board::position[static_cast<unsigned long int>(move.current)];
  Board::position[static_cast<unsigned long int>(move.current)] = 0;
}

bool isWhite(char c) {
  int i{static_cast<int>(c)};
  return (i >= 65 && i <= 90);
}

bool sameColor(char a, char b) {
  return ((isWhite(a) == isWhite(b)) && a != 0 && b != 0);
}

bool oppositeColor(
    char a,
    char b) {  // necessary because !sameColor() doesn't consider empty squares
  return ((isWhite(a) != isWhite(b)) && a != 0 && b != 0);
}

std::array<int, 2> RookLimits(int currentSquare, int offset) {
  const std::array<int, 4> limits{0, currentSquare / 8 * 8,
                                  currentSquare / 8 * 8 + 7, 63};

  int infLimit{0};
  int supLimit{63};

  if (std::abs(offset) == 8) {
    infLimit = limits[0];
    supLimit = limits[3];
  } else if (std::abs(offset) == 1) {
    infLimit = limits[1];
    supLimit = limits[2];
  }

  return std::array<int, 2>{infLimit, supLimit};
}

std::array<int, 2> BishopLimits(int currentSquare, int offset) {
  const std::array<int, 4> limits{
      0, currentSquare + (currentSquare % 8 * (offset)),
      currentSquare + ((7 - currentSquare % 8) * (offset)),
      63};  // limits[1] is the east limit while moving diagonally,
            // limit[2] is the west one

  int infLimit{0};
  int supLimit{63};

  switch (offset) {
    case -9:
      infLimit = std::max(infLimit, limits[1]);
      break;

    case -7:
      infLimit = std::max(infLimit, limits[2]);
      break;

    case 7:
      supLimit = std::min(supLimit, limits[1]);
      break;

    case 9:
      supLimit = std::min(supLimit, limits[2]);
      break;

    default:
      break;
  }
  return std::array<int, 2>{infLimit, supLimit};
}

void MovesInLimits(const Board& board, std::vector<Move>& moves,
                   int currentSquare, int offset, int infLimit, int supLimit) {
  int targetSquare{currentSquare};
  while (targetSquare >= infLimit && targetSquare <= supLimit) {
    if (currentSquare == targetSquare) {
      targetSquare += offset;
      continue;
    }

    // break if target has the same color
    if (sameColor(
            board.position[static_cast<unsigned long int>(currentSquare)],
            board.position[static_cast<long unsigned int>(targetSquare)])) {
      break;
    }

    moves.push_back(Move{static_cast<char>(currentSquare),
                         static_cast<char>(targetSquare)});

    // break if target has different color
    if (oppositeColor(
            board.position[static_cast<unsigned long int>(currentSquare)],
            board.position[static_cast<long unsigned int>(targetSquare)])) {
      break;
    }

    targetSquare += offset;
  }
}

void GenerateMoves(const Board& board, std::vector<Move>& moves,
                   int currentSquare) {
  switch (static_cast<char>(
      std::tolower(board.position[static_cast<unsigned long int>(
          currentSquare)]))) {  // std::lower() returns an integer,
                                // that should then be converted to
                                // char

    case 0:
      break;

    case 'p':
      break;

    case 'n':
      break;

    case 'b': {  // we create a scope to declare offsets case by case

      const std::array<int, 4> offsets{-9, -7, 7, 9};

      for (auto offset_it{offsets.begin()}; offset_it < offsets.end();
           ++offset_it) {
        const std::array<int, 2> limits{
            BishopLimits(currentSquare, *offset_it)};

        MovesInLimits(board, moves, currentSquare, *offset_it, limits[0],
                      limits[1]);
      }
    } break;

    case 'r': {  // we create a scope to declare offsets case by case

      const std::array<int, 4> offsets{-8, -1, 1, 8};

      for (auto offset_it{offsets.begin()}; offset_it < offsets.end();
           ++offset_it) {
        const std::array<int, 2> limits{RookLimits(currentSquare, *offset_it)};

        MovesInLimits(board, moves, currentSquare, *offset_it, limits[0],
                      limits[1]);
      }
    }

    break;

    case 'q':{
      std::array<int, 4> offsets{-8, -1, 1, 8};

      for (auto offset_it{offsets.begin()}; offset_it < offsets.end();
           ++offset_it) {
        const std::array<int, 2> limits{RookLimits(currentSquare, *offset_it)};

        MovesInLimits(board, moves, currentSquare, *offset_it, limits[0],
                      limits[1]);
      }

      offsets = {-9, -7, 7, 9};

      for (auto offset_it{offsets.begin()}; offset_it < offsets.end();
           ++offset_it) {
        const std::array<int, 2> limits{
            BishopLimits(currentSquare, *offset_it)};

        MovesInLimits(board, moves, currentSquare, *offset_it, limits[0],
                      limits[1]);
      }
    }
      break;

    case 'k':
      break;

    default:
      break;
  }
}

}  // namespace core

#endif