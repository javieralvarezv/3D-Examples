#include "glad/glad.h"
#include "Shader.hpp"
#include <vector>
#include <memory>
#include <string>


namespace example
{
	class Skybox
	{
	private:

		unsigned int vbo_id;
		unsigned int vao_id;

		GLint model_view_matrix_id;
		GLint projection_matrix_id;

		unsigned int cubemap_texture;

		float scale;

	public: 

		Skybox();

		~Skybox();

		void render(Shader &shader);

		glm::mat4 calculate_skybox_view_matrix(glm::vec3 new_pos);
	
	private:

		unsigned int load_cubemap(std::vector <std::string> &faces);

	};
}