#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "config.h"

// Game state structure
typedef struct {
    Vector2 squarePosition;
    float squareSize;
    float rotation;
    int currentColorIndex;
    
    // Input state tracking for single press events
    bool leftPressed;
    bool rightPressed;
    bool upPressed;
    bool downPressed;
    bool aPressed;
    bool bPressed;
    
    bool useGamepad;
} GameState;

// Function declarations
void InitGame(GameState* game);
void UpdateGame(GameState* game);
void DrawGame(const GameState* game);
bool ShouldGameExit(void);

// Input handling
void HandleInput(GameState* game);
void UpdateInputState(GameState* game);

#endif // GAME_H
