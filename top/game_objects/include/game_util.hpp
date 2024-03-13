#ifndef GAME_UTIL_HPP
#define GAME_UTIL_HPP

#include <cstdint>

#define FPS 60              // Frames per second
#define SCREEN_WIDTH 1920   // X-width in pixels
#define SCREEN_HEIGHT 1080  // Y-height in pixels

#define ACCEL_NORMALIZE_CONSTANT 0.5f

// Coefficient of restitution for ball-wall collisions
#define BALL_WALL_E 0.5

struct XYPairInt16 {
    int16_t x;
    int16_t y;
};

struct XYPairFloat {
    float x;
    float y;
};

XYPairFloat normalize_accel(XYPairInt16 input_accel);

float calculate_distance(XYPairFloat a, XYPairFloat b);

#endif
