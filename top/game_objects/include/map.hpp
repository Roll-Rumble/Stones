#ifndef MAP_HPP
#define MAP_HPP

#include "game_util.hpp"
#include "shader.hpp"
#include <array>

#define MAP_WIDTH 32    // Measured in tiles
#define MAP_HEIGHT 18   // Measured in tiles

#define TILE_WIDTH (SCREEN_WIDTH / MAP_WIDTH)       // Measured in pixels
#define TILE_HEIGHT (SCREEN_HEIGHT / MAP_HEIGHT)    // Measured in pixels

enum class Tile {
    W,  // Wall
    _,  // Floor
    I,  // Entrance
    O   // Exit
};

// Refers to 4 quadrants of a single tile
enum class TileQuadrant {
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT
};

class Map {
public:
    Map();
    ~Map() {}

    // Returns tile type at given coordinates
    bool is_wall(XYPairFloat coordinates) const;

    // Returns coordinates of centre of tile input coordinates are in
    XYPairFloat tile_centre(XYPairFloat coordinates) const;

    // Returns which quadrant of a tile coordinates are in
    TileQuadrant get_tile_quadrant(XYPairFloat coordinates) const;

    // Returns start position for centre of ball as float pixel coordinates
    XYPairFloat get_start_position() const;


    void draw(const Shader &shader) const;

private:
    unsigned int buffer_;
    // Zero-indexed tile relative to top left of map
    XYPairInt16 start_position_ = {2,2};

    using enum Tile;	// Allows map display below with strongly typed enums
    const std::array<std::array<Tile,MAP_WIDTH>,MAP_HEIGHT> map_ = {{
        {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
        {W,_,_,_,W,W,W,_,_,W,W,_,_,W,W,_,_,W,W,_,_,_,W,W,W,W,W,W,W,W,W,W},
        {W,_,I,_,W,O,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,W,W,W,W,W,W,W,W},
        {W,_,_,_,W,W,W,W,W,_,_,W,W,_,_,W,W,_,_,W,W,W,_,_,_,W,W,W,W,W,W,W},
        {W,_,_,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,_,_,_,W,W,W,W,W,W,W},
        {W,_,_,_,_,_,_,_,_,_,_,_,_,_,_,W,W,W,W,W,W,W,_,_,_,W,W,W,W,W,W,W},
        {W,_,_,_,_,_,_,_,_,_,_,_,_,_,_,W,W,W,W,W,_,_,_,_,_,_,W,W,W,W,W,W},
        {W,W,W,W,W,_,_,W,W,W,W,W,W,W,W,W,W,W,W,_,_,_,W,W,_,_,_,W,W,W,W,W},
        {W,W,W,W,W,_,_,_,_,_,_,_,W,W,W,W,W,W,W,_,_,_,W,W,W,_,_,_,W,W,W,W},
        {W,W,W,W,_,_,_,W,W,W,_,_,W,W,W,W,W,_,_,_,W,W,W,W,W,W,_,_,_,W,W,W},
        {W,W,W,_,_,_,W,W,W,W,W,_,_,W,W,W,W,_,_,_,W,W,W,W,W,W,W,_,_,W,W,W},
        {W,W,_,_,_,W,W,W,W,W,W,W,_,_,W,W,W,W,W,_,_,_,_,_,W,W,W,_,_,W,W,W},
        {W,W,_,_,W,W,W,W,W,W,W,W,_,_,_,W,W,W,W,_,_,_,_,_,W,W,W,_,_,W,W,W},
        {W,W,W,_,_,_,_,_,_,_,_,_,_,_,_,W,W,W,W,_,_,_,_,_,_,_,_,_,_,_,_,W},
        {W,W,W,W,_,_,_,_,_,_,_,_,W,W,W,W,W,W,W,_,_,_,_,_,_,_,_,_,_,_,_,W},
        {W,W,W,W,W,W,W,_,_,W,_,_,W,_,_,_,_,W,W,_,_,_,_,_,W,W,W,W,W,W,W,W},
        {W,W,W,W,W,W,W,_,_,W,_,_,_,_,W,W,_,_,_,_,W,W,W,W,W,W,W,W,W,W,W,W},
        {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W}
    }};
};

/* Template for creating additional maps

{{
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
}};
*/

#endif
