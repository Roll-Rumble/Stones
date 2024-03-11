#ifndef BALL_HPP
#define BALL_HPP

#include "game_util.hpp"
#include "Map.hpp"

// Radius of balls in pixels
#define RADIUS 25f

class Ball {
public:
    Ball(const Map &map);
    ~Ball() {}

    // Setters for position and acceleration
    void set_position(XYPairFloat position);
    void set_acceleration(XYPairFloat accel);

    // Uses current acceleration to update velocity
    void update_velocity();
    // Uses current velocity to update position
    void update_position();
    // Uses current position to resolve collisions and update velocity
    void resolve_wall_collisions(const Map &map);

    /* Order of function calls in frame calculation:
     * 1. set_accleration
     * 2. update_velocity
     * 2. update_position
     * 3. resolve_wall_collisions
     */

private:
    float radius_;          // Measured in pixels
    XYPairFloat position_;  // Pixel coordintes of centre of ball
    XYPairFloat velocity_;  // Measured in pixels/s
    XYPairFloat accel_;     // Measured in pixels/s^2
};

#endif