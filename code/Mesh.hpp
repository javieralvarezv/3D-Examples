#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
#include "Shader.hpp"

using namespace std;
namespace example
{
    
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texture_coords;
    };

    //Define un material
    struct Material 
    {
        //Material color lighting
        glm::vec4 Ka;
        //Diffuse
        glm::vec4 Kd;
        //Specular
        glm::vec4 Ks;
    };

    //Define una textura
    struct Texture
    {
        unsigned int id;
        string type;
        string path;
    };

    class Mesh
    {
    public:

        vector<Vertex>       vertices;
        vector<unsigned int> indices;
        vector<Texture>      textures;
        Material mats;

    private:    
        
        enum
        {
            COORDINATES_VBO,
            INDICES_VBO,
            VBO_COUNT
        };

        unsigned int vbo_ids[VBO_COUNT];
        unsigned int vao_id;
        unsigned int uniformBlockIndex;
        
    public:

        Mesh(vector<Vertex> &vertices, vector<unsigned int> &indices, vector<Texture> &textures, Material mat);

        void render(Shader &shader);

        inline Material &get_material() { return mats; }

    private:

        void setup_mesh();
    };
}
