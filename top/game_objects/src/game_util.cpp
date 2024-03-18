#include "game_util.hpp"
#include <cmath>

XYPairFloat normalize_accel(XYPairInt16 input_accel) {
    float x_accel = static_cast<float>(input_accel.x) / ACCEL_NORMALIZE_CONSTANT;
    float y_accel = static_cast<float>(input_accel.y) / ACCEL_NORMALIZE_CONSTANT;
    return {x_accel, y_accel};
}

float calculate_distance(XYPairFloat a, XYPairFloat b)
{
    float res = sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
    return res;
}
