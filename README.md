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

Set up debug mode and compile:
~~~bash
$ cmake -S . -B build/-Debug -DCMAKE_BUILD_TYPE=Debug
$ cmake --build build/-Debug
~~~

Set up release mode and compile:
~~~bash
$ cmake -S . -B build/-Release -DCMAKE_BUILD_TYPE=Release
$ cmake --build build/-Release
~~~

Manual compile (no warnings):
~~~bash
$ g++ main.cpp movegen.cpp -lsfml-graphics -lsfml-window -lsfml-system
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

## What's missing?
- Draws by repetition, fifty-move rule, insufficient material
- New game interface
- Board flip option with F key

## Issues
- ASan memory leaks
- Low quality pngs