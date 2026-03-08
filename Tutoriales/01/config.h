/***********************************************************************************
*   config.h — Game Constants
*
*   All game-wide constants live here. No magic numbers in game logic.
*   This file grows with each lesson as we add new systems.
************************************************************************************/

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
