#include "Model.hpp"

namespace example
{
	Model::Model(string const& path, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
        :
        position(position),
        rotation(rotation),
        scale(scale),
        angle(0)
	{
		load_model(path);
	}
	
	void Model::render(Shader &shader)
	{
		for (unsigned int i = 0; i < meshes.size(); i++)
		{
			meshes[i].render(shader);
		}
	}

    void Model::update()
    {
        angle += 0.2f;
    }

    glm::mat4 Model::calculate_model_view_matrix()
    {
        glm::mat4 model_view_matrix;

        model_view_matrix = glm::translate(model_view_matrix, position);
        model_view_matrix = glm::rotate(model_view_matrix, angle, rotation);
        model_view_matrix = glm::scale(model_view_matrix, scale);

        return model_view_matrix;
    }
	
	void Model::load_model(string const& path)
	{
		// lee el fichero via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		// check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
		{
			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
		
		directory = path.substr(0, path.find_last_of('/'));

		// procesa los nodos de ASSIMP recursivamente
		process_node(scene->mRootNode, scene);
	}

	void Model::process_node(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(process_mesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			process_node(node->mChildren[i], scene);
		}
	}
	
	Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene)
	{
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;


        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector;
            
            // vertices
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.position = vector;
            
            // normales
            //comprobamos si el modelo tiene normales
            if (mesh->mNormals != nullptr)
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.normal = vector;
            }
            
            // texture coordinates
            //comprobamos si el modelo tiene coordenadas de texturas
            if (mesh->mTextureCoords[0])
            {
                glm::vec2 vec;
               
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.texture_coords = vec;
            }
            else
                vertex.texture_coords = glm::vec2(0.0f, 0.0f);
           
            vertices.push_back(vertex);
        }
        // indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];

            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        
        Material mat;
        aiColor3D color;

        material->Get(AI_MATKEY_COLOR_AMBIENT, color);
        mat.Ka = glm::vec4(color.r, color.g, color.b, 1.0);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        mat.Kd = glm::vec4(color.r, color.g, color.b, 1.0);
        material->Get(AI_MATKEY_COLOR_SPECULAR, color);
        mat.Ks = glm::vec4(color.r, color.g, color.b, 1.0);

        //Asuminos esta convencion para los nombres
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. diffuse maps
        vector<Texture> diffuseMaps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        vector<Texture> specularMaps = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Texture> normalMaps = load_material_textures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> heightMaps = load_material_textures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        // retornamos una mesh con los datos extraidos
        return Mesh(vertices, indices, textures, mat);
	}
	
    vector<Texture> Model::load_material_textures(aiMaterial* mat, aiTextureType type, string typeName)
	{
        vector<Texture> textures;
        
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // Una textura con el mismo path ya ha sido cargada asique se continua a la siguiente
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; 
                    break;
                }
            }
            if (!skip)
            {   
                // Si la textura no ha sido cargada antes, se carga
                Texture texture;
                texture.id = texture_from_file(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);
            }
        }
        return textures;
    }

    unsigned int texture_from_file(const char* path, const string& directory)
    {
        string filename = string(path);
        filename = directory + '/' + filename;

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        stbi_set_flip_vertically_on_load(1);
        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        
        if (data)
        {
            GLenum format;

            if (nrComponents == 1)
            {
                format = GL_RED;
            }   
            else if (nrComponents == 3)
            {
                format = GL_RGB;
            }   
            else if (nrComponents == 4)
            {
                format = GL_RGBA;
            }
              
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }
}