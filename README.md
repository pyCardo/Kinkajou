# Kinkajou: Chess interface and engine

## Configuration

### Cmake

Set up debug mode with:
~~~bash
$ cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug
~~~

Compile with:
~~~bash
$ cmake --build build/debug
~~~

Manual compile:
~~~bash
$ g++ main.cpp -Wall -Wextra -lsfml-graphics -lsfml-window -lsfml-system
~~~

### .gitignore

Git will ignore the following folders/files:
- `build/`

## Issues
- AddressSanitizer preventing compilation with Cmake
- Low quality pngs