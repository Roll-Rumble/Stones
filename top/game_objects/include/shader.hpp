#ifdef CLIENT_COMPILE
#ifndef SHADER_H
#define SHADER_H

#include <string>

#define COLOR_UNF "col"
#define SCALE_TRANS_UNF "scale_trans"
#define SUB_TRANS_UNF "sub_trans"

class Shader
{
public:
	Shader(const std::string& vertex_shader, const std::string& fragment_shader);
	~Shader();

	void Use(float r, float g, float b, float a) const;
private:
	unsigned int shader_prog_;

	unsigned int CreateShader(const std::string& vertex_shader, const std::string& fragment_shader);
	unsigned int CompileShader(const std::string& shader, unsigned int type);
};

#endif // !SHADER_H
#endif