#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "stb_image.h"

#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

#include "Mesh.hpp"
#include "Shader.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;


namespace example
{
    //Este metodo se encaga de extraer la textura de un path dado
    unsigned int texture_from_file(const char* path, const string& directory);

    class Model
    {
    private:
        //Guarda las texturas cargadas para comprobar que no se cargan mas de una vez
        vector<Texture> textures_loaded;
        vector <Mesh> meshes;
        string directory;

        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        float angle;

    public:
        // constructor, espera un path a un modelo 3D
        Model(string const& path, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

        //Dibuja el modelo iterando por todas sus mayas
        void render(Shader &shader);

        void update();

        glm::mat4 calculate_model_view_matrix();


    private:
        //Carga un modelo con ASSIMP de un path y guarda las meshes en el vector de meshes.
        void load_model(string const& path);

        //Procesa los nodos recursivamente
        void process_node(aiNode* node, const aiScene* scene);

        Mesh process_mesh(aiMesh* mesh, const aiScene* scene);

        vector<Texture> load_material_textures(aiMaterial* mat, aiTextureType type, string typeName);

       
    };
}
