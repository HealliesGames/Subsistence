#ifndef SCENE_H
#define SCENE_H

#include "Utility.h"
#include "Ship.h"
#include "Bullet.h"
#include "Upgrade.h"
#include "Asteroid.h"

class Scene
{
    public:
        Scene( );
        virtual ~Scene();

        bool Menu ( sf::RenderWindow &window );
        bool Play ( sf::RenderWindow &window );
};

#endif // SCENE_H
