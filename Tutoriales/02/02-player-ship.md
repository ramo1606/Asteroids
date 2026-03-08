# Lesson 02: The Player Ship

## What We're Building

A ship at the center of the screen that rotates with the arrow keys and thrusts forward when you press up. It drifts with momentum and gradually slows down. When thrusting, the sprite swaps to show engine flames.

This is the first interactive element in the game. After this lesson, you have something you can fly around.

## Concepts

### Game Entities as Structs

In C, a game entity is just a struct. No base class, no component system, no ECS. A player is a position, a velocity, a rotation, and whatever else it needs:

```c
typedef struct Player {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    bool thrusting;
} Player;
```

This is the simplest pattern that works. Each lesson adds fields as needed. When we later add asteroids and bullets, they get their own structs.

### Thrust-Based Movement

Classic Asteroids movement isn't direct — you don't set your position from input. Instead:

1. **Rotation** — Left/right keys change the ship's facing angle.
2. **Thrust** — Up key applies acceleration in the facing direction.
3. **Velocity** — Accumulates from thrust, moves the ship each frame.
4. **Drag** — A friction multiplier that gradually slows the ship.

This gives the ship a sliding, momentum-driven feel. You have to think ahead — point where you want to go, thrust, then counter-thrust to stop. It's what makes Asteroids feel like Asteroids.

### Delta Time

Games need consistent speed regardless of frame rate. If a frame takes longer than expected, the ship should move proportionally more to compensate. `GetFrameTime()` returns the time elapsed since the last frame (in seconds). We multiply all per-frame changes by this value:

```c
player.rotation += PLAYER_ROTATION_SPEED * dt;  // degrees/sec * seconds = degrees
```

At 60 FPS, `dt` ≈ 0.0167. At 30 FPS, `dt` ≈ 0.033. The ship rotates at the same real-world speed either way.

### Drawing Rotated Sprites from an Atlas

Drawing a sprite without rotation is simple — `DrawTextureRec` takes a source rectangle and a screen position. Rotation adds complexity because you need to specify a pivot point and use `DrawTexturePro`.

Our `GetSpriteRect` helper already returns the tight bounding box for trimmed sprites — the actual visible pixels in the atlas. For rotation, we set the origin at the center of that trimmed rectangle. `DrawTexturePro` uses this origin as both the rotation pivot and the anchor point placed at the destination position.

The result: `position` in our game code always means "where the sprite's center is on screen".

## Why This? / Alternatives

### Why thrust + drag instead of direct movement?

Direct movement (`position += direction * speed`) is simpler to implement but removes the core feel of Asteroids. The sliding, inertia-driven movement is a defining game mechanic — it creates the tension between "where I'm going" and "where I'm pointing". Games like Geometry Wars, Solar Jetman, and Gravitron all use this pattern.

**Alternative:** For a twin-stick shooter, you'd use direct movement. Different game, different feel.

### Why a separate `DrawSprite` function?

We could inline the `DrawTexturePro` call everywhere. But every entity in the game (player, asteroids, bullets, explosions) needs the same trim-aware rotation logic. One function, used everywhere.

### Why `raymath.h`?

Raylib includes `raymath.h` — a header-only math library with `Vector2` operations. We could write `player.velocity.x += direction.x * thrust * dt` by hand, but `Vector2Add` and `Vector2Scale` are clearer and less error-prone. No performance cost — these are all inline functions.

## Implementation

### config.h — New Constants

```c
#define SPR_PLAYER          36
#define SPR_PLAYER_BOOST    37

#define PLAYER_ROTATION_SPEED   200.0f      // Degrees per second
#define PLAYER_THRUST           300.0f      // Acceleration in pixels/sec²
#define PLAYER_MAX_SPEED        250.0f      // Max velocity magnitude
#define PLAYER_DRAG             0.98f       // Velocity multiplier per frame
```

These values are tuned for feel. Tweak them — make rotation snappier, thrust stronger, drag heavier. Game feel lives in these numbers.

A note on drag: `0.98` means the ship retains 98% of its speed each frame. At 60 FPS that's roughly `0.98^60 ≈ 0.30` — the ship loses 70% of its speed per second. Lower values = more friction. `1.0` = no friction (ship slides forever).

### The DrawSprite Helper

This is the key new function. It handles the math so every draw call in the game is just one line:

```c
void DrawSprite(Texture2D atlas, int index, Vector2 position, float rotation, Color tint)
{
    Rectangle source = GetSpriteRect(index);

    Vector2 origin = { source.width / 2.0f, source.height / 2.0f };

    Rectangle dest = {
        position.x, position.y,
        source.width, source.height
    };

    DrawTexturePro(atlas, source, dest, origin, rotation, tint);
}
```

How `DrawTexturePro` works:
- **source** — what rectangle to read from the texture.
- **dest** — where to draw on screen. The `x, y` is the anchor position, `width, height` sets the draw size (scaling happens if it differs from source).
- **origin** — pivot point relative to dest's top-left. Gets placed at `(dest.x, dest.y)`.
- **rotation** — degrees, clockwise.

So when we call `DrawSprite(atlas, SPR_PLAYER, player.position, player.rotation, WHITE)`, the sprite's logical center lands exactly at `player.position`, and rotation spins around that point.

### Movement Update

The rotation-to-direction conversion needs a moment of explanation:

```c
float radians = player.rotation * DEG2RAD;
Vector2 direction = { sinf(radians), -cosf(radians) };
```

When `rotation = 0` (pointing up), we want direction `(0, -1)` — negative Y because screen Y points down. `sin(0) = 0`, `-cos(0) = -1`. Correct.

When `rotation = 90` (pointing right), we want `(1, 0)`. `sin(90°) = 1`, `-cos(90°) = 0`. Correct.

The rest of the update is straightforward vector math:

```c
// Apply thrust
player.velocity = Vector2Add(player.velocity, Vector2Scale(direction, PLAYER_THRUST * dt));

// Cap speed
if (Vector2Length(player.velocity) > PLAYER_MAX_SPEED) {
    player.velocity = Vector2Scale(Vector2Normalize(player.velocity), PLAYER_MAX_SPEED);
}

// Drag
player.velocity = Vector2Scale(player.velocity, PLAYER_DRAG);

// Move
player.position = Vector2Add(player.position, Vector2Scale(player.velocity, dt));
```

### Thrust Animation

One line swaps the sprite based on the thrust state:

```c
int playerSprite = player.thrusting ? SPR_PLAYER_BOOST : SPR_PLAYER;
DrawSprite(atlas, playerSprite, player.position, player.rotation, WHITE);
```

`player_boost` has the engine flame drawn into the sprite. No particle system needed — it's a simple sprite swap. When you release the up key, the flame disappears instantly. It's minimal but reads well at game speed.

## Result

A 640×480 window with the starfield background and a ship in the center. Arrow keys rotate it, up thrusts forward with visible engine flames. The ship slides with momentum and gradually decelerates. It can fly off-screen (we'll fix that in Lesson 06 with screen wrapping).

## Next

In **Lesson 03**, we add shooting — pressing a key spawns bullets that travel in the ship's facing direction. This introduces object pooling, a fundamental pattern for managing many short-lived entities.
