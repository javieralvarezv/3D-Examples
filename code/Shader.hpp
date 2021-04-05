#pragma once
#include <glm/mat4x4.hpp>
#include <string>
#include <unordered_map>

namespace example
{
	struct shader_program_source
	{
		std::string vertex_source;
		std::string fragment_source;
	};

	class Shader
	{
	
	private:

		std::string filepath;
		unsigned int renderer_id;
	
		std::unordered_map< std::string, int > uniform_location_cache;

	public:

		Shader(const std::string& filepath);

		~Shader();

		void   bind() const;
		void unbind() const;

		//Set uniforms
		void set_uniform1i(const std::string& name,   int value);
		void set_uniform1f(const std::string& name, float value);
		void set_uniform3f(const std::string& name, float v0, float v1, float v2);
		void set_uniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void set_uniform_mat4(const std::string& name, glm::mat4& matrix);

		inline unsigned int get_renderer_id() { return renderer_id; }

	private: 
		
		shader_program_source parse_shader(const std::string& filepath);
		unsigned int compile_shader(unsigned int type, const std::string& source);
		unsigned int create_shaders(const std::string& vertex_shader, const std::string fragment_shader);
		
		int get_uniform_location(const std::string& name);

	};
}