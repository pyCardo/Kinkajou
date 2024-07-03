#include <cassert>
#include <iostream>

#include "core.hpp"

namespace core {

Board::Board(const std::string& fen) {
  std::array<char, 12> pieces = {'b', 'k', 'n', 'p', 'q', 'r',
                                 'B', 'K', 'N', 'P', 'Q', 'R'};
  std::array<char, 8> spaces = {'1', '2', '3', '4', '5', '6', '7', '8'};

  if (fen.empty()) {
    throw std::runtime_error("Empty string is not a valid fen");
  }

  // POSITION SETUP
  auto fenIt = fen.begin();
  auto posIt = position.begin();
  while (fenIt != fen.end() && *fenIt != ' ') {
    if (posIt == position.end()) {
      throw std::runtime_error("Exceeded position range");
    }
    if (*fenIt == '/') {
      ++fenIt;
      continue;
    } else if (std::find(pieces.begin(), pieces.end(), *fenIt) !=
               pieces.end()) {
      *posIt = *fenIt;
      ++posIt;
    } else if (std::find(spaces.begin(), spaces.end(), *fenIt) !=
               spaces.end()) {
      int numSpaces = *fenIt - '0';
      posIt += numSpaces;
    } else {
      throw std::runtime_error(std::string(1, *fenIt) +
                               " is not a valid fen character");
    }
    ++fenIt;
  }
  ++fenIt;

  // MOVING SIDE
  if (fenIt == fen.end()) {
    throw std::runtime_error("Incomplete fen is not a valid fen");
  }
  switch (*fenIt) {
    case 'w':
      whiteToMove = true;
      break;

    case 'b':
      whiteToMove = false;
      break;

    default:
      throw std::runtime_error(
          "Error in moving side");  // there must be a moving side
      break;
  }
  fenIt += 2;

  // CASTLING
  if (fenIt >= fen.end()) {
    throw std::runtime_error("Incomplete fen");
  }
  for (; fenIt != fen.end() && *fenIt != ' '; ++fenIt) {
    switch (*fenIt) {
      case '-':
        if (*(fenIt + 1) != ' ') {
          throw std::runtime_error("Castling information is incorrect");
        }
        break;

      case 'K':
        whiteCastling[1] = true;
        break;

      case 'Q':
        whiteCastling[0] = true;
        break;

      case 'k':
        blackCastling[0] = true;
        break;

      case 'q':
        blackCastling[1] = true;
        break;

      default:
        throw std::runtime_error("Castling information is missing");
        break;
    }
  }
  ++fenIt;

  // EN PASSANT
  if (fenIt == fen.end()) {
    throw std::runtime_error("Incomplete fen");
  }
  if (*fenIt != '-') {
    if (*fenIt >= 97 && *fenIt <= 104) {
      int x = static_cast<int>(*fenIt) - 97;
      fenIt++;
      if (*fenIt >= 49 && *fenIt <= 56) {
        int y = 7 - (static_cast<int>(*fenIt) - 49);
        enPassant = static_cast<char>(x + 8 * y);
      }
    } else {
      std::cout << *fenIt;
      throw std::runtime_error("En Passant information is ill-formed");
    }
  }

  // KING POSITIONS
  whiteKing = static_cast<char>(
      std::find(position.begin(), position.end(), 'K') - position.begin());
  blackKing = static_cast<char>(
      std::find(position.begin(), position.end(), 'k') - position.begin());
}  // FEN constructor

char& Board::accessBoard(u32 index) { return position[index]; }
char& Board::accessBoard(int index) {
  auto idx{static_cast<u32>(index)};
  return accessBoard(idx);
}
char Board::accessBoard(u32 index) const { return position[index]; }
char Board::accessBoard(int index) const {
  auto idx{static_cast<u32>(index)};
  return accessBoard(idx);
};

void Board::makeMove(Move move) {
  char& currentPiece{accessBoard(move.current)};
  char& targetPiece{accessBoard(move.target)};

  if (currentPiece == 'k') {
    blackKing = move.target;
    blackCastling = {false};
  }
  if (currentPiece == 'K') {
    whiteKing = move.target;
    whiteCastling = {false};
  }  // Updating king position

  // SPECIAL MOVES

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
        int neighbourSquare{static_cast<int>(move.target + step * i)};
        char neighbourPiece{accessBoard(neighbourSquare)};
        if ((neighbourPiece == 'r') || (neighbourPiece == 'R')) {
          accessBoard(move.target - step) = neighbourPiece;
          accessBoard(neighbourSquare) = 0;
          break;
        }
      }
    }
  }

  // En Passant
  {
    bool isPawn{currentPiece == 'p' || currentPiece == 'P'};

    // if selected move is en passant capture
    if (isPawn && move.target == enPassant) {
      int x{static_cast<int>(move.target) %
            8};  // x coordinate of the pawn that will be captured by en passant
      int y{static_cast<int>(move.current) /
            8};  // y coordinate of the pawn that will be captured by en passant
      accessBoard(x + y * 8) = 0;
    }
    enPassant = 64;  // en passant is no longer available

    // if selected move is double pawn push, enable en passant for next move
    if (isPawn) {
      if (std::abs(static_cast<int>(move.current - move.target)) == 16) {
        enPassant =
            static_cast<char>(static_cast<int>(move.current + move.target) / 2);
      }
    }
  }

  // Promotion
  if (move.promotion != 0) {
    if (whiteToMove) {
      targetPiece = static_cast<char>(toupper(move.promotion));
    } else {
      targetPiece = move.promotion;
    }
  } else {
    targetPiece = currentPiece;
  }  // WARNING! THIS MIGHT NOT BE CORRECT/COMPLETE

  currentPiece = 0;
  whiteToMove = !whiteToMove;
}

