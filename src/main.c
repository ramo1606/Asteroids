#include "raylib.h"
#include "game.h"
#include "config.h"

int main(void)
{
    // Initialize raylib
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "RG35XX Square Demo");
    SetTargetFPS(60);
    
    // Initialize game state
    GameState game;
    InitGame(&game);
    
    // Main game loop
    while (!ShouldGameExit())
    {
        // Update game logic
        UpdateGame(&game);
        
        // Draw everything
        DrawGame(&game);
    }
    
    // Clean up
    CloseWindow();
    
    return 0;
}
