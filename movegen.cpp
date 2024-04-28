#include <iostream>

#include "core.hpp"

namespace core {

void Board::makeMove(Move move) {
  char& currentPiece{position[static_cast<unsigned long int>(move.current)]};
  char& targetPiece{position[static_cast<unsigned long int>(move.target)]};

  //  SPECIAL MOVES

  // En Passant
  {
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
  }

  // Promotion
  {
    int y = move.target / 8;
    if (currentPiece == 'p' && y == 7) {
      targetPiece = 'q';
      currentPiece = 0;
      return;
    } else if (currentPiece == 'P' && y == 0) {
      targetPiece = 'Q';
      currentPiece = 0;
      return;
    }
  }

  // Castling
  {
    // update castling rights
    switch (move.current) {
      // moving a Rook
      case 0:
        blackCastling[1] = false;
        break;

      case 7:
        blackCastling[0] = false;
        break;

      case 56:
        whiteCastling[0] = false;
        break;

      case 63:
        whiteCastling[1] = false;
        break;

      // moving a King
      case 4:
        blackCastling[0] = false;
        blackCastling[1] = false;
        break;

      case 60:
        whiteCastling[0] = false;
        whiteCastling[1] = false;
        break;

      default:
        break;
    }

    switch (move.target) {
      // a Rook is captured
      case 0:
        blackCastling[1] = false;
        break;

      case 7:
        blackCastling[0] = false;
        break;

      case 56:
        whiteCastling[0] = false;
        break;

      case 63:
        whiteCastling[1] = false;
        break;

      default:
        break;
    }

    // the selected move is castling

    bool isKing{currentPiece == 'k' || currentPiece == 'K'};
    bool isCastling{isKing && (std::abs(move.target - move.current) == 2)};

    if (isCastling) {
      // find rook
      char step{
          static_cast<char>(static_cast<int>(move.target - move.current) / 2)};

      for (int i{1}; i <= 2; i++) {
        unsigned long int neighbourSquare{
            static_cast<unsigned long int>(move.target + step * i)};
        char neighbourPiece{position[neighbourSquare]};
        if ((neighbourPiece == 'r') || (neighbourPiece == 'R')) {
          position[static_cast<unsigned long int>(move.target - step)] =
              neighbourPiece;
          position[neighbourSquare] = 0;
          break;
        }
      }

      targetPiece = currentPiece;
      currentPiece = 0;
      return;
    }
  }

  // if no special moves occured
  targetPiece = currentPiece;
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

void castle(const Board& board, std::vector<Move>& moves, int currentSquare) {
  bool white{isWhite(currentSquare, board)};

  if (white) {
    if (board.whiteCastling[0]) {
      bool freeRank{
          board.position[static_cast<unsigned long int>(currentSquare - 1)] ==
              0 &&
          board.position[static_cast<unsigned long int>(currentSquare - 2)] ==
              0};
      if (freeRank) {
        moves.push_back(Move{static_cast<char>(currentSquare),
                             static_cast<char>(currentSquare - 2)});
      }
    }
    if (board.whiteCastling[1]) {
      bool freeRank{
          board.position[static_cast<unsigned long int>(currentSquare + 1)] ==
              0 &&
          board.position[static_cast<unsigned long int>(currentSquare + 2)] ==
              0};
      if (freeRank) {
        moves.push_back(Move{static_cast<char>(currentSquare),
                             static_cast<char>(currentSquare + 2)});
      }
    }
  }

  if (!white) {
    if (board.blackCastling[0]) {
      bool freeRank{
          board.position[static_cast<unsigned long int>(currentSquare + 1)] ==
              0 &&
          board.position[static_cast<unsigned long int>(currentSquare + 2)] ==
              0};
      if (freeRank) {
        moves.push_back(Move{static_cast<char>(currentSquare),
                             static_cast<char>(currentSquare + 2)});
      }
    }
    if (board.blackCastling[1]) {
      bool freeRank{
          board.position[static_cast<unsigned long int>(currentSquare - 1)] ==
              0 &&
          board.position[static_cast<unsigned long int>(currentSquare - 2)] ==
              0};
      if (freeRank) {
        moves.push_back(Move{static_cast<char>(currentSquare),
                             static_cast<char>(currentSquare - 2)});
      }
    }
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

  if (std::tolower(
          board.position[static_cast<unsigned long int>(currentSquare)]) ==
      'k') {
    castle(board, moves, currentSquare);
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