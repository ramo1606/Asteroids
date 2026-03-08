# Lesson 01: Window & Game Loop

## What We're Building

A window with a tiled starfield background. No gameplay yet — just the foundation that every future lesson builds on: the game loop, the texture atlas loaded into memory, and sprites drawn to the screen.

By the end of this lesson you'll have a running program showing a seamless space background filling the entire window.

## Concepts

### The Game Loop

Every game runs the same core loop:

```
while (game is running) {
    1. Process input
    2. Update game state
    3. Draw everything
}
```

That's it. Input → Update → Draw, 60 times per second. The entire game — physics, AI, animations, UI — lives inside this loop. Raylib gives us the scaffolding (`BeginDrawing`/`EndDrawing`, `SetTargetFPS`), but the structure is ours.

In this lesson, Update is empty and Draw only tiles the background. But the skeleton is there, and we'll fill it in over the coming lessons.

### Texture Atlases

A texture atlas (also called a spritesheet) is a single image containing all the game's sprites packed together. Instead of loading 50 individual files, you load one image and draw pieces of it using source rectangles.

This isn't just a convenience — it's a performance pattern. Every time the GPU switches textures, it costs time. With an atlas, you bind one texture at the start of the frame and draw everything from it. For a 2D game like ours, this means virtually zero texture-switching overhead.

Our atlas (`Asteroids.png`) was packed with **rTexPacker**, which also generated `Asteroids.h` — a header file containing the exact position, size, and trim data for every sprite. We never hardcode pixel coordinates.

### Sprite Trimming

Some sprites in our atlas are **trimmed**. rTexPacker detected transparent pixels around the edges and packed only the visible portion to save space. Each sprite entry tracks:

- `sourceWidth/Height` — the original logical size of the sprite
- `trimRecX/Y/Width/Height` — where the actual pixels are within that logical size
- `trimmed` — whether trimming was applied

For drawing, we need a helper that gives us the correct source rectangle. If a sprite is trimmed, the source rect in the atlas is offset by the trim position. If not, it's just the sprite's position and size directly.

The background sprites aren't trimmed (they fill their entire area), so for this lesson it's straightforward. But we build the helper now because we'll need it for everything else.

## Why This? / Alternatives

### Why CMake?

CMake is the de facto standard for C/C++ projects. It's not the friendliest tool, but it's what you'll encounter in the real world. It handles cross-platform builds, dependency fetching (we use `FetchContent` to grab Raylib if it's not installed), and asset copying.

**Alternatives:**
- **Plain Makefile** — simpler for small projects, but breaks on cross-platform. Fine for Linux-only.
- **Meson** — cleaner syntax than CMake, but less ecosystem support.
- **Just compile manually** — `gcc main.c -lraylib -lm -o asteroids` works until your project grows. No asset handling, no dependency management.

### Why `SetTargetFPS(60)` instead of a fixed timestep?

Raylib's `SetTargetFPS` caps the frame rate and handles frame timing internally. It's simple and sufficient for a game like ours. A proper fixed timestep (accumulating delta time, running physics at a fixed rate) matters for networked games, complex physics, or deterministic replays. We don't need that here.

### Why `ClearBackground(BLACK)`?

Our background tiles cover the entire screen, so you'd think clearing is unnecessary. But if the tile math were ever off by a pixel, or if we change resolutions later, you'd see flickering garbage pixels in the gaps. Always clear. It costs almost nothing.

## Implementation

### Project Structure

After this lesson, your project looks like this:

```
asteroids/
├── CMakeLists.txt
├── src/
│   ├── main.c
│   └── config.h
├── include/
│   ├── Asteroids.h
│   ├── raygui.h
│   ├── reasings.h
│   └── rini.h
└── assets/
    └── Asteroids.png
```

### config.h

All constants in one place. Every future lesson adds to this file.

