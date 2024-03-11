#include "map.hpp"

#include "game_util.hpp"
#include <cmath>

bool Map::is_wall(XYPairFloat coordinates) const {
    int tile_x_idx = static_cast<int>(coordinates.x / TILE_WIDTH);
    int tile_y_idx = static_cast<int>(MAP_HEIGHT - 1 - (coordinates.y / TILE_HEIGHT));
    return map_[tile_y_idx][tile_x_idx] == Tile::W;
}

TileQuadrant Map::get_tile_quadrant(XYPairFloat coordinates) const {
    // Calculate pixel position in tile relative to bottom left of tile
    float tile_x_pos = remainder(coordinates.x, TILE_WIDTH);
    float tile_y_pos = remainder(coordinates.y, TILE_HEIGHT);

    TileQuadrant quadrant;

    if (tile_x_pos > TILE_WIDTH/2) {
        if (tile_y_pos > TILE_HEIGHT/2) {
            quadrant = TileQuadrant::TOP_RIGHT;
        } else {
            quadrant = TileQuadrant::BOTTOM_RIGHT;
        }
    } else {
        if (tile_y_pos > TILE_HEIGHT/2) {
            quadrant = TileQuadrant::TOP_LEFT;
        } else {
            quadrant = TileQuadrant::BOTTOM_LEFT;
        }
    }

    return quadrant;
}

XYPairFloat Map::get_start_position() const {
    // Convert tile coordinates to pixel coordinates
    int start_pos_x = start_position_.x*TILE_WIDTH + TILE_WIDTH/2;
    int start_pos_y = SCREEN_HEIGHT - (start_position_.y*TILE_HEIGHT + TILE_HEIGHT/2);
    // Cast integer pixel coordinates to floats
    return {static_cast<float>(start_pos_x), static_cast<float>(start_pos_y)};
}
