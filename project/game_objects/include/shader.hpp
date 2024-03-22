#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <unordered_map>

#define COLOR_UNF "col_u"
#define SCALE_TRANS_UNF "scale_trans_u"
#define SUB_TRANS_UNF "sub_trans_u"
#define TEX_UNF "tex_slot_u"

class Shader
{
public:
	Shader(const std::string& vertex_shader, const std::string& fragment_shader);
	~Shader();

	void Bind();
	void Unbind();

	void SetUniform1i(const std::string& name, int value);
	void SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
	void SetUniform4fv(const std::string &name, float *arr);
	void SetUniformMatrix4fv(const std::string& name, float* arr);
private:
	unsigned int shader_prog_;
	std::unordered_map<std::string, int> uniform_location_cache_;

	unsigned int CreateShader(const std::string& vertex_shader, const std::string& fragment_shader);
	unsigned int CompileShader(const std::string& shader, unsigned int type);

	int GetUniformLocation(const std::string& name);
};

#endif // !SHADER_H