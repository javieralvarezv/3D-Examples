
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <glad/glad.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include "Scene.hpp"

using namespace sf;
using namespace example;


int main()
{
	Window window(sf::VideoMode(1280, 720), "3D Avanzado", Style::Close, ContextSettings(64));
	
	Clock reloj;
	Time deltaTime;
    
    //Inicializamos glad
	int check = gladLoadGL();

	Scene scena(1280, 720);

	window.setVerticalSyncEnabled(true);

    bool running = true;

    do
    {
        Event event;

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed:
            {
                running = false;
                break;
            }
            case sf::Event::KeyPressed:

                switch (event.key.code)
                {
                case sf::Keyboard::W:

                    scena.get_camara()->move_front();

                    break;

                case sf::Keyboard::S:

                    scena.get_camara()->move_back();
                    
                    break;

                case sf::Keyboard::A:

                    scena.get_camara()->move_left();

                    break;

                case sf::Keyboard::D:

                    scena.get_camara()->move_right();

                    break;

                case sf::Keyboard::Q:

                    scena.get_camara()->look_left();

                    break;

                case sf::Keyboard::E:

                    scena.get_camara()->look_right();

                    break;

                case sf::Keyboard::R:

                    scena.get_camara()->move_up();

                    break;

                case sf::Keyboard::F:

                    scena.get_camara()->move_down();

                    break;

                case sf::Keyboard::T:

                    scena.get_camara()->look_up();

                    break;
                
                case sf::Keyboard::G:

                    scena.get_camara()->look_down();

                    break;

               
                default:
                    break;
                }

            }
        }

        scena.update();
        scena.render();

        window.display();

    } while (running);

    return (EXIT_SUCCESS);
}