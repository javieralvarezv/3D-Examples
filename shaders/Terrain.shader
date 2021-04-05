#shader vertex
#version 330

uniform mat4 model_view_matrix;
uniform mat4 projection_matrix;
uniform mat4 camera_matrix;

uniform float t;

layout(location = 0) in vec3  vertex_coordinates;
layout(location = 1) in vec3 normals_coordinates;

out vec3 front;
out vec3 normals;

void main()
{
    front = vec3(model_view_matrix * vec4(vertex_coordinates, 1.f));
    normals = normals_coordinates;

    gl_Position = projection_matrix * camera_matrix * model_view_matrix * vec4(vertex_coordinates.x, vertex_coordinates.y, vertex_coordinates.z, 1.0);
};

#shader fragment
#version 330



in vec3 front;
in vec3 normals;

out vec4 fragment_color;

uniform vec3 light_position;
uniform vec3 light_color;
uniform vec3 object_color;


void main()
{
    // ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * light_color;

    // diffuse 
    vec3 norm = normalize(normals);
    vec3 lightDir = normalize(light_position - front);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light_color;

    vec3 result = (ambient + diffuse) * object_color;

    fragment_color = vec4(result, 1.f);
};