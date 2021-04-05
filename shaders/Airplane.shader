#shader vertex
#version 330

uniform mat4 projection_matrix;
uniform mat4 camera_matrix;
uniform mat4 model_view_matrix;

layout(location = 0) in vec3  vertex_coordinates;
layout(location = 1) in vec3 normals_coordinates;

uniform Mat
{
    vec4 aAmbient;
    vec4 aDiffuse;
    vec4 aSpecular;
};

out vec4 Ambient;
out vec4 Diffuse;
out vec4 Specular;

out vec3 front;
out vec3 normal;


void main()
{
    front = vec3(model_view_matrix * vec4(vertex_coordinates, 1.0));
    normal = mat3(transpose(inverse(model_view_matrix))) * normals_coordinates;

    Ambient = aAmbient;
    Diffuse = aDiffuse;
    Specular = aSpecular;


    gl_Position = projection_matrix * camera_matrix * vec4(front, 1.0);
};

#shader fragment
#version 330

in vec3 front;
in vec2 v_TextCoord;
in vec3 normal;

in vec4 Ambient;
in vec4 Diffuse;
in vec4 Specular;


struct PointLight
{
    vec3 f_position;
    vec3 f_ambient;
    vec3 f_diffuse;
    vec3 f_specular;
};

uniform PointLight point_light;

struct DirLight
{
    vec3 f_direcction;
    vec3 f_ambient;
    vec3 f_diffuse;
    vec3 f_specular;
};

uniform DirLight dir_light;

//Posicion de la camara
uniform vec3 viewPos;

out vec4 fragment_color;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewPos)
{
    // ambient
    vec3 ambient = light.f_ambient * Ambient.rgb;

    // diffuse 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.f_position - front);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.f_diffuse * diff * Diffuse.rgb;

    // specular
    vec3 viewDir = normalize(viewPos - front);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 35.f);
    vec3 specular = light.f_specular * spec * Specular.rgb;

    vec3 result = ambient + diffuse + specular;

    return result;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewPos)
{
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - front);

    vec3 lightDir = normalize(-light.f_direcction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 35.f);
    // combine results

    vec3 ambient = light.f_ambient * Ambient.rgb;
    vec3 diffuse = light.f_diffuse * Diffuse.rgb;
    vec3 specular = light.f_specular * Specular.rgb;

    vec3 result = ambient + diffuse + specular;

    return result;
}

void main()
{

    vec3 result = CalcPointLight(point_light, normal, front, viewPos);

    result += CalcDirLight(dir_light, normal, viewPos);

    fragment_color = vec4(result, 1.0);
};