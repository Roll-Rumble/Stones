#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ball.hpp"

#include "game_util.hpp"
#include "map.hpp"

Ball::Ball(const Map &map) : velocity_{0}, accel_{0}, radius_ {RADIUS}
{
    position_ = map.get_start_position();
    std::cout << position_.x << "," << position_.y << std::endl;
    glGenBuffers(1, &buffer_);
}

void Ball::set_position(XYPairFloat position)
{
    position_ = position;
}

void Ball::set_acceleration(XYPairFloat accel)
{
    accel_ = accel;
}

void Ball::update_velocity()
{
    velocity_.x += accel_.x/FPS;
    velocity_.y += accel_.y/FPS;
}

void Ball::update_position()
{
    position_.x += velocity_.x/FPS;
    position_.y += velocity_.y/FPS;
}

void Ball::resolve_wall_collisions(const Map& map)
{
    // If ball is far inside a block reverse its direction
    // and do no further calculations
    // if (map.is_wall(position_)) {
    //     velocity_.x *= -1;
    //     velocity_.y *= -1;
    //     return;
    // }

    // TileQuadrant quadrant = map.get_tile_quadrant(position_);

    // // Currently doesn't check tile diagonally adjacent
    // switch (quadrant) {
    // case TileQuadrant::TOP_LEFT:
    //     if (map.is_wall({position_.x - radius_, position_.y})) {
    //         velocity_.x *= -1;
    //     }
    //     if (map.is_wall({position_.x, position_.y + radius_})) {
    //         velocity_.y *= -1;
    //     }
    //     break;

    // case TileQuadrant::TOP_RIGHT:
    //     if (map.is_wall({position_.x + radius_, position_.y})) {
    //         velocity_.x *= -1;
    //     }
    //     if (map.is_wall({position_.x, position_.y + radius_})) {
    //         velocity_.y *= -1;
    //     }
    //     break;

    // case TileQuadrant::BOTTOM_LEFT:
    //     if (map.is_wall({position_.x - radius_, position_.y})) {
    //         velocity_.x *= -1;
    //     }
    //     if (map.is_wall({position_.x, position_.y - radius_})) {
    //         velocity_.y *= -1;
    //     }
    //     break;

    // case TileQuadrant::BOTTOM_RIGHT:
    //     if (map.is_wall({position_.x + radius_, position_.y})) {
    //         velocity_.x *= -1;
    //     }
    //     if (map.is_wall({position_.x, position_.y - radius_})) {
    //         velocity_.y *= -1;
    //     }
    //     break;

    // default:
    //     std::cerr << "Unrecognised tile quadrant\n";
    //     exit(1);
    // }

    if (map.is_wall({position_.x + radius_, position_.y})) {
        velocity_.x *= -1;
    }
    if (map.is_wall({position_.x - radius_, position_.y})) {
        velocity_.x *= -1;
    }
    if (map.is_wall({position_.x, position_.y + radius_})) {
        velocity_.y *= -1;
    }
    if (map.is_wall({position_.x, position_.y - radius_})) {
        velocity_.y *= -1;
    }
}

XYPairFloat Ball::get_circle_pos(float angle) const
{
    XYPairFloat pos;
    pos.x = radius_ * cosf(angle);
    pos.y = radius_ * sinf(angle);
    return pos;
}

void Ball::draw(const Shader &shader) const
{
    shader.Use(1.0, 0.0, 0.0, 1.0);
    float positions[6];

    float x_off, y_off;

    for (int i = 0; i < CIRCLE_SEGS; i++)
    {
        auto [x_off_0, y_off_0] = get_circle_pos(2.0f * PI * float(i) / float(CIRCLE_SEGS));

        positions[0] = position_.x + x_off_0;
        positions[1] = position_.y + y_off_0;

        auto [x_off_1, y_off_1] = get_circle_pos(2.0f * PI * float(i + 1) / float(CIRCLE_SEGS));
        positions[2] = position_.x + x_off_1;
        positions[3] = position_.y + y_off_1;
        positions[4] = position_.x;
        positions[5] = position_.y;

        glBindBuffer(GL_ARRAY_BUFFER, buffer_);
        glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 2, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    


}