bool isWhite(int index, const Board& board) {
  int i{static_cast<int>(board.accessBoard(index))};
  return (i >= 65 && i <= 90);  // ASCII values for uppercase letters
}

bool sameColor(int indexA, int indexB, const Board& board) {
  return ((isWhite(indexA, board) == isWhite(indexB, board)) &&
          board.accessBoard(indexA) != 0 && board.accessBoard(indexB) != 0);
}

bool oppositeColor(int indexA, int indexB, const Board& board) {
  return ((isWhite(indexA, board) != isWhite(indexB, board)) &&
          board.accessBoard(indexA) != 0 && board.accessBoard(indexB) != 0);
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
  // already sorted
}

bool isCheck(Board& board) {
  const int king =
      static_cast<int>(board.whiteToMove ? board.blackKing : board.whiteKing);

  for (auto offset : core::offsets::rook) {
    const std::array<int, 2> limits{checkLimits(king, offset)};

    int enemy{king};
    while (enemy >= limits[0] && enemy <= limits[1]) {
      if (king == enemy) {
        enemy += offset;
        continue;
      }

      // break if target has the same color
      if (sameColor(king, enemy, board)) {
        break;
      }

      // break if target has different color
      if (oppositeColor(king, enemy, board)) {
        auto enemyPiece =
            static_cast<char>(std::tolower(board.accessBoard(enemy)));
        if (enemyPiece == 'r' || enemyPiece == 'q') {
          return true;
        }
        break;
      }

      enemy += offset;
    }
  }

  for (auto offset : core::offsets::bishop) {
    const std::array<int, 2> limits{checkLimits(king, offset)};

    int enemy{king};
    while (enemy >= limits[0] && enemy <= limits[1]) {
      if (king == enemy) {
        enemy += offset;
        continue;
      }

      // break if target has the same color
      if (sameColor(king, enemy, board)) {
        break;
      }

      // break if target has different color
      if (oppositeColor(king, enemy, board)) {
        auto enemyPiece =
            static_cast<char>(std::tolower(board.accessBoard(enemy)));
        if (enemyPiece == 'b' || enemyPiece == 'q') {
          return true;
        }
        break;
      }

      enemy += offset;
    }
  }

  for (auto offset : core::offsets::knight) {
    int x{king % 8};
    int y{king / 8};

    bool xLimit{0 <= x + offset.x && x + offset.x <= 7};
    bool yLimit{0 <= y + offset.y && y + offset.y <= 7};

    int enemy{(y + offset.y) * 8 + (x + offset.x)};
    if (xLimit && yLimit && oppositeColor(king, enemy, board)) {
      auto enemyPiece =
          static_cast<char>(std::tolower(board.accessBoard(enemy)));
      if (enemyPiece == 'n') {
        return true;
      }
    }
  }

  for (auto offset : core::offsets::king) {
    int x{king % 8};
    int y{king / 8};

    bool xLimit{0 <= x + offset.x && x + offset.x <= 7};
    bool yLimit{0 <= y + offset.y && y + offset.y <= 7};

    int enemy{(y + offset.y) * 8 + (x + offset.x)};
    if (xLimit && yLimit && oppositeColor(king, enemy, board)) {
      char enemyPiece =
          static_cast<char>(std::tolower(board.accessBoard(enemy)));
      if (enemyPiece == 'k') {
        return true;
      }
    }
  }

  const std::array<Delta, 4> pawnOffsets =
      board.whiteToMove ? core::offsets::blackPawn : core::offsets::whitePawn;

  for (u32 i{2}; i <= 3; ++i) {
    int x{king % 8};
    int y{king / 8};

    bool xLimit{0 <= x + pawnOffsets[i].x && x + pawnOffsets[i].x <= 7};
    bool yLimit{0 <= y + pawnOffsets[i].y && y + pawnOffsets[i].y <= 7};

    int enemy{(y + pawnOffsets[i].y) * 8 + (x + pawnOffsets[i].x)};
    if (xLimit && yLimit && oppositeColor(king, enemy, board)) {
      char enemyPiece =
          static_cast<char>(std::tolower(board.accessBoard(enemy)));
      if (enemyPiece == 'p') {
        return true;
      }
    }
  }

  return false;
}

