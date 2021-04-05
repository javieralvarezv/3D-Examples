#pragma once
#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "Shader.hpp"

using namespace glm;

namespace example
{
	const double DEG2RAD = 3.1415926f / 180.0;

	class Camera
	{
	private:

		vec3 camera_position;
		vec3 camera_direction;						
		vec3 camera_target;
		vec3 camera_up;
		vec3 camera_front;
		vec3 camera_right;
		
		mat4 view;
		
		float        angle;
		float     angle_up;
		float camera_speed;

		

	public:

		Camera();

		~Camera();

		inline void set_camera_speed(float v) { camera_speed = v; }
		inline mat4         get_camera_view() { return view; }
		inline vec3 get_camera_position() { return camera_position; }
		inline void set_view_pos(Shader& shader) { shader.set_uniform3f("viewPos" ,camera_position.x, camera_position.y, camera_position.z); }

		void look_right ();
		void look_left  ();
		void look_up    ();
		void look_down  ();
		
		void move_right ();
		void move_left  ();
		void move_front ();
		void move_back  ();
		void move_up    ();
		void move_down  ();

		void update();

	};
}