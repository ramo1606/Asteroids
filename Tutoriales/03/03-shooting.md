# Lesson 03: Shooting

## What We're Building

Press space to shoot. Bullets spawn from the ship's nose, travel in the facing direction, animate between two frames, and disappear after a couple of seconds. Hold space to keep firing with a cooldown between shots.

This lesson introduces **object pooling** — the standard pattern for managing many short-lived entities in games.

## Concepts

### Object Pooling

Bullets come and go constantly. A player holding the fire key spawns 6–7 bullets per second, each living for 2 seconds. You could `malloc` each bullet and `free` it on death, but that's wasteful — dynamic allocation is slow relative to game frame times, and you risk fragmentation.

The solution: pre-allocate a fixed-size array and reuse slots.

```c
Bullet bullets[MAX_BULLETS] = { 0 };   // All slots start inactive
```

Each bullet has an `active` flag. To spawn: find the first inactive slot, fill it in. To kill: set `active = false`. No allocation, no deallocation, no pointer management. Just a flag and a loop.

This is how virtually every arcade game manages bullets, particles, enemies, and other transient objects. It's simple, cache-friendly, and has zero overhead.

**What happens when the pool is full?** The bullet is silently dropped. No crash, no error. In practice, `MAX_BULLETS = 32` with a 0.15s cooldown and 2s lifetime means the player can have at most ~13 bullets alive at once. Plenty of headroom.

### Fire Rate and Cooldowns

Without a cooldown, holding space would spawn a bullet every frame — 60 bullets per second. That's not gameplay, that's a laser beam.

A cooldown is a timer that counts down each frame. When it reaches zero, the action is allowed:

```c
player.fireCooldown -= dt;
if (IsKeyDown(KEY_SPACE) && player.fireCooldown <= 0.0f) {
    SpawnBullet(bullets, &player);
    player.fireCooldown = PLAYER_FIRE_COOLDOWN;     // Reset the timer
}
```

This pattern appears everywhere in games: attack speed, ability cooldowns, spawn timers, invincibility frames. The mechanic is always the same — a float that ticks down.

### Sprite Animation

Our bullet has two frames: `shoot_0` and `shoot_1`. We animate by accumulating time and picking a frame:

```c
int frame = (int)(animTimer * BULLET_ANIM_SPEED) % 2;
int sprite = SPR_BULLET_0 + frame;
```

`animTimer` grows every frame. Multiply by speed (frames per second), cast to int, mod by frame count. At `BULLET_ANIM_SPEED = 10.0`, the bullet swaps frames 10 times per second.

This formula works for any frame count. A 6-frame explosion would use `% 6`. A 4-frame walk cycle, `% 4`. Same pattern everywhere.

## Why This? / Alternatives

### Why a fixed pool instead of a dynamic list?

For a game this size, a fixed array is the right tool. It avoids heap allocation entirely, has predictable memory use, and iterating over 32 elements is effectively free on modern hardware.

**Alternatives:**
- **Dynamic array / linked list** — adds complexity (realloc, pointer management) with no benefit at our scale. Makes sense for an MMO server tracking thousands of projectiles, not for 32 bullets.
- **Entity Component System (ECS)** — a powerful architecture for large games, but massive overkill here. An ECS adds a framework; we just need an array.

### Why bullets ignore the ship's velocity?

In our implementation, bullet speed is absolute — `BULLET_SPEED` in the facing direction, period. We don't add the ship's velocity to the bullet.

Physically, you'd expect a moving ship to impart its momentum to the bullet. But in Asteroids-style games, absolute bullet speed feels better: your shots are predictable regardless of how fast you're moving. The original Asteroids did it this way, and most shmups do too.

**Alternative:** Add `player.velocity` to the bullet's velocity at spawn time for realistic physics. Try it — it feels slippery.

### Why `IsKeyDown` instead of `IsKeyPressed`?

