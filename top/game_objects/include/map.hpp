#ifndef MAP_HPP
#define MAP_HPP

#include "game_util.hpp"
#include "shader.hpp"
#include <array>

#define MAP_WIDTH 32    // Measured in tiles
#define MAP_HEIGHT 18   // Measured in tiles

#define HOLE_RADIUS 30 //Measured in pixels

#define TILE_WIDTH (SCREEN_WIDTH / MAP_WIDTH)       // Measured in pixels
#define TILE_HEIGHT (SCREEN_HEIGHT / MAP_HEIGHT)    // Measured in pixels

enum class Tile {
    W,  // Wall
    _,  // Floor
    I,  // Entrance
    O,  // Exit
    H   // Hole
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

    // Returns whether tile is a hole
    bool is_hole(XYPairFloat coordinates) const;

    // Returns coordinates of centre of tile input coordinates are in
    XYPairFloat tile_centre(XYPairFloat coordinates) const;

    // Returns which quadrant of a tile coordinates are in
    TileQuadrant get_tile_quadrant(XYPairFloat coordinates) const;

    // Returns start position for centre of ball as float pixel coordinates
    XYPairFloat get_start_position() const;


#ifdef CLIENT_COMPILE
    void draw(Shader &shader) const;
#endif

private:
    unsigned int buffer_;
    // Zero-indexed tile relative to top left of map
    XYPairInt16 start_position_ = {16,2};

    using enum Tile;	// Allows map display below with strongly typed enums
    /*const std::array<std::array<Tile,MAP_WIDTH>,MAP_HEIGHT> map_ = {{
        {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
        {W,_,_,_,W,W,W,_,_,W,W,_,_,W,W,_,_,W,W,_,_,_,W,W,W,W,W,W,W,W,W,W},
        {W,_,I,_,W,O,_,_,_,_,H,_,_,_,_,_,_,_,_,_,_,_,_,_,W,W,W,W,W,W,W,W},
        {W,_,H,_,W,W,W,W,W,_,_,_,W,_,_,W,W,_,_,W,W,W,_,_,_,W,W,W,W,W,W,W},
        {W,_,_,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,_,H,_,W,W,W,W,W,W,W},
        {W,_,_,H,_,_,_,_,_,_,_,_,_,_,_,W,W,W,W,W,W,W,_,H,_,W,W,W,W,W,W,W},
        {W,_,H,_,_,_,_,_,_,_,_,_,_,_,_,W,W,W,W,W,_,_,_,_,_,H,W,W,W,W,W,W},
        {W,W,W,W,W,_,_,W,W,W,W,W,W,W,W,W,W,W,W,_,H,_,W,W,_,_,_,W,W,W,W,W},
        {W,W,W,W,W,_,_,_,_,_,_,_,W,W,W,W,W,W,W,_,_,_,W,W,W,_,_,_,W,W,W,W},
        {W,W,W,W,_,_,_,W,W,W,_,_,W,W,W,W,W,_,H,_,W,W,W,W,W,W,_,_,_,W,W,W},
        {W,W,W,_,_,_,W,W,W,W,W,_,_,W,W,W,W,_,_,_,W,W,W,W,W,W,W,_,_,W,W,W},
        {W,W,_,_,H,W,W,W,W,W,W,W,_,_,W,W,W,W,W,_,_,_,_,_,W,W,W,_,_,W,W,W},
        {W,W,_,_,W,W,W,W,W,W,W,W,_,H,_,W,W,W,W,_,_,_,_,_,W,W,W,_,H,W,W,W},
        {W,W,W,_,_,_,_,_,_,_,_,_,_,_,_,W,W,W,W,_,H,_,_,_,_,_,_,_,_,_,_,W},
        {W,W,W,W,_,_,_,_,H,_,_,_,W,W,W,W,W,W,W,_,_,_,_,_,_,_,_,_,_,_,_,W},
        {W,W,W,W,W,W,W,_,_,W,_,_,W,_,_,_,_,W,W,_,_,_,H,_,W,W,W,W,W,W,W,W},
        {W,W,W,W,W,W,W,_,_,W,_,_,_,_,W,W,_,_,_,_,W,W,W,W,W,W,W,W,W,W,W,W},
        {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W}
    }};
};*/
// W - wall; _ - normal; H - hole
const std::array<std::array<Tile, MAP_WIDTH>, MAP_HEIGHT> map_ = { {
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,_,_,_,W,_,_,_,_,_,_,_,_,_,_,W,W,W,_,_,_,_,W,_,_,_,_,_,W},
    {W,W,W,_,_,W,_,_,_,W,W,W,W,W,W,_,I,_,W,W,W,_,H,H,_,W,_,H,_,H,_,W},
    {W,_,_,_,H,W,H,W,H,W,_,_,_,H,W,_,_,_,W,W,_,_,W,_,_,W,_,H,O,H,_,W},
    {W,_,W,W,W,W,_,_,_,_,_,W,_,W,W,W,W,W,W,W,_,_,W,_,_,W,_,H,H,H,_,W},
    {W,_,_,_,W,W,_,W,H,W,W,W,_,_,W,H,_,_,_,_,_,W,H,_,W,W,_,_,_,_,_,W},
    {W,W,W,_,W,_,_,_,_,W,W,H,W,_,W,_,_,_,W,W,W,W,W,_,W,W,W,W,_,W,W,W},
    {W,_,_,_,W,_,_,_,H,H,W,_,_,_,W,W,W,_,W,H,W,W,W,_,_,_,_,_,_,W,W,W},
    {W,_,W,W,W,H,_,_,_,_,W,_,_,_,W,H,_,_,_,_,W,W,W,_,_,_,_,_,_,W,W,W},
    {W,_,_,_,H,_,H,W,_,_,W,_,W,_,H,_,H,W,W,W,W,W,W,W,W,H,W,H,W,W,W,W},
    {W,H,W,_,W,_,W,W,_,_,W,_,_,_,W,_,_,_,_,_,W,W,W,W,W,_,_,_,_,W,W,W},
    {W,_,_,_,W,_,W,_,_,_,W,W,W,_,W,W,W,W,W,_,_,H,W,W,W,_,W,W,_,W,W,W},
    {W,_,W,W,H,_,_,_,_,_,W,_,_,_,H,W,W,H,W,W,_,_,_,_,_,_,W,W,_,H,_,W},
    {W,_,H,W,W,_,H,H,W,W,W,H,_,W,W,H,_,_,_,W,W,_,_,W,W,H,W,W,H,_,_,W},
    {W,H,_,W,W,_,_,W,W,W,_,_,_,W,W,_,_,H,_,_,W,W,W,W,_,_,_,_,_,_,H,W},
    {W,_,H,W,_,_,_,W,W,W,_,W,H,W,_,_,W,W,W,_,_,W,W,W,_,W,_,W,_,_,_,W},
    {W,_,_,_,_,W,H,H,_,_,_,_,_,_,_,_,_,H,_,_,_,_,_,_,_,W,_,_,H,_,_,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    } };
};

/* Template for creating additional maps

{{
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,W},
    {W,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,W},
    {W,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,W},
    {W,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,W},
    {W,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,W},
    {W,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,W},
    {W,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,W},
    {W,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,W},
    {W,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,W},
    {W,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,W},
    {W,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,W},
    {W,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,W},
    {W,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,W},
    {W,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,W},
    {W,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,W},
    {W,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
}};
*/

#endif
