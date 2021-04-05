#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.hpp"
#include <iostream>

using namespace glm;

namespace example
{
	class PointLight
	{
	private:

		vec3 position;
		vec3 ambient;
		vec3 diffuse;
		vec3 specular;

	public:

		PointLight(vec3 new_position, vec3 new_ambient, vec3 new_diffuse, vec3 new_specular)
			:
			position(new_position),
			ambient(new_ambient),
			diffuse(new_diffuse),
			specular(new_specular)

		{

		}

		void set_uniforms(Shader& shader)
		{
			shader.set_uniform3f("point_light.f_position", position.x, position.y, position.z);
			shader.set_uniform3f("point_light.f_ambient" , ambient.x , ambient.y ,  ambient.z);
			shader.set_uniform3f("point_light.f_diffuse" , diffuse.x , diffuse.y ,  diffuse.z);
			shader.set_uniform3f("point_light.f_specular", specular.x, specular.y, specular.z);
		}


	};
}