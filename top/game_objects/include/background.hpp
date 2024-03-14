#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP
#include "shader.hpp"
class Background
{
public:
    Background();

#ifdef CLIENT_COMPILE
    void draw(Shader &shader) const;
#endif
};

#endif