#include "game.h"
#include <math.h>

// Color array definition
Color colors[] = {
    {230, 41, 55, 255},      // RED
    {0, 228, 48, 255},       // GREEN
    {0, 121, 241, 255},      // BLUE
    {253, 249, 0, 255},      // YELLOW
    {255, 161, 0, 255},      // ORANGE
    {200, 122, 255, 255},    // PURPLE
    {255, 109, 194, 255},    // PINK
    {0, 255, 0, 255},        // LIME
    {190, 33, 55, 255},      // MAROON
    {0, 117, 44, 255},       // DARKGREEN
    {255, 0, 255, 255},      // MAGENTA
    {255, 203, 0, 255},      // GOLD
    {102, 191, 255, 255}     // SKYBLUE
};
const int COLOR_COUNT = 13;

void InitGame(GameState* game)
{
    game->squarePosition = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    game->squareSize = 50.0f;
    game->rotation = 0.0f;
    game->currentColorIndex = 0;
    
    // Initialize input state
    game->leftPressed = false;
    game->rightPressed = false;
    game->upPressed = false;
    game->downPressed = false;
    game->aPressed = false;
    game->bPressed = false;
    
    // Check if gamepad is available
    game->useGamepad = IsGamepadAvailable(0);
}

void UpdateGame(GameState* game)
{
    // Update gamepad availability each frame
    game->useGamepad = IsGamepadAvailable(0);
    
    // Handle input
    HandleInput(game);
    
    // Keep rotation in 0-360 range
    if (game->rotation >= 360.0f) game->rotation -= 360.0f;
    if (game->rotation < 0.0f) game->rotation += 360.0f;
}

void DrawGame(const GameState* game)
{
    BeginDrawing();
    
    ClearBackground(DARKGRAY);
    
    // Draw the rotated square
    DrawRectanglePro(
        (Rectangle){ game->squarePosition.x, game->squarePosition.y, game->squareSize, game->squareSize },
        (Vector2){ game->squareSize / 2.0f, game->squareSize / 2.0f }, // Origin (center of square)
        game->rotation,
        colors[game->currentColorIndex]
    );
    
    // Draw FPS
    DrawFPS(10, 10);
    
    // Draw input method indicator
    const char* inputMethod = game->useGamepad ? "Gamepad" : "Keyboard";
    DrawText(TextFormat("Input: %s", inputMethod), 10, 35, 20, game->useGamepad ? GREEN : YELLOW);
    
    // Draw instructions
    DrawText("D-PAD L/R: Rotate", 10, 65, 20, WHITE);
    DrawText("D-PAD U/D: Change Color", 10, 90, 20, WHITE);
    DrawText("A: Scale Up", 10, 115, 20, WHITE);
    DrawText("B: Scale Down", 10, 140, 20, WHITE);
    DrawText("START: Exit", 10, 165, 20, WHITE);
    
    // Draw current values
    DrawText(TextFormat("Rotation: %.1f°", game->rotation), 10, SCREEN_HEIGHT - 105, 20, WHITE);
    DrawText(TextFormat("Size: %.1f", game->squareSize), 10, SCREEN_HEIGHT - 80, 20, WHITE);
    DrawText(TextFormat("Color: %d/%d", game->currentColorIndex + 1, COLOR_COUNT), 10, SCREEN_HEIGHT - 55, 20, WHITE);
    
    // Debug info for gamepad
    if (game->useGamepad)
    {
        DrawText(TextFormat("Gamepad: %s", GetGamepadName(0)), 10, SCREEN_HEIGHT - 30, 16, GRAY);
    }
    else
    {
        DrawText("No gamepad detected", 10, SCREEN_HEIGHT - 30, 16, GRAY);
    }
    
    EndDrawing();
}

bool ShouldGameExit(void)
{
    if (WindowShouldClose()) return true;
    
    bool useGamepad = IsGamepadAvailable(0);
    bool startPressed = false;
    
    if (useGamepad)
    {
        startPressed = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT);
    }
    else
    {
        // Fallback: Use Enter/Escape for start button
        startPressed = IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE);
    }
    
    return startPressed;
}

void HandleInput(GameState* game)
{
    // Handle input - try gamepad first, fallback to keyboard
    bool currentLeft, currentRight, currentUp, currentDown, currentA, currentB;
    
    if (game->useGamepad)
    {
        // Gamepad input
        currentLeft = IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
        currentRight = IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
        currentUp = IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_UP);
        currentDown = IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
        currentA = IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
        currentB = IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);
    }
    else
    {
        // Keyboard fallback (common RG35XX key mappings)
        currentLeft = IsKeyDown(KEY_LEFT);
        currentRight = IsKeyDown(KEY_RIGHT);
        currentUp = IsKeyDown(KEY_UP);
        currentDown = IsKeyDown(KEY_DOWN);
        currentA = IsKeyDown(KEY_Z);  // A button typically mapped to Z
        currentB = IsKeyDown(KEY_X);  // B button typically mapped to X
    }
    
    // Handle D-pad input for rotation (Left/Right)
    if (currentLeft && !game->leftPressed)
    {
        game->rotation -= ROTATION_STEP; // Rotate counter-clockwise
        game->leftPressed = true;
    }
    else if (!currentLeft)
    {
        game->leftPressed = false;
    }
    
    if (currentRight && !game->rightPressed)
    {
        game->rotation += ROTATION_STEP; // Rotate clockwise
        game->rightPressed = true;
    }
    else if (!currentRight)
    {
        game->rightPressed = false;
    }
    
    // Handle D-pad input for color change (Up/Down)
    if (currentUp && !game->upPressed)
    {
        game->currentColorIndex = (game->currentColorIndex + 1) % COLOR_COUNT;
        game->upPressed = true;
    }
    else if (!currentUp)
    {
        game->upPressed = false;
    }
    
    if (currentDown && !game->downPressed)
    {
        game->currentColorIndex = (game->currentColorIndex - 1 + COLOR_COUNT) % COLOR_COUNT;
        game->downPressed = true;
    }
    else if (!currentDown)
    {
        game->downPressed = false;
    }
    
    // Handle A button for scaling up
    if (currentA && !game->aPressed)
    {
        game->squareSize += SIZE_STEP;
        if (game->squareSize > MAX_SIZE) game->squareSize = MAX_SIZE;
        game->aPressed = true;
    }
    else if (!currentA)
    {
        game->aPressed = false;
    }
    
    // Handle B button for scaling down
    if (currentB && !game->bPressed)
    {
        game->squareSize -= SIZE_STEP;
        if (game->squareSize < MIN_SIZE) game->squareSize = MIN_SIZE;
        game->bPressed = true;
    }
    else if (!currentB)
    {
        game->bPressed = false;
    }
}
