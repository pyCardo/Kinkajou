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

## Issues
- ASan memory leaks
- Low quality pngs
- Not a "real" issue, but so far it's possible to castle even if in-between squares are attacked. It will be partly solved once the legal moves filter is implemented, removing the possibility to castle if the king's landing house is attacked. Still, core::castle() (in movegen.cpp) should be updated to prevent castling if an intermidiate house on the king's journey is attacked. The only viable seems to be similar to the legal move filter check. 