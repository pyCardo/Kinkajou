#ifndef CORE_HPP
#define CORE_HPP

#include <array>
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

void Board::makeMove(Move move) {
  char& currentPiece{position[static_cast<unsigned long int>(move.current)]};
  char& targetPiece{position[static_cast<unsigned long int>(move.target)]};

  bool isPawn{currentPiece == 'p' || currentPiece == 'P'};

  if (isPawn && move.target == enPassant) {
    int x{static_cast<int>(move.target) % 8};
    int y{static_cast<int>(move.current) / 8};
    position[static_cast<unsigned long int>(x + y * 8)] = 0;
  }
  enPassant = 64;
  if (isPawn) {
    if (std::abs(static_cast<int>(move.current - move.target))) {
      enPassant =
          static_cast<char>(static_cast<int>(move.current + move.target) / 2);
    }
  }
  int y = move.target / 8;
  if (currentPiece == 'p' && y == 7) {
    targetPiece = 'q';
  } else if (currentPiece == 'P' && y == 0) {
    targetPiece = 'Q';
  } else {
    targetPiece = currentPiece;
  }
  currentPiece = 0;
}

bool isWhite(int index, const Board& board) {
  int i{
      static_cast<int>(board.position[static_cast<unsigned long int>(index)])};
  return (i >= 65 && i <= 90);  // ASCII values for uppercase letters
}

bool sameColor(int index_a, int index_b, const Board& board) {
  return ((isWhite(index_a, board) == isWhite(index_b, board)) &&
          board.position[static_cast<unsigned long int>(index_a)] != 0 &&
          board.position[static_cast<unsigned long int>(index_b)] != 0);
}

bool oppositeColor(int index_a, int index_b, const Board& board) {
  return ((isWhite(index_a, board) != isWhite(index_b, board)) &&
          board.position[static_cast<unsigned long int>(index_a)] != 0 &&
          board.position[static_cast<unsigned long int>(index_b)] != 0);
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
    if (sameColor(currentSquare, targetSquare, board)) {
      break;
    }

    moves.push_back(Move{static_cast<char>(currentSquare),
                         static_cast<char>(targetSquare)});

    // break if target has different color
    if (oppositeColor(currentSquare, targetSquare, board)) {
      break;
    }

    targetSquare += offset;
  }
}

void slidingLoop(const Board& board, std::vector<Move>& moves,
                 int currentSquare, const std::array<int, 4>& offsets) {
  for (auto offset : offsets) {
    const std::array<int, 2> limits{checkLimits(currentSquare, offset)};
    movesInLimits(board, moves, limits, currentSquare, offset);
  }
}

void nonSlidingLoop(const Board& board, std::vector<Move>& moves,
                    int currentSquare, const std::array<Location, 8>& offsets) {
  int x{currentSquare % 8};
  int y{currentSquare / 8};

  for (auto offset : offsets) {
    bool xLimit{0 <= x + offset.x && x + offset.x <= 7};
    bool yLimit{0 <= y + offset.y && y + offset.y <= 7};

    char targetSquare{static_cast<char>((y + offset.y) * 8 + (x + offset.x))};
    if (xLimit && yLimit && !sameColor(currentSquare, targetSquare, board)) {
      moves.push_back(Move{static_cast<char>(currentSquare), targetSquare});
    }
  }
}

void pawnLoop(Board& board, std::vector<Move>& moves, int currentSquare,
              const std::array<Location, 4>& offsets) {
  int x{currentSquare % 8};
  int y{currentSquare / 8};

  bool amIWhite{isWhite(currentSquare, board)};
  bool atStart{(amIWhite && y == 6) || (!amIWhite && y == 1)};

  char advanceSquare{static_cast<char>((y + (offsets[0]).y) * 8 + x)};
  // access the square in front of the pawn
  bool canAdvance = static_cast<int>(
      board.position[static_cast<unsigned long int>(advanceSquare)] == 0);
  // is that square empty

  for (auto offset : offsets) {
    if (!atStart && std::abs(offset.y) == 2) {
      continue;
    }  // skip the double jump offset if the pawn is not on its starting square

    bool xLimit{0 <= x + offset.x && x + offset.x <= 7};
    bool yLimit{0 <= y + offset.y &&
                y + offset.y <= 7};  // is the target square inside the board
    bool isCapturing{offset.x != 0};

    char targetSquare{static_cast<char>((y + offset.y) * 8 + (x + offset.x))};

    bool isSameColor{sameColor(currentSquare, targetSquare, board)};
    bool isOppositeColor{oppositeColor(currentSquare, targetSquare, board)};

    if (xLimit && yLimit) {
      if (isCapturing && (isOppositeColor || targetSquare == board.enPassant)) {
        moves.push_back(Move{static_cast<char>(currentSquare), targetSquare});
      } else if (!isCapturing && canAdvance && !isSameColor &&
                 !isOppositeColor) {
        moves.push_back(Move{static_cast<char>(currentSquare), targetSquare});
      }
    }
  }
}

void generateMoves(Board& board, std::vector<Move>& moves, int currentSquare) {
  char currentPiece{static_cast<char>(
      std::tolower(board.position[static_cast<unsigned long int>(
          currentSquare)]))}; /* std::tolower() returns an integer, which is
                               * converted to char; movegen is symmetrical for
                               * black and white */
  switch (currentPiece) {
    case 'p':
      if (isWhite(currentSquare, board)) {
        pawnLoop(board, moves, currentSquare, offsets::whitePawn);
      } else {
        pawnLoop(board, moves, currentSquare, offsets::blackPawn);
      }
      // modify board.enPassant if necessary
      break;

    case 'n':
      nonSlidingLoop(board, moves, currentSquare, offsets::knight);
      break;

    case 'b':
      slidingLoop(board, moves, currentSquare, offsets::bishop);
      break;

    case 'r':
      slidingLoop(board, moves, currentSquare, offsets::rook);
      break;

    case 'q':
      slidingLoop(board, moves, currentSquare, offsets::bishop);
      slidingLoop(board, moves, currentSquare, offsets::rook);
      break;

    case 'k':
      nonSlidingLoop(board, moves, currentSquare, offsets::king);
      break;

    default:
      break;
  }
}

}  // namespace core

#endif