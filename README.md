# Kinkajou: Chess interface and engine

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
https://www.chessprogramming.org/Perft
https://www.chessprogramming.org/Perft_Results

## What's missing?
- Checkmate and stalemate banner/warning
- Draws by repetition, fifty-move rule, insufficient material
- New game interface
- Board flip option with F key
- Count legal moves at fixed depth and test with doctest.h
- FEN reader

## Issues
- ASan memory leaks
- Low quality pngs
- It's possible to castle even if in-between squares are attacked. Core::castle() (in movegen.cpp) should be updated to prevent castling if an intermidiate house on the king's journey is attacked. The only viable option seems to be similar to the legal move filter check.