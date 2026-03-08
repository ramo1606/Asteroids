//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
// rTexpacker v5.5 Atlas Descriptor Code exporter v5.0                          //
//                                                                              //
// more info and bugs-report:  github.com/raylibtech/rtools                     //
// feedback and support:       ray[at]raylibtech.com                            //
//                                                                              //
// Copyright (c) 2019-2025 raylib technologies (@raylibtech)                    //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////

#define ATLAS_ASTEROIDS_IMAGE_PATH      "Asteroids.png"
#define ATLAS_ASTEROIDS_SPRITE_COUNT    51

// Atlas sprite properties
typedef struct rtpAtlasSprite {
    const char *nameId;
    const char *tag;
    int originX, originY;
    int positionX, positionY;
    int sourceWidth, sourceHeight;
    int padding;
    bool trimmed;
    int trimRecX, trimRecY, trimRecWidth, trimRecHeight;
    int colliderType;
    int colliderPosX, colliderPosY, colliderSizeX, colliderSizeY;
} rtpAtlasSprite;

// Atlas sprites array
static rtpAtlasSprite rtpDescAsteroids[51] = {
    { "number_big_0", "BigNumber", 0, 0, 495, 243, 8, 14, 0, true, 2, 2, 6, 10, 0, 0, 0, 0, 0 },
    { "number_big_1", "BigNumber", 0, 0, 448, 242, 8, 14, 0, true, 2, 2, 6, 10, 0, 0, 0, 0, 0 },
    { "number_big_2", "BigNumber", 0, 0, 501, 243, 8, 14, 0, true, 2, 2, 6, 10, 0, 0, 0, 0, 0 },
    { "number_big_3", "BigNumber", 0, 0, 477, 244, 8, 14, 0, true, 2, 2, 6, 10, 0, 0, 0, 0, 0 },
    { "number_big_4", "BigNumber", 0, 0, 463, 246, 8, 14, 0, true, 2, 2, 6, 10, 0, 0, 0, 0, 0 },
    { "number_big_5", "BigNumber", 0, 0, 448, 252, 8, 14, 0, true, 2, 2, 6, 10, 0, 0, 0, 0, 0 },
    { "number_big_6", "BigNumber", 0, 0, 489, 253, 8, 14, 0, true, 2, 2, 6, 10, 0, 0, 0, 0, 0 },
    { "number_big_7", "BigNumber", 0, 0, 495, 253, 8, 14, 0, true, 2, 2, 6, 10, 0, 0, 0, 0, 0 },
    { "number_big_8", "BigNumber", 0, 0, 489, 243, 8, 14, 0, true, 2, 2, 6, 10, 0, 0, 0, 0, 0 },
    { "number_big_9", "BigNumber", 0, 0, 501, 253, 8, 14, 0, true, 2, 2, 6, 10, 0, 0, 0, 0, 0 },
    { "number_small_0", "SmallNumber", 0, 0, 472, 261, 6, 8, 0, true, 1, 1, 5, 6, 0, 0, 0, 0, 0 },
    { "number_small_1", "SmallNumber", 0, 0, 448, 268, 6, 8, 0, true, 2, 1, 3, 6, 0, 0, 0, 0, 0 },
    { "number_small_2", "SmallNumber", 0, 0, 472, 267, 6, 8, 0, true, 1, 1, 4, 6, 0, 0, 0, 0, 0 },
    { "number_small_3", "SmallNumber", 0, 0, 498, 263, 6, 8, 0, true, 1, 1, 4, 6, 0, 0, 0, 0, 0 },
    { "number_small_4", "SmallNumber", 0, 0, 494, 263, 6, 8, 0, true, 2, 1, 4, 6, 0, 0, 0, 0, 0 },
    { "number_small_5", "SmallNumber", 0, 0, 476, 267, 6, 8, 0, true, 1, 1, 4, 6, 0, 0, 0, 0, 0 },
    { "number_small_6", "SmallNumber", 0, 0, 489, 263, 6, 8, 0, true, 1, 1, 5, 6, 0, 0, 0, 0, 0 },
    { "number_small_7", "SmallNumber", 0, 0, 461, 263, 6, 8, 0, true, 1, 1, 4, 6, 0, 0, 0, 0, 0 },
    { "number_small_8", "SmallNumber", 0, 0, 448, 262, 6, 8, 0, true, 1, 1, 5, 6, 0, 0, 0, 0, 0 },
    { "number_small_9", "SmallNumber", 0, 0, 477, 261, 6, 8, 0, true, 1, 1, 5, 6, 0, 0, 0, 0, 0 },
    { "shoot_0", "Bullet", 0, 0, 448, 220, 16, 16, 0, true, 4, 3, 7, 11, 0, 0, 0, 0, 0 },
    { "shoot_1", "Bullet", 0, 0, 448, 231, 16, 16, 0, true, 4, 3, 7, 11, 0, 0, 0, 0, 0 },
    { "flash_0", "Flash", 0, 0, 472, 254, 16, 16, 0, true, 4, 4, 7, 7, 0, 0, 0, 0, 0 },
    { "flash_1", "Flash", 0, 0, 461, 256, 16, 16, 0, true, 4, 4, 7, 7, 0, 0, 0, 0, 0 },
    { "mine_0", "Mine", 0, 0, 463, 232, 16, 16, 0, true, 1, 1, 14, 14, 0, 0, 0, 0, 0 },
    { "mine_1", "Mine", 0, 0, 477, 218, 16, 16, 0, true, 1, 1, 14, 14, 0, 0, 0, 0, 0 },
    { "mine_2", "Mine", 0, 0, 463, 218, 16, 16, 0, true, 1, 1, 14, 14, 0, 0, 0, 0, 0 },
    { "mine_3", "Mine", 0, 0, 477, 204, 16, 16, 0, true, 1, 1, 14, 14, 0, 0, 0, 0, 0 },
    { "missile_0", "Missile", 0, 0, 490, 0, 16, 32, 0, true, 2, 1, 12, 28, 0, 0, 0, 0, 0 },
    { "missile_1", "Missile", 0, 0, 490, 28, 16, 32, 0, true, 2, 1, 12, 27, 0, 0, 0, 0, 0 },
    { "explosion_0", "Explosion", 0, 0, 464, 188, 32, 32, 0, true, 8, 8, 16, 16, 0, 0, 0, 0, 0 },
    { "explosion_1", "Explosion", 0, 0, 477, 232, 32, 32, 0, true, 9, 11, 12, 12, 0, 0, 0, 0, 0 },
    { "explosion_2", "Explosion", 0, 0, 463, 204, 32, 32, 0, true, 9, 9, 14, 14, 0, 0, 0, 0, 0 },
    { "explosion_3", "Explosion", 0, 0, 448, 188, 32, 32, 0, true, 8, 8, 16, 16, 0, 0, 0, 0, 0 },
    { "explosion_4", "Explosion", 0, 0, 448, 204, 32, 32, 0, true, 8, 8, 15, 16, 0, 0, 0, 0, 0 },
    { "explosion_5", "Explosion", 0, 0, 480, 188, 32, 32, 0, true, 7, 8, 16, 16, 0, 0, 0, 0, 0 },
    { "player", "Player", 0, 0, 448, 74, 48, 48, 0, true, 3, 7, 42, 35, 0, 0, 0, 0, 0 },
    { "player_boost", "Player", 0, 0, 448, 38, 48, 48, 0, true, 3, 7, 42, 36, 0, 0, 0, 0, 0 },
    { "heart_power_up", "Upgrade", 0, 0, 448, 154, 18, 18, 0, true, 0, 0, 18, 18, 0, 0, 0, 0, 0 },
    { "background", "Background", 0, 0, 224, 0, 224, 296, 0, false, 0, 0, 224, 296, 0, 0, 0, 0, 0 },
    { "meteor_4", "Asteroid", 0, 0, 448, 0, 48, 48, 0, true, 2, 6, 42, 38, 0, 0, 0, 0, 0 },
    { "meteor_3", "Asteroid", 0, 0, 448, 109, 32, 32, 0, true, 2, 2, 27, 26, 0, 0, 0, 0, 0 },
    { "meteor_2", "Asteroid", 0, 0, 475, 109, 32, 32, 0, true, 4, 8, 25, 20, 0, 0, 0, 0, 0 },
    { "meteor_1", "Asteroid", 0, 0, 484, 154, 32, 32, 0, true, 7, 9, 16, 17, 0, 0, 0, 0, 0 },
    { "background_2", "Background", 0, 0, 0, 0, 224, 296, 0, false, 0, 25, 224, 249, 0, 0, 0, 0, 0 },
    { "missle_counter", "UI", 0, 0, 448, 172, 31, 16, 0, true, 0, 0, 31, 16, 0, 0, 0, 0, 0 },
    { "bullets_counter", "UI", 0, 0, 479, 172, 31, 16, 0, true, 0, 0, 31, 16, 0, 0, 0, 0, 0 },
    { "kill_count", "UI", 0, 0, 448, 135, 46, 19, 0, true, 0, 0, 46, 19, 0, 0, 0, 0, 0 },
    { "heart", "UI", 0, 0, 489, 232, 16, 16, 0, true, 1, 3, 14, 11, 0, 0, 0, 0, 0 },
    { "health_bar", "UI", 0, 0, 0, 296, 58, 19, 0, true, 0, 0, 58, 19, 0, 0, 0, 0, 0 },
    { "missle_power_up", "Upgrade", 0, 0, 466, 154, 18, 18, 0, true, 0, 0, 18, 18, 0, 0, 0, 0, 0 },
};
