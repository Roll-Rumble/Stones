#ifdef CLIENT_COMPILE
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif
#include <cmath>

// These may have to go in CLIENT_COMPILE
#include "vertex_buffer_layout.hpp"
#include "vertex_array.hpp"
#include "vertex_buffer.hpp"
#include "texture.hpp"

#include "shader.hpp"
#include "map.hpp"

#include "game_util.hpp"
#include "shader.hpp"
#include <iostream>


Map::Map()
{
    #ifdef CLIENT_COMPILE
    glGenBuffers(1, &buffer_);
    #endif
}

bool Map::is_wall(XYPairFloat coordinates) const
{
    int tile_x_idx = static_cast<int>(coordinates.x / TILE_WIDTH);
    int tile_y_idx = static_cast<int>(MAP_HEIGHT - (coordinates.y / TILE_HEIGHT));
    return map_[tile_y_idx][tile_x_idx] == Tile::W;
}

bool Map::is_hole(XYPairFloat coordinates) const
{
    int tile_x_idx = static_cast<int>(coordinates.x / TILE_WIDTH);
    int tile_y_idx = static_cast<int>(MAP_HEIGHT - (coordinates.y / TILE_HEIGHT));
    return map_[tile_y_idx][tile_x_idx] == Tile::H;
}

/* this function was made to mimic is_wall design to achieve consistency
do not change! */
XYPairFloat Map::tile_centre(XYPairFloat coordinates) const
{
    int tile_x_idx = static_cast<int>(coordinates.x / TILE_WIDTH);
    int tile_y_idx = static_cast<int>(MAP_HEIGHT - (coordinates.y / TILE_HEIGHT));
    float x_coord = tile_x_idx * TILE_WIDTH + TILE_WIDTH / 2;
    float y_coord = SCREEN_HEIGHT - (tile_y_idx * TILE_HEIGHT + TILE_HEIGHT / 2);
    return { x_coord, y_coord };
}

TileQuadrant Map::get_tile_quadrant(XYPairFloat coordinates) const
{
    // Calculate pixel position in tile relative to bottom left of tile
    float tile_x_pos = std::fmod(coordinates.x, TILE_WIDTH);
    float tile_y_pos = std::fmod(coordinates.y, TILE_HEIGHT);


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

XYPairFloat Map::get_start_position() const
{
    // Convert tile coordinates to pixel coordinates
    int start_pos_x = start_position_.x*TILE_WIDTH + TILE_WIDTH/2;
    int start_pos_y = SCREEN_HEIGHT - (start_position_.y*TILE_HEIGHT + TILE_HEIGHT/2);
    // Cast integer pixel coordinates to floats
    return {static_cast<float>(start_pos_x), static_cast<float>(start_pos_y)};
}

#ifdef CLIENT_COMPILE
void Map::draw(Shader &shader) const
{
    for (int row = 0; row < map_.size(); row++) {
        for (int col = 0; col < map_[row].size(); col++) {
            int row_conv = MAP_HEIGHT - 1 - row;
            float positions[] = {
                col * TILE_WIDTH, row_conv* TILE_HEIGHT, 0.0f, 0.0f,
                col * TILE_WIDTH, (row_conv + 1) * TILE_HEIGHT, 1.0f, 0.0f,
                (col + 1) * TILE_WIDTH, (row_conv + 1) * TILE_HEIGHT, 1.0f, 1.0f,
                (col + 1) * TILE_WIDTH, row_conv * TILE_HEIGHT, 0.0f, 1.0f,
            };
            VertexArray va;
            VertexBuffer vb(positions, 4 * 4 * sizeof(float));
            VertexBufferLayout layout;
            layout.Push<float>(2);
            layout.Push<float>(2);
            // this will already bind vertex array and buffer
            va.AddBuffer(vb, layout);
            shader.Bind();

            switch (map_[row][col]) {
            case Tile::W:
                shader.SetUniform1i(TEX_UNF, WALL_SLOT);
                break;
            case Tile::_:
                shader.SetUniform1i(TEX_UNF, FLOOR_SLOT);
                break;
            case Tile::I:
                shader.SetUniform1i(TEX_UNF, ENTRANCE_SLOT);
                break;
            case Tile::O:
                shader.SetUniform1i(TEX_UNF, EXIT_SLOT);
                break;
            case Tile::H:
                shader.SetUniform1i(TEX_UNF, HOLE_SLOT);
                break;
            }

            glDrawArrays(GL_QUADS, 0, 4);
            va.Unbind();
            vb.Unbind();
            shader.Unbind();
        }
    }
}
#endif
