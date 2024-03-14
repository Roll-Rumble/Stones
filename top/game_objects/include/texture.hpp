#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

#define WALL_SLOT 0
#define FLOOR_SLOT 1
#define HOLE_SLOT 2
#define ENTRANCE_SLOT 3
#define EXIT_SLOT 4
#define MY_BALL_SLOT 5
#define ENEMY_BALL_SLOT 6
#define BACKGROUND_SLOT 7

class Texture
{
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

private:
	unsigned int renderer_id_;
	std::string file_path_;
	unsigned char* local_buffer_;
	int width_, height_, bpp_;
};

#endif