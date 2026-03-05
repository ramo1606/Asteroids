#ifndef CONFIG_H
#define CONFIG_H

#include "raylib.h"

// Screen dimensions
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// Game constants
#define ROTATION_STEP 15.0f
#define SIZE_STEP 10.0f
#define MIN_SIZE 10.0f
#define MAX_SIZE 200.0f

// Color array for cycling through different colors
extern Color colors[];
extern const int COLOR_COUNT;

#endif // CONFIG_H
