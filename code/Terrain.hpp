#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"
#include <vector>
#include <iostream>

namespace example
{
	class Terrain
	{
	private:

		int           n_columnas_x;					// numero de columnas
		int              n_filas_z;					// numero de filas

		int           n_de_vertices;
		int         n_de_triangulos;

		int puntos_x;
		int puntos_z;

		std::vector <float> y_heights;

		enum
		{
			COORDINATES_VBO,
			INDICES_IBO,
			NORMALS_VBO,
			VBO_COUNT
		};

		GLuint vbo_ids[VBO_COUNT];      // Ids de los VBOs que se usan
		GLuint vao_id;                  // Id del VertexArrayObject del modelo

		std::vector        <float>  vertices_coords;
		std::vector <unsigned int>     index_coords;
		std::vector        <float>   normals_coords;
		std::vector        <float>   texture_coords;

		std::vector<glm::vec3> normals_coord3;


		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

		//Heightmap para la malla
		//Texture heightmap;

	public:

		Terrain(int n_filas, int n_columnas);

		~Terrain();

		void create_vertices();

		void create_indices();

		void create_normals();

		void render();
		
		glm::mat4 calculate_model_view_matrix();

	private:

		void get_height_from_image(const std::string& path, float h_scale);

	};
}