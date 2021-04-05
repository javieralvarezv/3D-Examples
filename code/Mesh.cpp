#include "Mesh.hpp"
#include "glm/ext.hpp"
#include <iostream>

namespace example
{
	Mesh::Mesh(vector<Vertex> &new_vertices, vector<unsigned int> &new_indices, vector<Texture> &new_textures, Material mat)
	{
		vertices = new_vertices;
		indices = new_indices;
		textures = new_textures;
        mats = mat;

		setup_mesh();
	}
	void Mesh::render(Shader &shader)
	{
        // Bindeamos las texturas apropiadas
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            // Activamos la textura
            glActiveTexture(GL_TEXTURE0 + i); 
            
            string number;
            string name = textures[i].type;
            
            if (name == "texture_diffuse")
            {
                number = std::to_string(diffuseNr++);
            }                
            else if (name == "texture_specular")
            {
                number = std::to_string(specularNr++); 
            }   
            else if (name == "texture_normal")
            {
                number = std::to_string(normalNr++); 
            }                
            else if (name == "texture_height")
            {
                number = std::to_string(heightNr++); 
            }

            // Set del sampler
            glUniform1i(glGetUniformLocation(shader.get_renderer_id(), (name + number).c_str()), i);
            // Bindeamos la textura
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        // Dibujamos la malla
        glBufferData(GL_UNIFORM_BUFFER, sizeof(mats), &mats, GL_STATIC_DRAW);
        glBindVertexArray(vao_id);
        //Casting a GLsizei para eliminar un warning
        glDrawElements(GL_TRIANGLES, GLsizei(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Seteamos a default una vez configurado
        glActiveTexture(GL_TEXTURE0);


	}
	void Mesh::setup_mesh()
	{
        // Se generan índices para los VBOs del modelo:
        glGenVertexArrays(1, &vao_id);
        glGenBuffers(VBO_COUNT, vbo_ids);
        
        // Se activa el VAO del modelo para configurarlo:
        glBindVertexArray(vao_id);
        
        // Se suben a un VBO los datos de vertices y se vinculan al VAO:
        glBindBuffer(GL_ARRAY_BUFFER,vbo_ids[COORDINATES_VBO]);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        //Establecemos los vertices en el layout 0 del shader
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // Se suben a un VBO los datos de indices y se vinculan al VAO:
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_VBO]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        
        //Establecemos las normales en el layout 1 del shader
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        
        //Establecemos las texture coords en el layout 1 del shader
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coords));

        //Colores
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, uniformBlockIndex, 0, sizeof(Material));
        glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockIndex);




        glBindVertexArray(0);
	}
}