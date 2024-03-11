#include "ball.hpp"

#include "game_util.hpp"
#include "map.hpp"
#include <iostream>

Ball::Ball(const Map &map) : velocity_{0}, accel_{0} {
    position_ = map.get_start_position();
}

void Ball::set_position(XYPairFloat position) {
    position_ = position;
}

void Ball::set_acceleration(XYPairFloat accel) {
    accel_ = accel;
}

void Ball::update_velocity() {
    velocity_.x += accel_.x/FPS;
    velocity_.y += accel_.y/FPS;
}

void Ball::update_position() {
    position_.x += velocity_.x/FPS;
    position_.y += velocity_.y/FPS;
}

void Ball::resolve_wall_collisions(const Map& map) {
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
        velocity_.x *= -1;
    }
    if (map.is_wall({position_.x, position_.y - radius_})) {
        velocity_.x *= -1;
    }
}
