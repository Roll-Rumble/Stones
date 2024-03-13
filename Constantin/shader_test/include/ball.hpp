#ifndef BALL_HPP
#define BALL_HPP

#include "shader.hpp"
#include "game_util.hpp"
#include "map.hpp"

#define CIRCLE_SEGS 100

// Radius of balls in pixels
#define RADIUS 25.0f
#define PI 3.1415926f

class Ball {
public:
    Ball(const Map &map);
    ~Ball() {}

    // Setters for position and acceleration
    void set_position(XYPairFloat position);
    void set_acceleration(XYPairFloat accel);

    // Binds current acceleration to update velocity
    void update_velocity();
    // Binds current velocity to update position
    void update_position();
    // Binds current position to resolve collisions and update velocity
    void resolve_wall_collisions(const Map &map);

    void draw(Shader &shader) const;

    /* Order of function calls in frame calculation:
     * 1. set_accleration
     * 2. update_velocity
     * 2. update_position
     * 3. resolve_wall_collisions
     */

private:
    unsigned int buffer_;
    float radius_;          // Measured in pixels
    XYPairFloat position_;  // Pixel coordintes of centre of ball
    XYPairFloat velocity_;  // Measured in pixels/s
    XYPairFloat accel_;     // Measured in pixels/s^2
    XYPairFloat get_circle_pos(float angle) const;
};

#endif