void AddMove(const Board& board, std::vector<Move>& moves,
             Move& candidateMove) {
  Board pseudoBoard(board);
  pseudoBoard.makeMove(candidateMove);

  // PROMOTION
  int y{candidateMove.target / 8};
  bool isPawn{static_cast<char>(std::tolower(
                  board.accessBoard(candidateMove.current))) == 'p'};
  bool promotion{isPawn && (y == 0 || y == 7)};

  if (!isCheck(pseudoBoard)) {
    if (promotion) {
      for (auto c : offsets::promotionPieces) {
        candidateMove.promotion = c;
        moves.push_back(candidateMove);
      }
      return;  // pre-empting another push_back
    }
    moves.push_back(candidateMove);
  }
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

    // here we should verify if the king is in check, and only if not push_back
    Move candidateMove{static_cast<char>(currentSquare),
                       static_cast<char>(targetSquare)};

    AddMove(board, moves, candidateMove);

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

  // we can check wheter the king path is free by probing the moves vector for a
  // specific move; the following implementation uses a lambda function

  bool shortCastle{std::find_if(moves.begin(), moves.end(),
                                [currentSquare](const Move& move) {
                                  return move.target == currentSquare + 1;
                                }) != moves.end()};
  bool longCastle{std::find_if(moves.begin(), moves.end(),
                               [currentSquare](const Move& move) {
                                 return move.target == currentSquare - 1;
                               }) != moves.end()};

  // check all the in-between squares
  auto isEmpty = [&](int offset) {
    return board.accessBoard(currentSquare + offset) == 0;
  };
  bool freeLong{isEmpty(-1) && isEmpty(-2) && isEmpty(-3)};
  bool freeShort{isEmpty(1) && isEmpty(2)};

  if (white && currentSquare == 60) {
    if (board.whiteCastling[0] && longCastle && freeLong) {
      Move candidateMove{static_cast<char>(currentSquare),
                         static_cast<char>(currentSquare - 2)};
      AddMove(board, moves, candidateMove);
    }
    if (board.whiteCastling[1] && shortCastle && freeShort) {
      Move candidateMove{static_cast<char>(currentSquare),
                         static_cast<char>(currentSquare + 2)};
      AddMove(board, moves, candidateMove);
    }
  } else if (!white && currentSquare == 4) {
    if (board.blackCastling[0] && shortCastle && freeShort) {
      Move candidateMove{static_cast<char>(currentSquare),
                         static_cast<char>(currentSquare + 2)};
      AddMove(board, moves, candidateMove);
    }
    if (board.blackCastling[1] && longCastle && freeLong) {
      Move candidateMove{static_cast<char>(currentSquare),
                         static_cast<char>(currentSquare - 2)};
      AddMove(board, moves, candidateMove);
    }
  }
}

