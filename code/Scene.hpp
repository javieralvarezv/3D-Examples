#ifndef SCENE_HEADER      
#define SCENE_HEADER      

#include <glad/glad.h>
#include "Terrain.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Skybox.hpp"
#include "Model.hpp"
#include "DirLight.hpp"
#include "PointLight.hpp"
#include <string>

namespace example
{
    class Scene
    {
    private:

        int width;
        int height;
         
        Camera camera;
        
        Terrain   malla;
        Skybox skybox;
        
        Model backpack;
        Model earth;
        Model spaceship;

        Shader shader_backpack;
        Shader shader_skybox;
        Shader shader_terrain;
        Shader shader_earth;
        Shader shader_airplane;

        PointLight point_light;
        DirLight     dir_light;

    public:

        Scene(int width, int height);

        inline Camera* get_camara() { return &camera; }

        void   render();
        void   update();
    };
}

#endif
