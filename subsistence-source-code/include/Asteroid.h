#ifndef ASTEROID_H
#define ASTEROID_H

#include "Bullet.h"

class Asteroid
{
public:
    Asteroid( float xParam, float yParam, sf::VertexArray asteroidShapeParam);
    virtual ~Asteroid();

    void Update( float dTParam );
    void Draw( sf::RenderWindow& winParam);

    int BulletCollision ( std::vector<Bullet*> &bVec, float dTParam);
    void GetSplitPoint ( sf::Vector2f a, sf::Vector2f b );
    sf::VertexArray Split( );

    sf::Vector2f GetPosition();
    sf::VertexArray _asteroidShape;

    sf::Vector2f _splitPointA,
    _splitPointB;

    sf::Vector2f rebuildPoint;

private:

    float _x,                                // Asteroid X coordinate.
          _y,                                // Asteroid Y coordinate.
          _direction,                        // Asteroid direction.
          _speed;                            // Asteroid speed;

    void ClampPosColRot();



    std::vector<sf::Vector2f> _asteroidShapeVertices;
};

#endif // ASTEROID_H