void nonSlidingLoop(const Board& board, std::vector<Move>& moves,
                    int currentSquare, const std::array<Delta, 8>& offsets) {
  int x{currentSquare % 8};
  int y{currentSquare / 8};

  for (auto offset : offsets) {
    bool xLimit{0 <= x + offset.x && x + offset.x <= 7};
    bool yLimit{0 <= y + offset.y && y + offset.y <= 7};

    char targetSquare{static_cast<char>((y + offset.y) * 8 + (x + offset.x))};
    if (xLimit && yLimit && !sameColor(currentSquare, targetSquare, board)) {
      Move candidateMove{static_cast<char>(currentSquare),
                         static_cast<char>(targetSquare)};

      AddMove(board, moves, candidateMove);
    }
  }

  if ((board.accessBoard(currentSquare) == 'k' && currentSquare == 4) ||
      (board.accessBoard(currentSquare) == 'K' && currentSquare == 60)) {
    Board pseudoboard(board);
    pseudoboard.whiteToMove = !pseudoboard.whiteToMove;
    if (!isCheck(pseudoboard)) {
      castle(board, moves, currentSquare);
    }  // not currently in check
  }
}

void pawnLoop(const Board& board, std::vector<Move>& moves, int currentSquare,
              const std::array<Delta, 4>& offsets) {
  int x{currentSquare % 8};
  int y{currentSquare / 8};

  bool amIWhite{isWhite(currentSquare, board)};
  bool atStart{(amIWhite && y == 6) || (!amIWhite && y == 1)};

  char advanceSquare{static_cast<char>((y + (offsets[0]).y) * 8 + x)};
  // access the square in front of the pawn
  bool canAdvance = static_cast<int>(board.accessBoard(advanceSquare) == 0);
  // is that square empty

  for (auto offset : offsets) {
    if (!atStart && std::abs(offset.y) == 2) {
      continue;
    }  // skip the double jump offset if the pawn is not on its starting
       // square

    bool xLimit{0 <= x + offset.x && x + offset.x <= 7};
    bool yLimit{0 <= y + offset.y &&
                y + offset.y <= 7};  // is the target square inside the board

    if (xLimit && yLimit) {
      // both sameColor and oppositeColor access the board array at the given
      // index, so this is to prevent them from accessing an invalid memory
      // address

      bool isCapturing{offset.x != 0};

      char targetSquare{static_cast<char>((y + offset.y) * 8 + (x + offset.x))};

      bool isSameColor{sameColor(currentSquare, targetSquare, board)};
      bool isOppositeColor{oppositeColor(currentSquare, targetSquare, board)};

      Move candidateMove{static_cast<char>(currentSquare),
                         static_cast<char>(targetSquare)};

      if (isCapturing && (isOppositeColor || targetSquare == board.enPassant)) {
        AddMove(board, moves, candidateMove);
      } else if (!isCapturing && canAdvance && !isSameColor &&
                 !isOppositeColor) {
        AddMove(board, moves, candidateMove);
      }
    }
  }
}

void generateMoves(Board& board, std::vector<Move>& moves, int currentSquare) {
  char symbol{board.accessBoard(currentSquare)};
  char currentPiece{static_cast<char>(
      std::tolower(symbol))}; /* std::tolower() returns an integer, which is
                               * converted to char; movegen is symmetrical for
                               * black and white */

  if ((currentPiece != symbol && board.whiteToMove) ||
      (currentPiece == symbol && !board.whiteToMove)) {
    // 0 is ignored anyway as there's no case inside
    // the switch
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
}

bool gameOver(Board& board) {
  // 0 means the game is not over, 1 means checkmate, 2 means stalemate
  std::vector<Move> possibleMoves;

  for (int i{0}; i < 64; i++) {
    generateMoves(board, possibleMoves, i);
  }

  if (possibleMoves.empty()) {
    return true;
  }

  return false;
}

}  // namespace core

namespace test {

u64 perft(int depth, core::Board& board) {
  u64 nodes{0};

  // generate all possible moves for the current board state
  for (int i = 0; i < 64; ++i) {
    if (board.accessBoard(i) != 0) {  // non-empty square
      std::vector<core::Move> moves;
      core::generateMoves(board, moves, i);

      // at depth 1, simply return the number of legal moves
      if (depth == 1) {
        nodes += static_cast<u64>(moves.size());
      } else {
        // recursively apply each move and count resulting positions
        for (const auto& move : moves) {
          core::Board pseudoBoard(board);
          pseudoBoard.makeMove(move);
          nodes += perft(depth - 1, pseudoBoard);
        }
      }
    }
  }

  return nodes;
}

}  // namespace test