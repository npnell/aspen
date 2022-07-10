#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int ID;

	Shader(const char* compute_path) {
		std::string compute_code;
		std::ifstream c_shader_file;

		c_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			c_shader_file.open(compute_path);
			std::stringstream c_shaderstream;
			c_shaderstream << c_shader_file.rdbuf();
			c_shader_file.close();

			compute_code = c_shaderstream.str();
		}
		catch (std::ifstream::failure& e) {
			std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
		}
		const char* c_shader_code = compute_code.c_str();

		unsigned int compute;
		// Compute shader
		compute = glCreateShader(GL_COMPUTE_SHADER);
		glShaderSource(compute, 1, &c_shader_code, NULL);
		glCompileShader(compute);
		check_compile_errors(compute, "COMPUTE");
		// Shader Program
		ID = glCreateProgram();
		glAttachShader(ID, compute);
		glLinkProgram(ID);
		check_compile_errors(ID, "PROGRAM");

		glDeleteShader(compute);
	}

	inline void use() const;

	inline void set_bool(const std::string& name, bool value) const;

	inline void set_int(const std::string& name, int value) const;

	inline void set_float(const std::string& name, float value) const;

private:
	void check_compile_errors(GLuint shader, std::string type);
};

// activate the shader
// ------------------------------------------------------------------------
inline void Shader::use() const
{
    glUseProgram(ID);
}
// utility uniform functions
// ------------------------------------------------------------------------
inline void Shader::set_bool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
inline void Shader::set_int(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
inline void Shader::set_float(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::check_compile_errors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cerr << "ERROR::SHADER_COMPILATION_ERROR::" << type << "\n" << infoLog << std::endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cerr << "ERROR::PROGRAM_LINKING_ERROR::" << type << "\n" << infoLog << std::endl;
		}
	}
}

#endif // !SHADER_H