```c
#ifndef CONFIG_H
#define CONFIG_H

// --- Display ---
#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   480
#define TARGET_FPS      60

// --- Asset Paths ---
#define ATLAS_PATH      "assets/Asteroids.png"

// --- Atlas Sprite Indices ---
// These map directly to rtpDescAsteroids[] in Asteroids.h.
// We define them here so game code never uses raw numbers.
#define SPR_BACKGROUND      39
#define SPR_BACKGROUND_2    44

#endif // CONFIG_H
```

No magic numbers. If someone reads `SPR_BACKGROUND` in the draw code, they know what sprite it is. If they see `39`, they don't.

### main.c

The entire game, for now, fits in one file.

#### Includes and Atlas Helper

```c
#include "raylib.h"
#include "config.h"
#include "Asteroids.h"
```

We need a function to extract the correct source rectangle from the atlas data. This is the bridge between rTexPacker's generated data and Raylib's `DrawTextureRec`:

```c
Rectangle GetSpriteRect(int index)
{
    rtpAtlasSprite spr = rtpDescAsteroids[index];

    if (spr.trimmed) {
        return (Rectangle){
            spr.positionX + spr.trimRecX,
            spr.positionY + spr.trimRecY,
            spr.trimRecWidth,
            spr.trimRecHeight
        };
    }

    return (Rectangle){
        spr.positionX,
        spr.positionY,
        spr.sourceWidth,
        spr.sourceHeight
    };
}
```

For trimmed sprites, we offset into the actual pixel data. For non-trimmed sprites (like our background), we use the full area.

#### Initialization

```c
int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Asteroids");
    SetTargetFPS(TARGET_FPS);

    Texture2D atlas = LoadTexture(ATLAS_PATH);
    Rectangle bgSource = GetSpriteRect(SPR_BACKGROUND);
```

`LoadTexture` uploads the atlas PNG to the GPU once. We pre-calculate `bgSource` because it never changes — no reason to look it up every frame.

#### The Game Loop

```c
    while (!WindowShouldClose())
    {
        // Update — empty for now

        BeginDrawing();
            ClearBackground(BLACK);

            for (int y = 0; y < SCREEN_HEIGHT; y += (int)bgSource.height) {
                for (int x = 0; x < SCREEN_WIDTH; x += (int)bgSource.width) {
                    DrawTextureRec(atlas, bgSource, (Vector2){ x, y }, WHITE);
                }
            }
        EndDrawing();
    }
```

The background tile is 224×296 pixels. Our screen is 640×480. So we need 3 columns and 2 rows of tiles to cover it — the loop handles this automatically regardless of screen or tile size.

`DrawTextureRec` draws a rectangular region from a texture at a screen position. The `WHITE` tint means no color modification — draw the pixels as-is.

#### Cleanup

```c
    UnloadTexture(atlas);
    CloseWindow();
    return 0;
}
```

Always unload what you load. Raylib tracks GPU resources, and `CloseWindow` would clean up anyway, but being explicit is a good habit — and it matters in larger projects where you load and unload resources dynamically.

### CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.15)
project(asteroids C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(raylib 5.5 QUIET)

if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG        5.5
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(raylib)
endif()

add_executable(asteroids src/main.c)

target_include_directories(asteroids PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(asteroids PRIVATE raylib)

if (UNIX AND NOT APPLE)
    target_link_libraries(asteroids PRIVATE m pthread dl rt X11)
endif()

# Copy assets next to the executable so it can find them at runtime
add_custom_command(TARGET asteroids POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:asteroids>/assets
)
```

Key points: if Raylib isn't installed system-wide, CMake downloads and builds it automatically via `FetchContent`. The `POST_BUILD` step copies assets next to the binary so relative paths work.

## Building & Running

```bash
mkdir build && cd build
cmake ..
cmake --build .
./asteroids
```

First build takes a bit because Raylib compiles from source. Subsequent builds are fast.

## Result

A 640×480 window filled with a tiled starfield background. The tiles repeat seamlessly because the background sprite was designed for tiling. Press ESC or close the window to exit.

No ship, no asteroids, no input. That's next.

## Next

In **Lesson 02**, we add the player ship — loading it from the atlas, placing it on screen, and responding to keyboard input for movement and rotation.
