#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.hpp"

using namespace glm;

namespace example
{
	class DirLight
	{
	private:
		
		vec3 direction;
		vec3 ambient;
		vec3 diffuse;
		vec3 specular;

	public:

		DirLight(vec3 new_direction,vec3 new_ambient, vec3 new_diffuse, vec3 new_specular)
			:
			direction(new_direction),
			ambient(new_ambient),
			diffuse(new_diffuse),
			specular(new_specular)
			
		{

		}

		void set_uniforms(Shader &shader)
		{
			shader.set_uniform3f("dir_light.f_direction", direction.x, direction.y, direction.z);
			shader.set_uniform3f("dir_light.f_ambient", ambient.x,ambient.y,ambient.z);
			shader.set_uniform3f("dir_light.f_diffuse",diffuse.x,diffuse.y,diffuse.z );
			shader.set_uniform3f("dir_light.f_specular", specular.x,specular.y,specular.z);
		}


	};
}