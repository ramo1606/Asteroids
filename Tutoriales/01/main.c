/***********************************************************************************
*
*   Asteroids Clone — Lesson 01: Window & Game Loop
*
*   In this lesson we:
*   - Open a Raylib window
*   - Load the texture atlas
*   - Tile the background across the screen
*   - Establish the Init/Update/Draw pattern
*
************************************************************************************/

#include "raylib.h"
#include "config.h"
#include "Asteroids.h"

//-----------------------------------------------------------------------------------
// Atlas Helper
//-----------------------------------------------------------------------------------

// Returns the source rectangle for a sprite in the atlas.
// For trimmed sprites, this returns the tight bounding box (actual pixels).
// For non-trimmed sprites, it returns the full sprite area.
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

//-----------------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------------
int main(void)
{
    //-------------------------------------------------------------------------------
    // Init
    //-------------------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Asteroids");
    SetTargetFPS(TARGET_FPS);

    // Load the texture atlas — one texture for all sprites
    Texture2D atlas = LoadTexture(ATLAS_PATH);

    // Pre-calculate the background source rectangle from the atlas.
    // This avoids recalculating it every frame.
    Rectangle bgSource = GetSpriteRect(SPR_BACKGROUND);

    //-------------------------------------------------------------------------------
    // Game Loop
    //-------------------------------------------------------------------------------
    while (!WindowShouldClose())    // ESC or close button ends the loop
    {
        //-----------------------------------------------------------------------
        // Update
        //-----------------------------------------------------------------------
        // Nothing to update yet — the background is static.
        // This section will grow as we add gameplay.

        //-----------------------------------------------------------------------
        // Draw
        //-----------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);

            // Tile the background across the entire screen.
            // The background sprite is 224x296 — smaller than our 640x480 window.
            // We draw it in a grid pattern to fill the screen.
            for (int y = 0; y < SCREEN_HEIGHT; y += (int)bgSource.height) {
                for (int x = 0; x < SCREEN_WIDTH; x += (int)bgSource.width) {
                    DrawTextureRec(atlas, bgSource, (Vector2){ x, y }, WHITE);
                }
            }

        EndDrawing();
    }

    //-------------------------------------------------------------------------------
    // Cleanup
    //-------------------------------------------------------------------------------
    UnloadTexture(atlas);
    CloseWindow();

    return 0;
}
