#shader vertex
#version 330

uniform mat4 projection_matrix;
uniform mat4 camera_matrix;
uniform mat4 model_view_matrix;

layout (location = 0) in vec3 vertex_coordinates;

out vec3 texture_coordinates;

void main()
{
    texture_coordinates = vertex_coordinates;
    vec4 pos = projection_matrix * camera_matrix * model_view_matrix * vec4(vertex_coordinates, 1.0);
    gl_Position = pos.xyww;
};

#shader fragment
#version 330

in  vec3 texture_coordinates;
out vec4 fragment_color;

uniform samplerCube skybox;

void main()
{
   fragment_color = texture(skybox, texture_coordinates);
};