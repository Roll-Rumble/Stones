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

const XYPairFloat Ball::get_position() {
    return position_;
}


const XYPairFloat Ball::get_velocity() {
    return velocity_;
}

const float Ball::get_radius() {
	return radius_;
}

void Ball::update_velocity() {
    velocity_.x += accel_.x/FPS;
    velocity_.y += accel_.y/FPS;
}

void Ball::set_velocity(XYPairFloat velocity) {
	velocity_ = velocity;
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
        velocity_.y *= -1;
    }
    if (map.is_wall({position_.x, position_.y - radius_})) {
        velocity_.y *= -1;
    }
}

void Ball::resolve_ball_collision(Ball &ball1, Ball &ball2) {
    XYPairFloat ball1_position = ball1.get_position();
    XYPairFloat ball1_velocity = ball1.get_velocity();

    XYPairFloat ball2_position = ball2.get_position();
    XYPairFloat ball2_velocity = ball2.get_velocity();

	float distance = sqrt(pow(ball1_position.x - ball2_position.x, 2) + pow(ball1_position.y - ball2_position.y, 2));
	/*float dist_x;
	float dist_y;
	float impulse_x;
	float impulse_y;

	bool x1_bigger = 1;
	bool y1_bigger = 1;*/
	//float magnitude = sqrt(pow(ball1_position.x, 2) + pow(ball1_position.y, 2)) * sqrt(pow(ball2_position.x, 2) + pow(ball2_position.y, 2));
	if (distance < 2*ball1.get_radius()) { // we have a collision
		/*if ( ball1_position.x > ball2_position.x){
			dist_x = ball1_position.x - ball2_position.x;
		}
		else{
			dist_x = ball2_position.x - ball1_position.x;
			x1_bigger = 0;
		}
		if ( ball1_position.y > ball2_position.y){
			dist_y = ball1_position.y - ball2_position.y;
		}
		else{
			dist_y = ball2_position.y - ball1_position.y;
			y1_bigger = 0;
		}
		float magnitude = sqrt(pow(dist_x, 2) + pow(dist_y, 2));
		float cos_theta = (ball1_position.x * ball2_position.x + ball1_position.y * ball2_position.y)/magnitude;

		
		impulse_x = */

		float C1_C2x = ball1_position.x - ball2_position.x;
		float V1_V2x = ball1_velocity.x - ball2_velocity.x;
		float C1_C2y = ball1_position.y - ball2_position.y;
		float V1_V2y = ball1_velocity.y - ball2_velocity.y;
		//float 
		float dot_product = C1_C2x * V1_V2x + C1_C2y * V1_V2y;
		float magnitude_2 = pow(C1_C2x, 2) + pow(C1_C2y, 2);
		float ball1_velocity_x = ball1_velocity.x - (dot_product/magnitude_2) * C1_C2x;
		float ball1_velocity_y = ball1_velocity.y - (dot_product/magnitude_2) * C1_C2y;
		float ball2_velocity_x = ball2_velocity.x + (dot_product/magnitude_2) * C1_C2x;
		float ball2_velocity_y = ball2_velocity.y + (dot_product/magnitude_2) * C1_C2y;
		XYPairFloat ball1_velocity = {ball1_velocity_x, ball1_velocity_y};
		XYPairFloat ball2_velocity = {ball2_velocity_x, ball2_velocity_y};
		ball1.set_velocity(ball1_velocity);
		ball2.set_velocity(ball2_velocity);
	}

}
