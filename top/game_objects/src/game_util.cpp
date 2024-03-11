#include "game_util.hpp"

XYPairFloat normalize_accel(XYPairInt16 input_accel) {
    float x_accel = static_cast<float>(input_accel.x) / ACCEL_NORMALIZE_CONSTANT;
    float y_accel = static_cast<float>(input_accel.y) / ACCEL_NORMALIZE_CONSTANT;
    return {x_accel, y_accel};
}
