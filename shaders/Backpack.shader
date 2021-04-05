#shader vertex
#version 330

uniform mat4 projection_matrix;
uniform mat4 camera_matrix;
uniform mat4 model_view_matrix;

layout(location = 0) in vec3  vertex_coordinates;
layout(location = 1) in vec3 normals_coordinates;
layout(location = 2) in vec2 texture_coordinates;

out vec3 front;
out vec3 normal;
out vec2 v_TextCoord;

void main()
{
    front = vertex_coordinates;
    normal = normals_coordinates;
    v_TextCoord = texture_coordinates;

    gl_Position = projection_matrix * camera_matrix * model_view_matrix * vec4(vertex_coordinates, 1.0);

};

#shader fragment
#version 330

in vec3 front;
in vec2 v_TextCoord;
in vec3 normal;

out vec4 fragment_color;

uniform sampler2D texture_diffuse1;

void main()
{
    vec4 texColor = texture(texture_diffuse1, v_TextCoord);
    fragment_color = texColor;
};