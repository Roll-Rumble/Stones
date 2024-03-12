#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include "shader.hpp"
#include "map.hpp"

#include "game_util.hpp"

Map::Map()
{
    glGenBuffers(1, &buffer_);
}
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

void Map::draw(const Shader &shader) const
{
    for (int row = 0; row < map_.size(); row++) {
        for (int col = 0; col < map_[row].size(); col++) {
            int row_conv = MAP_HEIGHT - 1 - row;
            float positions[8] = {
                col * TILE_WIDTH, row_conv* TILE_HEIGHT,
                col * TILE_WIDTH, (row_conv + 1) * TILE_HEIGHT,
                (col + 1) * TILE_WIDTH, (row_conv + 1) * TILE_HEIGHT,
                (col + 1) * TILE_WIDTH, row_conv * TILE_HEIGHT,
            };
            switch (map_[row][col]) {
            case Tile::W:
                shader.Use(0.6, 0.6, 0.6, 1.0);
                break;
            case Tile::_:
                shader.Use(0.0, 0.0, 0.0, 1.0);
                break;
            case Tile::I:
                shader.Use(0.2, 0.2, 0.2, 1.0);
                break;
            case Tile::O:
                shader.Use(0.0, 0.7, 0.0, 1.0);
                break;
            }
            glBindBuffer(GL_ARRAY_BUFFER, buffer_);
            glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_DYNAMIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 2, 0);


            glDrawArrays(GL_QUADS, 0, 4);
        }
    }
}