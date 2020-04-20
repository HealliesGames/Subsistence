#include "Scene.h"

int main()
{

    srand(time(NULL));
    LoadResources();
    bool quit = false;
    bool restart;
    sf::RenderWindow window(sf::VideoMode(SCENE_WIDTH, SCENE_HEIGHT), "Subsistence");

    Scene* scene = new Scene();

    while(!quit && window.isOpen()) {
        restart = true;
        quit = scene->Menu(window);

        if(!quit && window.isOpen())
            while(restart && window.isOpen())
                restart = scene->Play(window);
    }

    delete scene;

    return 0;
}
