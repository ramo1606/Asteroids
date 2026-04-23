# Copilot instructions for Asteroids

## Build commands

### Windows development build

Use the repository script:

```bat
build-windows.bat
```

Equivalent manual commands from `README.md`:

```bat
mkdir build-windows
cd build-windows
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

Output: `build-windows/Release/Asteroids.exe` (the batch file text still mentions the older `RG35XX_Game` name, but the CMake target is `Asteroids`).

### RG35XX cross-build

Use the repository script:

```bat
build-rg35xx.bat
```

Equivalent manual commands from the repo:

```bat
mkdir build-rg35xx
cd build-rg35xx
cmake .. -G "NMake Makefiles" ^
  -DCMAKE_TOOLCHAIN_FILE=../rg35xx-toolchain.cmake ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DBUILD_FOR_RG35XX=ON
nmake
```

Output: `build-rg35xx/rg35xx/Asteroids`

## Tests and lint

This repository does not define automated tests, `ctest` targets, or lint commands. There is no single-test command today.

## High-level architecture

- `CMakeLists.txt` builds a single executable target, `Asteroids`, and fetches raylib 5.5 with `FetchContent`.
- The build has two runtime configurations:
  - desktop development: raylib desktop platform with OpenGL 3.3
  - RG35XX: SDL platform with OpenGL ES 3.0, linking against libraries from `rg35xx-sysroot/usr/lib` via `rg35xx-toolchain.cmake`
- `src/main.c` is the whole playable game right now. It contains:
  - local entity structs (`Player`, `Bullet`)
  - atlas helpers (`GetSpriteRect`, `DrawSprite`)
  - entity update/draw helpers
  - the main raylib loop
- `include/config.h` is the gameplay/configuration hub. Screen size, frame rate, asset paths, sprite IDs, and tuning constants all live there.
- `include/Asteroids.h` is generated atlas metadata from rTexPacker. Rendering code reads `rtpDescAsteroids[]` instead of hardcoding sprite rectangles.
- `assets/Asteroids.png` is the main texture atlas. CMake copies the whole `assets/` directory into the build output after each build, so runtime asset paths stay relative to the executable.
- `Tutoriales/01`, `02`, and `03` are not random notes; they document the intended progression of the current codebase. The top-level `src/main.c` and `include/config.h` currently correspond to Lesson 03 ("Shooting").

## Key conventions

- Keep the game tutorial-shaped. New systems are added incrementally to the existing single-file gameplay loop instead of being split into many modules early.
- Put all tunable constants and sprite index macros in `include/config.h`. Game logic should not introduce raw sprite indices or other magic numbers.
- Treat `include/Asteroids.h` as generated source-of-truth atlas data. If you need a sprite in gameplay code, add a named `SPR_*` macro in `config.h` that maps to the correct `rtpDescAsteroids[]` index.
- Use the atlas helper path already established in `main.c`: `GetSpriteRect()` handles trimmed vs. untrimmed atlas entries, and `DrawSprite()` is the common draw path for rotated sprites.
- Prefer fixed-size pools for short-lived entities. Bullets use a stack-allocated array plus an `active` flag rather than heap allocation.
- When reinitializing pooled entities, follow the existing C99 style: overwrite the full struct with a compound literal and designated initializers.
- Preserve draw-order intent when adding visuals. The current frame order is background, bullets, then player so bullets appear to emerge from under the ship.

## Workflow
- Explain plan before start coding.
- Do short meaningful commits.
- Ask about the commit or push before.
- Do not use co-authored-by in commit messages.
- Check for Raylib api and examples if in doubt.
- Keep C99 code standards.