`IsKeyPressed` fires once when the key is first pressed. `IsKeyDown` fires every frame the key is held. Combined with the cooldown, `IsKeyDown` gives us auto-fire: hold space and bullets stream out at the cooldown rate. Using `IsKeyPressed` would require tapping space for each shot.

## Implementation

### config.h — New Constants

```c
#define SPR_BULLET_0        20
#define SPR_BULLET_1        21

#define PLAYER_FIRE_COOLDOWN    0.15f       // Seconds between shots

#define MAX_BULLETS         32              // Pool size
#define BULLET_SPEED        400.0f          // Pixels per second
#define BULLET_LIFETIME     2.0f            // Seconds before auto-despawn
#define BULLET_ANIM_SPEED   10.0f           // Frame swaps per second
#define BULLET_SPAWN_OFFSET 20.0f           // Distance from ship center to muzzle
```

`BULLET_SPAWN_OFFSET` pushes the bullet's spawn point forward from the ship's center. Without it, bullets would appear in the middle of the ship. 20 pixels puts them roughly at the nose.

### The Bullet Struct

```c
typedef struct Bullet {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    float lifetime;
    float animTimer;
    bool active;
} Bullet;
```

Every field has a purpose. `rotation` is set once at spawn (matches the ship's angle). `lifetime` counts down. `animTimer` counts up. `active` gates the entire update/draw logic.

### SpawnBullet

```c
void SpawnBullet(Bullet pool[], Player *player)
{
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!pool[i].active) {
            float radians = player->rotation * DEG2RAD;
            Vector2 direction = { sinf(radians), -cosf(radians) };

            pool[i] = (Bullet){
                .position = Vector2Add(
                    player->position,
                    Vector2Scale(direction, BULLET_SPAWN_OFFSET)
                ),
                .velocity = Vector2Scale(direction, BULLET_SPEED),
                .rotation = player->rotation,
                .lifetime = BULLET_LIFETIME,
                .animTimer = 0.0f,
                .active = true,
            };
            return;
        }
    }
}
```

The linear scan to find a free slot is fine for 32 elements. You could track a free list index for O(1) spawning, but the complexity isn't justified here.

Note the compound literal `(Bullet){ ... }` — this overwrites the entire struct, ensuring no stale data from a previous bullet leaks through. C99 designated initializers make it readable.

### UpdateBullets and DrawBullets

The update loop is straightforward — move, animate, age, kill:

```c
void UpdateBullets(Bullet pool[], float dt)
{
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!pool[i].active) continue;

        pool[i].position = Vector2Add(
            pool[i].position,
            Vector2Scale(pool[i].velocity, dt)
        );
        pool[i].animTimer += dt;
        pool[i].lifetime -= dt;

        if (pool[i].lifetime <= 0.0f) {
            pool[i].active = false;
        }
    }
}
```

Drawing selects the animation frame and delegates to `DrawSprite`:

```c
void DrawBullets(Bullet pool[], Texture2D atlas)
{
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!pool[i].active) continue;

        int frame = (int)(pool[i].animTimer * BULLET_ANIM_SPEED) % 2;
        int sprite = SPR_BULLET_0 + frame;
        DrawSprite(atlas, sprite, pool[i].position, pool[i].rotation, WHITE);
    }
}
```

### Draw Order

Bullets are drawn before the player:

```c
DrawBullets(bullets, atlas);
DrawSprite(atlas, playerSprite, player.position, player.rotation, WHITE);
```

This makes bullets appear to emerge from under the ship. If drawn after, they'd pop in on top of it — visually jarring.

## Result

A ship that flies and shoots. Hold space to stream bullets forward. Bullets animate, travel straight, and fade out after 2 seconds. The fire rate is capped at ~6.7 shots per second. Bullets can still fly off-screen (screen wrapping is coming in Lesson 06).

## Next

In **Lesson 04**, we spawn asteroids — big rocks that drift across the screen and split into smaller ones when destroyed. This sets up the core gameplay loop.
