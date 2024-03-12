#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "game_util.hpp"


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

void Shader::Use(float r, float g, float b, float a) const
{
	int vertexColorLocation = glGetUniformLocation(shader_prog_, COLOR_UNF);
	int scaleTransformLocation = glGetUniformLocation(shader_prog_, SCALE_TRANS_UNF);
	int subTransformLocation = glGetUniformLocation(shader_prog_, SUB_TRANS_UNF);
	glUseProgram(shader_prog_);
	glUniform4f(vertexColorLocation, r, g, b, a);
	float scale_trans_mat[16] = {
		2.0/SCREEN_WIDTH, 0.0, 0.0, 0.0,
		0.0, 2.0/SCREEN_HEIGHT, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0,
	};

	float sub_trans_vec[4] = {
		-1.0, -1.0, 0.0, 0.0,
	};
	
	glUniformMatrix4fv(scaleTransformLocation, 1, GL_FALSE, scale_trans_mat);
	glUniform4fv(subTransformLocation, 1, sub_trans_vec);
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
	// can also not use Detach to keep shader in there which might be
	// useful for debugging
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