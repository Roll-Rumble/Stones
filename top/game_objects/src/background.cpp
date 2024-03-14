#include "background.hpp"
// These may have to go in CLIENT_COMPILE
#include "vertex_buffer_layout.hpp"
#include "vertex_array.hpp"
#include "vertex_buffer.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "game_util.hpp"

#ifdef CLIENT_COMPILE
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

Background::Background()
{
    #ifdef CLIENT_COMPILE
    glGenBuffers(1, &buffer_);
    #endif
}

void Background::draw(Shader &shader) const
{
    float positions[] = {
        0, 0, 0.0f, 0.0f,
        0, SCREEN_HEIGHT-1, 1.0f, 0.0f,
        SCREEN_WIDTH-1, SCREEN_HEIGHT-1, 1.0f, 1.0f,
        SCREEN_WIDTH-1, 0, 0.0f, 1.0f   
    };

    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    // this will already bind vertex array and buffer
    va.AddBuffer(vb, layout);
    shader.Bind();
    shader.SetUniform1i(TEX_UNF, BACKGROUND_SLOT);

    glDrawArrays(GL_QUADS, 0, 4);
    va.Unbind();
    vb.Unbind();
    shader.Unbind();
}
