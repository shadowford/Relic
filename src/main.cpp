#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include <time.h>
#include <stdlib.h>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include "Vector2D.h"
#include "Game.h"
#include "RenderUtils.h"

// Window stuff
double FPS = 60;
double microsecondsPerFrame = 1000000/FPS;
double WIDTH = 1024;
double HEIGHT = 768;

int currentFrame = 0;

struct Vector2D<double> mousePos = {0};

Game * currentGame;

void render (Game * game) {
    glLoadIdentity();
    glUseProgram(0);
    
    RenderUtils::renderSquare (mousePos, 0.005, 0.005, RenderUtils::Colour(0xff, 0xff, 0xff));

    game->render(currentFrame);
}

void renderingThread(sf::Window * window) {

    window->setActive(true);

    glMatrixMode(GL_MODELVIEW);

    while (window->isOpen()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render(currentGame);

        window->display();
    }
}

void glInit () {
    glViewport(0, 0, WIDTH, HEIGHT);
    glOrtho(0, WIDTH, HEIGHT, 0, 0, 1000);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}

int main (int argc, char ** argv) {

    std::cout << "Relic C++ Version" << std::endl;

    sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "Relic", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);
    window.setMouseCursorVisible(false);

    glInit();

    Game game = Game();
    currentGame = &game;

    sf::Thread thread(&renderingThread, &window);
    thread.launch();

    sf::Clock clock;

    srand(time(NULL));

    while (true) {
        clock.restart();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                break;
            } else if (event.type == sf::Event::Resized) {
                glViewport(0, 0, event.size.width, event.size.height);
                WIDTH = event.size.width;
                HEIGHT = event.size.height;
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == 36) { // ESC - closes program
                    exit(0);
                }
            }
        }

        game.update(currentFrame, mousePos);
 
        currentFrame += 1;

        mousePos.x += (sf::Mouse::getPosition(window).x / (double)WIDTH  - 0.5);
        mousePos.y -= (sf::Mouse::getPosition(window).y / (double)HEIGHT - 0.5);
        sf::Mouse::setPosition(sf::Vector2i(WIDTH / 2, HEIGHT / 2), window);

        sf::sleep(sf::microseconds(microsecondsPerFrame - clock.restart().asMicroseconds()));
    }

    return 0;
}