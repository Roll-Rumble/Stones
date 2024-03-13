#ifdef CLIENT_COMPILE
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#include <string>

#include "texture.hpp"
#include "stb_image.h"

#ifdef CLIENT_COMPILE

Texture::Texture(const std::string& path)
	: renderer_id_(0), file_path_(path), local_buffer_(nullptr),
	width_(0), height_(0), bpp_(0)
{
	stbi_set_flip_vertically_on_load(1);

	// 4 channels for rgba
	local_buffer_ = stbi_load(path.c_str(), &width_, &height_, &bpp_, 4);

	glGenTextures(1, &renderer_id_);
	glBindTexture(GL_TEXTURE_2D, renderer_id_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// internal format: GL_RGBA8
	// source format: GL_RGBA
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, local_buffer_);

	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
	if (local_buffer_) {
		stbi_image_free(local_buffer_);
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &renderer_id_);
}

void Texture::Bind(unsigned int slot /* = 0 */) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, renderer_id_);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

#endif
