#include <glad/glad.h>
#include "Shader.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>   

namespace example
{

	Shader::Shader(const std::string& filepath)
		: filepath(filepath), renderer_id(0)
	{
		shader_program_source source = parse_shader(filepath);
		renderer_id = create_shaders(source.vertex_source, source.fragment_source);
	}
	
	Shader::~Shader()
	{
		glDeleteProgram(renderer_id);
	}

	shader_program_source Shader::parse_shader(const std::string& filepath)
	{
		std::ifstream stream(filepath);

		enum class shader_type
		{
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};

		std::string line;
		std::stringstream ss[2];
		shader_type type = shader_type::NONE;

		while (getline(stream, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
					type = shader_type::VERTEX;
				else if (line.find("fragment") != std::string::npos)
					type = shader_type::FRAGMENT;
			}
			else
			{
				ss[(int)type] << line << '\n';
			}
		}

		return { ss[0].str(), ss[1].str() };
	}

	unsigned int Shader::create_shaders(const std::string& vertex_shader, const std::string fragment_shader)
	{
		unsigned int program = glCreateProgram();
		unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
		unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}


	unsigned int Shader::compile_shader(unsigned int type, const std::string& source)
	{
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)_malloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);

			std::cout << "Fallo de compilacion " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
			std::cout << message << std::endl;

			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	void Shader::bind() const
	{
		glUseProgram(renderer_id);
	}

	void Shader::unbind() const
	{
		glUseProgram(0);
	}
	
	void Shader::set_uniform1i(const std::string& name, int value)
	{
		glUniform1i(get_uniform_location(name), value);
	}

	void Shader::set_uniform1f(const std::string& name, float value)
	{
		glUniform1f(get_uniform_location(name), value);
	}

	void Shader::set_uniform3f(const std::string& name, float v0, float v1, float v2)
	{
		glUniform3f(get_uniform_location(name), v0, v1, v2);
	}

	void Shader::set_uniform4f(const std::string& name, float v0, float v1, float v2, float v3)
	{
		glUniform4f(get_uniform_location(name), v0, v1, v2, v3);
	}

	void Shader::set_uniform_mat4(const std::string& name, glm::mat4& matrix)
	{
		glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(matrix));
	}
	
	int Shader::get_uniform_location(const std::string& name)
	{
		if (uniform_location_cache.find(name) != uniform_location_cache.end())
			return uniform_location_cache[name];

		int location = glGetUniformLocation(renderer_id, name.c_str());
		
		if (location == -1)
		{
			std::cout << "Warning: uniform " << name << " no existe" << std::endl;
		}
		
		uniform_location_cache[name] = location;
		
		return location;
	}
}