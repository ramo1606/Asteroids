/***********************************************************************************
*
*   Asteroids Clone — Lesson 02: The Player Ship
*
*   In this lesson we:
*   - Define the Player struct
*   - Draw a rotated sprite from the atlas (handling trim offsets)
*   - Implement thrust-based movement with drag
*   - Respond to keyboard input for rotation and thrust
*
************************************************************************************/

#include "raylib.h"
#include "raymath.h"        // Vector2Add, Vector2Scale, Vector2Length, etc.
#include <math.h>           // sinf, cosf

#include "config.h"
#include "Asteroids.h"

//-----------------------------------------------------------------------------------
// Types
//-----------------------------------------------------------------------------------
typedef struct Player {
    Vector2 position;
    Vector2 velocity;
    float rotation;         // Degrees. 0 = pointing up, increases clockwise.
    bool thrusting;         // True while the thrust key is held — drives animation.
} Player;

//-----------------------------------------------------------------------------------
// Atlas Helpers
//-----------------------------------------------------------------------------------

// Returns the source rectangle for a sprite in the atlas.
// For trimmed sprites, returns the tight bounding box (actual pixels).
// For non-trimmed sprites, returns the full sprite area.
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

// Draws a sprite from the atlas at a world position with rotation.
// The rotation pivot is the center of the trimmed sprite.
void DrawSprite(Texture2D atlas, int index, Vector2 position, float rotation, Color tint)
{
    Rectangle source = GetSpriteRect(index);

    // Origin at the center of the trimmed sprite.
    // Rotation pivots around this point.
    Vector2 origin = { source.width / 2.0f, source.height / 2.0f };

    Rectangle dest = {
        position.x, position.y,
        source.width, source.height
    };

    DrawTexturePro(atlas, source, dest, origin, rotation, tint);
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

    Texture2D atlas = LoadTexture(ATLAS_PATH);
    Rectangle bgSource = GetSpriteRect(SPR_BACKGROUND);

    // Player starts at the center of the screen, stationary, pointing up.
    Player player = {
        .position = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f },
        .velocity = { 0, 0 },
        .rotation = 0.0f,
        .thrusting = false,
    };

    //-------------------------------------------------------------------------------
    // Game Loop
    //-------------------------------------------------------------------------------
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        //-----------------------------------------------------------------------
        // Update
        //-----------------------------------------------------------------------

        // Rotation: left/right arrows
        if (IsKeyDown(KEY_LEFT))  player.rotation -= PLAYER_ROTATION_SPEED * dt;
        if (IsKeyDown(KEY_RIGHT)) player.rotation += PLAYER_ROTATION_SPEED * dt;

        // Thrust: up arrow accelerates in the direction the ship is facing.
        // We convert the rotation angle to a direction vector.
        // Rotation 0 = up = (0, -1). Raylib angles are clockwise, and
        // screen Y grows downward, so:
        //   direction.x =  sin(angle)
        //   direction.y = -cos(angle)
        player.thrusting = IsKeyDown(KEY_UP);

        if (player.thrusting) {
            float radians = player.rotation * DEG2RAD;
            Vector2 direction = { sinf(radians), -cosf(radians) };
            player.velocity = Vector2Add(
                player.velocity,
                Vector2Scale(direction, PLAYER_THRUST * dt)
            );
        }

        // Clamp velocity to max speed
        if (Vector2Length(player.velocity) > PLAYER_MAX_SPEED) {
            player.velocity = Vector2Scale(
                Vector2Normalize(player.velocity),
                PLAYER_MAX_SPEED
            );
        }

        // Apply drag — velocity decays each frame so the ship doesn't slide forever.
        // This isn't physically realistic but it feels right for an arcade game.
        player.velocity = Vector2Scale(player.velocity, PLAYER_DRAG);

        // Update position
        player.position = Vector2Add(
            player.position,
            Vector2Scale(player.velocity, dt)
        );

        //-----------------------------------------------------------------------
        // Draw
        //-----------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);

            // Background tiling
            for (int y = 0; y < SCREEN_HEIGHT; y += (int)bgSource.height) {
                for (int x = 0; x < SCREEN_WIDTH; x += (int)bgSource.width) {
                    DrawTextureRec(atlas, bgSource, (Vector2){ x, y }, WHITE);
                }
            }

            // Player — show thrust sprite when accelerating
            int playerSprite = player.thrusting ? SPR_PLAYER_BOOST : SPR_PLAYER;
            DrawSprite(atlas, playerSprite, player.position, player.rotation, WHITE);

        EndDrawing();
    }

    //-------------------------------------------------------------------------------
    // Cleanup
    //-------------------------------------------------------------------------------
    UnloadTexture(atlas);
    CloseWindow();

    return 0;
}
