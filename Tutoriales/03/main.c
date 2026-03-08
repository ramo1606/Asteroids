/***********************************************************************************
*
*   Asteroids Clone — Lesson 03: Shooting
*
*   In this lesson we:
*   - Introduce the Bullet struct and an object pool (fixed-size array)
*   - Spawn bullets from the ship's nose in the facing direction
*   - Animate bullets with a 2-frame sprite cycle
*   - Manage bullet lifetime and automatic despawning
*   - Add a fire cooldown to prevent infinite spam
*
************************************************************************************/

#include "raylib.h"
#include "raymath.h"
#include <math.h>

#include "config.h"
#include "Asteroids.h"

//-----------------------------------------------------------------------------------
// Types
//-----------------------------------------------------------------------------------
typedef struct Player {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    bool thrusting;
    float fireCooldown;     // Time remaining before the player can fire again.
} Player;

typedef struct Bullet {
    Vector2 position;
    Vector2 velocity;
    float rotation;         // Matches the ship's rotation at spawn time.
    float lifetime;         // Seconds remaining. When <= 0, bullet is inactive.
    float animTimer;        // Accumulates time for frame switching.
    bool active;            // False = this pool slot is available.
} Bullet;

//-----------------------------------------------------------------------------------
// Atlas Helpers
//-----------------------------------------------------------------------------------

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

void DrawSprite(Texture2D atlas, int index, Vector2 position, float rotation, Color tint)
{
    Rectangle source = GetSpriteRect(index);
    Vector2 origin = { source.width / 2.0f, source.height / 2.0f };
    Rectangle dest = { position.x, position.y, source.width, source.height };
    DrawTexturePro(atlas, source, dest, origin, rotation, tint);
}

//-----------------------------------------------------------------------------------
// Bullet Functions
//-----------------------------------------------------------------------------------

// Finds the first inactive slot in the pool and spawns a bullet there.
// If the pool is full, the bullet is silently dropped — no allocation,
// no crash. This is the intended behavior for object pools.
void SpawnBullet(Bullet pool[], Player *player)
{
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!pool[i].active) {
            float radians = player->rotation * DEG2RAD;
            Vector2 direction = { sinf(radians), -cosf(radians) };

            pool[i] = (Bullet){
                // Spawn at the nose of the ship, not at its center
                .position = Vector2Add(
                    player->position,
                    Vector2Scale(direction, BULLET_SPAWN_OFFSET)
                ),
                // Bullets travel at a fixed speed, independent of ship velocity
                .velocity = Vector2Scale(direction, BULLET_SPEED),
                .rotation = player->rotation,
                .lifetime = BULLET_LIFETIME,
                .animTimer = 0.0f,
                .active = true,
            };
            return;
        }
    }
    // Pool full — bullet is lost. In practice, MAX_BULLETS is generous
    // enough that this rarely happens.
}

void UpdateBullets(Bullet pool[], float dt)
{
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!pool[i].active) continue;

        // Move
        pool[i].position = Vector2Add(
            pool[i].position,
            Vector2Scale(pool[i].velocity, dt)
        );

        // Animate — accumulate time and use it to pick the frame
        pool[i].animTimer += dt;

        // Age and despawn
        pool[i].lifetime -= dt;
        if (pool[i].lifetime <= 0.0f) {
            pool[i].active = false;
        }
    }
}

void DrawBullets(Bullet pool[], Texture2D atlas)
{
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!pool[i].active) continue;

        // 2-frame animation: alternate based on accumulated time.
        // (int)(timer * speed) % frameCount gives us the current frame.
        int frame = (int)(pool[i].animTimer * BULLET_ANIM_SPEED) % 2;
        int sprite = SPR_BULLET_0 + frame;

        DrawSprite(atlas, sprite, pool[i].position, pool[i].rotation, WHITE);
    }
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

    Player player = {
        .position = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f },
        .velocity = { 0, 0 },
        .rotation = 0.0f,
        .thrusting = false,
        .fireCooldown = 0.0f,
    };

    // The bullet pool — all slots start inactive (zero-initialized).
    Bullet bullets[MAX_BULLETS] = { 0 };

    //-------------------------------------------------------------------------------
    // Game Loop
    //-------------------------------------------------------------------------------
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        //-----------------------------------------------------------------------
        // Update
        //-----------------------------------------------------------------------

        // --- Player movement (unchanged from lesson 02) ---
        if (IsKeyDown(KEY_LEFT))  player.rotation -= PLAYER_ROTATION_SPEED * dt;
        if (IsKeyDown(KEY_RIGHT)) player.rotation += PLAYER_ROTATION_SPEED * dt;

        player.thrusting = IsKeyDown(KEY_UP);
        if (player.thrusting) {
            float radians = player.rotation * DEG2RAD;
            Vector2 direction = { sinf(radians), -cosf(radians) };
            player.velocity = Vector2Add(
                player.velocity,
                Vector2Scale(direction, PLAYER_THRUST * dt)
            );
        }

        if (Vector2Length(player.velocity) > PLAYER_MAX_SPEED) {
            player.velocity = Vector2Scale(
                Vector2Normalize(player.velocity),
                PLAYER_MAX_SPEED
            );
        }

        player.velocity = Vector2Scale(player.velocity, PLAYER_DRAG);
        player.position = Vector2Add(
            player.position,
            Vector2Scale(player.velocity, dt)
        );

        // --- Shooting ---
        // Cooldown decreases each frame. When it hits zero, the player can fire.
        player.fireCooldown -= dt;
        if (player.fireCooldown < 0.0f) player.fireCooldown = 0.0f;

        if (IsKeyDown(KEY_SPACE) && player.fireCooldown <= 0.0f) {
            SpawnBullet(bullets, &player);
            player.fireCooldown = PLAYER_FIRE_COOLDOWN;
        }

        // --- Bullets ---
        UpdateBullets(bullets, dt);

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

            // Bullets (drawn behind the player so they emerge from under the ship)
            DrawBullets(bullets, atlas);

            // Player
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
