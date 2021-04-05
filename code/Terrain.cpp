#include "Terrain.hpp"
#include <glm/gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <glm/gtc/type_ptr.hpp>                 // value_ptr
#include <glm\gtx\string_cast.hpp>

#include <iostream>
#include "stb_image.h"

namespace example
{
	Terrain::Terrain(int n_filas, int n_columnas)
		:
		n_filas_z(n_filas),
		n_columnas_x(n_columnas),
		position(-7.5f, 0.f, 0.f),
		rotation(0.f, 0.f, 0.f),
		scale(0.5f, 0.5f, 0.5f)
	{
		puntos_x = n_columnas_x + 1;
		puntos_z = n_filas_z + 1;

		n_de_vertices = puntos_x * puntos_z;
		n_de_triangulos = (n_filas_z * n_columnas_x) * 2;

		get_height_from_image("../../assets/textures/hm.png", 0.02f);

		create_vertices();
		create_indices();
		create_normals();

		// Se generan índices para los VBOs del modelo:
		glGenBuffers(VBO_COUNT, vbo_ids);
		glGenVertexArrays(1, &vao_id);

		// Se activa el VAO del modelo para configurarlo:

		glBindVertexArray(vao_id);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
		glBufferData(GL_ARRAY_BUFFER, vertices_coords.size() * sizeof(float), &vertices_coords.front(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Se suben a un IBO los datos de índices:

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_IBO]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_coords.size() * sizeof(unsigned int), &index_coords.front(), GL_STATIC_DRAW);

		// Se suben a un VBO los datos de normales y se vinculan al VAO:
		glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[NORMALS_VBO]);
		glBufferData(GL_ARRAY_BUFFER, normals_coords.size() * sizeof(float), &normals_coords.front(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	}
	Terrain::~Terrain()
	{
		glDeleteVertexArrays(1, &vao_id);
		glDeleteBuffers(VBO_COUNT, vbo_ids);
	}
	void Terrain::create_vertices()
	{

		float index = 0;
		
		for (float x = 0; x < puntos_x; x++)
		{
			for (float z = 0; z < puntos_z; z++)
			{
				vertices_coords.push_back(x);
				vertices_coords.push_back(y_heights.at(index));
				vertices_coords.push_back(-z);
				index++;
				
			}
		}

	}
	void Terrain::create_indices()
	{
		unsigned int index = 0;

		for (int x = 0; x < n_columnas_x; x++)            // x barre de 0 a (n_x - 1)
		{
			if (index >= n_de_triangulos) break;

			for (int z = 0; z < n_filas_z; z++)  // z barre de 0 a (m_z - 1)
			{
				unsigned int Vertex_a = (x * puntos_z) + z;
				unsigned int Vertex_b = Vertex_a + puntos_z;
				unsigned int Vertex_c = Vertex_b + 1;

				index_coords.push_back(Vertex_a);
				index_coords.push_back(Vertex_b);
				index_coords.push_back(Vertex_c);

				index++;

				index_coords.push_back(Vertex_a);
				index_coords.push_back(Vertex_b + 1);
				index_coords.push_back(Vertex_a + 1);

				index++;
			}
		}
	}
	
	void Terrain::create_normals()
	{
		unsigned int a, b, c, v;
		glm::vec3 va, vb, vc;
		glm::vec3 vn(0.f);
		bool cargar_v = false;					//= { 0.f, 0.f, 0.f };
		unsigned int cuenta_vectores = 0;

		for (unsigned int i = 0; i < n_de_vertices; i++)
		{
			for (unsigned int j = 0; j < n_de_triangulos*3; j+=3)
			{
				a = index_coords.at(j);
				b = index_coords.at(j+1);
				c = index_coords.at(j+2);

				va.x = vertices_coords.at(a * 3);
				va.y = vertices_coords.at(a * 3 + 1);
				va.z = vertices_coords.at(a * 3 + 2);

				vb.x = vertices_coords.at(b * 3);
				vb.y = vertices_coords.at(b * 3 + 1);
				vb.z = vertices_coords.at(b * 3 + 2);

				vc.x = vertices_coords.at(c * 3);
				vc.y = vertices_coords.at(c * 3 + 1);
				vc.z = vertices_coords.at(c * 3 + 2);

				if (a == i)   // caso 1   orden; i, v, W;
				{
					vn += glm::cross((vb - va), (vc - va));
					cargar_v = true;
					cuenta_vectores++;
				}

				if (b == i)
				{
					vn += glm::cross((vc - vb), (va - vb));
					cargar_v = true;
					cuenta_vectores++;
				}

				if (c == i)
				{
					vn += glm::cross((va - vc), (vb - vc));
					cargar_v = true;
					cuenta_vectores++;
				}
			}

			if (cargar_v)
			{
				normals_coord3.push_back(glm::normalize(vn));
				cargar_v = false;
			}
			vn = { 0.f, 0.f, 0.f };

		}

		for (int k = 0; k < normals_coord3.size(); k++)
		{
			normals_coords.push_back(normals_coord3.at(k).x);
			normals_coords.push_back(normals_coord3.at(k).y);
			normals_coords.push_back(normals_coord3.at(k).z);
		}

	}

	void Terrain::render()
	{
		// Habilidad el test de profundidad
		glEnable(GL_DEPTH_TEST);
		// Aceptar el fragmento si está más cerca de la cámara que el fragmento anterior
		glDepthFunc(GL_LESS);

		glBindVertexArray(vao_id);
		glDrawElements(GL_TRIANGLES, index_coords.size() * sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	}
	
	glm::mat4 Terrain::calculate_model_view_matrix()
	{
		glm::mat4 model_view_matrix;

		model_view_matrix = glm::translate(model_view_matrix, position);
		//model_view_matrix = glm::rotate(model_view_matrix, 0.f, rotation);
		model_view_matrix = glm::scale(model_view_matrix, scale);

		return model_view_matrix;
	}

	void Terrain::get_height_from_image(const std::string& path, float h_scale)
	{
		unsigned char* local_buffer;
		int width;
		int height;
		int bit_per_pixel;

		local_buffer = stbi_load(path.c_str(), &width, &height, &bit_per_pixel, 4);

		int rate_x = width /  puntos_x;
		int rate_z = height / puntos_z;

		unsigned char h_point = 0.f;

		for (int x = 0; x < puntos_x; x++)
		{
			for (int z = 0; z < puntos_z; z++)
			{
				h_point = local_buffer[(x * rate_x * height * 4 + z * rate_z * 4)];
				h_point = h_point * h_scale;
				y_heights.push_back(h_point);
			}
		}

		if (local_buffer)
		{
			stbi_image_free(local_buffer);
		}

	}

}