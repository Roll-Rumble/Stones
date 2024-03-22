#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#ifdef CLIENT_COMPILE
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#include "game_util.hpp"
#include "shader.hpp"
#include "objects.hpp"

#ifdef CLIENT_COMPILE


Shader::Shader(const std::string& vertex_shader_fname, const std::string& fragment_shader_fname)
{
	std::ifstream shader_in {vertex_shader_fname};


	if (!shader_in) {
		std::cerr << "Can't open vertex shader file '" << vertex_shader_fname << "'" << std::endl;
		exit(1);
	}

	std::stringstream vertex_str_stream;

	vertex_str_stream << shader_in.rdbuf();
	std::string vertex_s = vertex_str_stream.str();

	shader_in.close();
	shader_in.open(fragment_shader_fname);
	if (!shader_in.is_open()) {
		std::cerr << "Can't open vertex shader file '" << fragment_shader_fname << "'" << std::endl;
		exit(1);
	}

	std::stringstream fragment_str_stream;
	fragment_str_stream << shader_in.rdbuf();
	std::string fragment_s = fragment_str_stream.str();
	shader_prog_ = CreateShader(vertex_s, fragment_s);
}

Shader::~Shader()
{
	glDeleteProgram(shader_prog_);
}

void Shader::Bind()
{
	glUseProgram(shader_prog_);
	float scale_trans_mat[16] = {
		2.0/SCREEN_WIDTH, 0.0, 0.0, 0.0,
		0.0, 2.0/SCREEN_HEIGHT, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0,
	};

	float sub_trans_vec[4] = {
		-1.0, -1.0, 0.0, 0.0,
	};

	SetUniformMatrix4fv(SCALE_TRANS_UNF, scale_trans_mat);
	SetUniform4fv(SUB_TRANS_UNF, sub_trans_vec);
}

void Shader::Unbind()
{
	glUseProgram(0);
}


unsigned int Shader::CreateShader(const std::string& vertex_shader, const std::string& fragment_shader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(vertex_shader, GL_VERTEX_SHADER);
	unsigned int fs = CompileShader(fragment_shader, GL_FRAGMENT_SHADER);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	// this is optional and will detach them from the program object
	// can also not Bind Detach to keep shader in there which might be
	// Bindful for debugging
	glDetachShader(program, vs);
	glDetachShader(program, fs);

	glValidateProgram(program);

	// can delete them now since the program has been built from them
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;

}

unsigned int Shader::CompileShader(const std::string& shader, unsigned int type)
{
	unsigned int id = glCreateShader(type);
	const char* src = shader.c_str();

	// 1 specified number of elements in string array (here &src) where
	// src is the only element
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// error handling
	int result;

	// iv specifies types
	// i: integer GL_COMPILE_STATUS
	// v: vector of results
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		// get error message
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		glGetShaderInfoLog(id, length, &length, message);

		std::cerr << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< std::endl;
		std::cerr << message << std::endl;
		delete[] message;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

void Shader::SetUniform1i(const std::string& name, int val)
{
	glUniform1i(GetUniformLocation(name), val);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniform4fv(const std::string& name, float* arr)
{
	glUniform4fv(GetUniformLocation(name), 1, arr);
}

void Shader::SetUniformMatrix4fv(const std::string& name, float* arr)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, arr);
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (uniform_location_cache_.find(name) != uniform_location_cache_.end()) {
		return uniform_location_cache_[name];
	}

	int location = glGetUniformLocation(shader_prog_, name.c_str());
	if (location == -1) {
		std::cerr << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	}

	uniform_location_cache_[name] = location;
	return location;

}

#endif
