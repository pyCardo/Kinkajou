# Kinkajou: Chess interface and engine

## Castling rules

The right to castle is **permanently** lost:
1. On either side if the king has moved (*at any point in the game*).
2. On one side if the respective rook has been moved/captured (*at any point in the game*).

Castling is prevented **temporarily**:
1. If there is any piece between the king and the rook with which the castling is to be effected.
2. If either the square on which the king stands, or the square which it must cross, or the square which it is to occupy, is attacked by one or more pieces.

Note that the rook _can_ castle while under attack.

## Configuration

### Cmake

Set up debug mode with:
~~~bash
$ cmake -S . -B build/-Debug -DCMAKE_BUILD_TYPE=Debug
~~~

Compile with:
~~~bash
$ cmake --build build/-Debug
~~~

Manual compile (no warnings):
~~~bash
$ g++ main.cpp -lsfml-graphics -lsfml-window -lsfml-system
~~~

### .gitignore

Git will ignore the following folders/files:
- `.vscode/`
- `build/`
- `*.out` and `*.exe`

## Tests
Testing related resources:\
https://www.chessprogramming.org/Perft \
https://www.chessprogramming.org/Perft_Results \
http://bernd.bplaced.net/fengenerator/fengenerator.html

### FEN Board setup
- No control over how many ranks are in the first part of the FEN (position might be incomplete)

#### Positions
- 2n1q3/r1B5/1pB3p1/3K4/2P2P2/r1k4p/5p1R/2N5 w - - 0 1

## What's missing?
- Checkmate and stalemate banner/warning
- Promotion to knight/bishop/rook
- Draws by repetition, fifty-move rule, insufficient material
- New game interface
- Board flip option with F key
- Count legal moves at fixed depth and test with doctest.h
- FEN reader

## Issues
- ASan memory leaks
- Low quality pngs
- It's possible to castle even if in-between squares are attacked. Core::castle() (in movegen.cpp) should be updated to prevent castling if an intermidiate house on the king's journey is attacked. The only viable option seems to be similar to the legal move filter check.
- Castling when in check is an illegal move, and should not be allowed (fix required).