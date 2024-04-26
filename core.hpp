#ifndef CORE_HPP
#define CORE_HPP

#include <array>
#include <vector>

namespace core {
namespace offsets {
auto const rook{std::array<int, 4>{-8, -1, 1, 8}};
auto const bishop{std::array<int, 4>{-9, -7, 7, 9}};
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
  position[static_cast<unsigned long int>(move.target)] =
      position[static_cast<unsigned long int>(move.current)];
  position[static_cast<unsigned long int>(move.current)] = 0;
}

bool isWhite(char c) {
  int i{static_cast<int>(c)};
  return (i >= 65 && i <= 90);  // ASCII values for uppercase letters
}

bool sameColor(char a, char b) {
  return ((isWhite(a) == isWhite(b)) && a != 0 && b != 0);
}

bool oppositeColor(char a, char b) {
  return ((isWhite(a) != isWhite(b)) && a != 0 && b != 0);
}  // necessary because !sameColor() doesn't consider empty squares

std::array<int, 2> checkLimits(int currentSquare, int offset) {
  const std::array<int, 6> limits{
      0,                                               // top left [0]
      currentSquare / 8 * 8,                           // lateral negative [1]
      currentSquare / 8 * 8 + 7,                       // lateral positive [2]
      currentSquare + (currentSquare % 8 * (offset)),  // diagonal negative [3]
      currentSquare +
          ((7 - currentSquare % 8) * (offset)),  // diagonal positive [4]
      63                                         // bottom right [5]
  };

  int infLimit{0};
  int supLimit{63};

  switch (offset) {
    case -9:
      infLimit = std::max(infLimit, limits[3]);
      break;

    case -8:
      [[fallthrough]];
    case 8:
      infLimit = limits[0];
      supLimit = limits[5];
      break;

    case -7:
      infLimit = std::max(infLimit, limits[4]);
      break;

    case -1:
      [[fallthrough]];
    case 1:
      infLimit = limits[1];
      supLimit = limits[2];
      break;

    case 7:
      supLimit = std::min(supLimit, limits[3]);
      break;

    case 9:
      supLimit = std::min(supLimit, limits[4]);
      break;

    default:
      break;
  }

  return std::array<int, 2>{infLimit, supLimit};
}

void movesInLimits(const Board& board, std::vector<Move>& moves,
                   const std::array<int, 2>& limits, int currentSquare,
                   int offset) {
  int targetSquare{currentSquare};
  while (targetSquare >= limits[0] && targetSquare <= limits[1]) {
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

void generateMoves(const Board& board, std::vector<Move>& moves,
                   int currentSquare) {
  switch (static_cast<char>(std::tolower(
      board.position[static_cast<unsigned long int>(currentSquare)]))) {
      /* std::tolower() returns an integer, which is converted to char; movegen
       * is symmetrical for black and white */

    case 'p':
      break;

    case 'n':
      break;

    case 'b': {
      for (auto offset : offsets::bishop) {
        const std::array<int, 2> limits{checkLimits(currentSquare, offset)};
        movesInLimits(board, moves, limits, currentSquare, offset);
      }
    } break;

    case 'r': {
      for (auto offset : offsets::rook) {
        const std::array<int, 2> limits{checkLimits(currentSquare, offset)};
        movesInLimits(board, moves, limits, currentSquare, offset);
      }
    } break;

    case 'q': {
      for (auto offset : offsets::rook) {
        const std::array<int, 2> limits{checkLimits(currentSquare, offset)};
        movesInLimits(board, moves, limits, currentSquare, offset);
      }

      for (auto offset : offsets::bishop) {
        const std::array<int, 2> limits{checkLimits(currentSquare, offset)};
        movesInLimits(board, moves, limits, currentSquare, offset);
      }
    } break;

    case 'k':
      break;

    default:
      break;
  }
}

}  // namespace core

#endif