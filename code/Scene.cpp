//#define GLM_FORCE_RADIANS
#include "Scene.hpp"
#include <iostream>
#include <cassert>

#include <glm/glm.hpp>                          // vec3, vec4, ivec4, mat4
#include <glm/gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <glm/gtc/type_ptr.hpp>                 // value_ptr
#include <glm/gtx/string_cast.hpp>

using namespace std;

namespace example
{

    Scene::Scene(int width, int height)
        :
        width(width),
        height(height),
        malla(30, 30),
        backpack("../../assets/backpack/backpack.obj", glm::vec3(1.f, 1.33f, -3.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.2f, 0.2, 0.2f)),
        earth("../../assets/earth/earth.obj", glm::vec3(5.f, 5.f, -12.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(2.0f, 2.0f, 2.0f)),
        spaceship("../../assets/airplane/Attacker.obj", glm::vec3(-3.f, 5.f, -5.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.2f, 0.2f, 0.2f)),
        shader_backpack("../../shaders/Backpack.shader"),
        shader_skybox("../../shaders/Skybox.shader"),
        shader_terrain("../../shaders/Terrain.shader"),
        shader_earth("../../shaders/Earth.shader"),
        shader_airplane("../../shaders/Airplane.shader"),
        point_light(glm::vec3(3.f, 5.f, -10.f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.95f, 0.95f, 0.95f), glm::vec3(1.0f, 1.0f, 1.0f)),
        dir_light(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.1f, 0.1f, 0.1f))

    {
        glClearColor(0.f, 0.8f, 0.7f, 1.f);
    }

    void Scene::render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glViewport(0, 0, width, height);

        //Se calculan las matrices de proyeccion y de la camara;
        glm::mat4 projection_matrix = glm::perspective(20.f, GLfloat(1280) / GLfloat(720), 1.f, 100.f);
        glm::mat4 camera_matrix     = camera.get_camera_view();

        //Malla
        shader_terrain.bind();
        glm::mat4 malla_model_view_matrix = malla.calculate_model_view_matrix();

        shader_terrain.set_uniform_mat4("projection_matrix", projection_matrix);
        shader_terrain.set_uniform_mat4("camera_matrix", camera_matrix);
        shader_terrain.set_uniform_mat4("model_view_matrix", malla_model_view_matrix);

        shader_terrain.set_uniform3f("light_position", 1.2f, 6.f, 2.f);
        shader_terrain.set_uniform3f("object_color", 0.266f, 0.439f, 0.262f);
        shader_terrain.set_uniform3f("light_color", 1.f, 1.f, 1.f);
        
        malla.render();
        shader_terrain.unbind();
        
        //Mochila
        shader_backpack.bind();
        glm::mat4 backpack_model_view_matrix = backpack.calculate_model_view_matrix();

        shader_backpack.set_uniform_mat4("projection_matrix",          projection_matrix);
        shader_backpack.set_uniform_mat4("camera_matrix"    ,              camera_matrix);
        shader_backpack.set_uniform_mat4("model_view_matrix", backpack_model_view_matrix);
        backpack.render(shader_backpack);

        shader_backpack.unbind();

        //Earth
        shader_earth.bind();

        vec3 cameraPos = camera.get_camera_position();
        glm::mat4 arbol_model_view_matrix = earth.calculate_model_view_matrix();
        
        shader_earth.set_uniform_mat4("projection_matrix", projection_matrix);
        shader_earth.set_uniform_mat4("camera_matrix", camera_matrix);
        shader_earth.set_uniform_mat4("model_view_matrix", arbol_model_view_matrix);

        camera.set_view_pos(shader_earth);
        point_light.set_uniforms(shader_earth);
        dir_light.set_uniforms(shader_earth);
        
        earth.render(shader_earth);

        shader_earth.unbind();

        //Airplane

        shader_airplane.bind();

        glm::mat4 spaceship_model_view_matrix = spaceship.calculate_model_view_matrix();

        shader_airplane.set_uniform_mat4("projection_matrix", projection_matrix);
        shader_airplane.set_uniform_mat4("camera_matrix", camera_matrix);
        shader_airplane.set_uniform_mat4("model_view_matrix", spaceship_model_view_matrix);

        camera.set_view_pos(shader_airplane);
        point_light.set_uniforms(shader_airplane);
        dir_light.set_uniforms(shader_airplane);

        spaceship.render(shader_airplane);

        shader_airplane.unbind();
        

        //Skybox
        shader_skybox.bind();
        glm::mat4 skybox_model_view_matrix = skybox.calculate_skybox_view_matrix(camera.get_camera_position());

        shader_skybox.set_uniform_mat4("projection_matrix",             projection_matrix);
        shader_skybox.set_uniform_mat4("camera_matrix"    ,                 camera_matrix);
        shader_skybox.set_uniform_mat4("model_view_matrix",      skybox_model_view_matrix);
        skybox.render(shader_skybox);

        shader_skybox.unbind();
    }

    void Scene::update()
    {
        camera.update();
        earth.update();
    }
}

