#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext.hpp"
#include <iostream>

example::Camera::Camera()
	:
	camera_position(0.f,5.f,6.f),
	camera_direction(0.f ,0.f, -1.f),
	camera_target(0.f ,0.f, 0.f),
	camera_up(0.f, 1.f, 0.f),
	camera_front(0.f, 0.f, -1.f),
	camera_right(1.f,0.f,0.f),
	view(),
	camera_speed(0.3f),
	angle(1.57f),
	angle_up(1.57f)
{
	view = glm::lookAt(camera_position, camera_position+camera_direction, camera_up);
}

example::Camera::~Camera()
{
}

void example::Camera::look_right()
{
	angle -= (0.035f);
	camera_direction.x =  cosf(angle);
	camera_direction.z = -sinf(angle);
}

void example::Camera::look_left()
{
	angle += (0.035f);
	camera_direction.x =  cosf(angle);
	camera_direction.z = -sinf(angle);
}

void example::Camera::look_up()
{
	angle_up -= (0.035f);
	camera_direction.y = cosf(angle_up);
	camera_direction = glm::normalize(camera_direction);
}

void example::Camera::look_down()
{
	angle_up += (0.035f);
	camera_direction.y = cosf(angle_up);
	camera_direction = glm::normalize(camera_direction);
}

void example::Camera::move_right()
{
	camera_right = glm::cross(camera_direction, camera_up);

	camera_position += camera_right * camera_speed;

}

void example::Camera::move_left()
{
	camera_right = glm::cross(camera_direction, camera_up);

	camera_position += -camera_right * camera_speed;
}

void example::Camera::move_front()
{
	camera_position += camera_direction * camera_speed;
}

void example::Camera::move_back()
{
	camera_position += -camera_direction * camera_speed;
}

void example::Camera::move_up()
{
	camera_position.y += camera_speed;
}

void example::Camera::move_down()
{
	camera_position.y -= camera_speed;
}

void example::Camera::update()
{
	view = glm::lookAt(camera_position, camera_position + camera_direction, camera_up);
}
