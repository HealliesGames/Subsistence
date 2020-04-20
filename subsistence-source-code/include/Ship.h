#ifndef SHIP_H
#define SHIP_H

#include "Bullet.h"
#include "Asteroid.h"

class Ship
{
public:

    /****************** CTOR/DTOR ******************/
    Ship( float xParam, float yParam );      // Constructor.
    virtual ~Ship( );                        // Destructor.
    /***********************************************/



    /******************* Methods *******************/
    void Update( float dTParam );            // Ship logic called each frame.
    void Draw( sf::RenderWindow& winParam ); // Ship drawing called each frame.
    void Shoot ( std::vector<Bullet*> &bVec );
    void HasCollided( std::vector<Asteroid*> &aVec );
    void Death( float dTParam );

    sf::Vector2f GetPosition();
    float GetAngle();
    /***********************************************/

    bool shootUpgrade;


private:

    /****************** Constants ******************/
    const int ROTATION_SPEED = 300,         // Rotation speed.
              ACCELERATION = 300,           // Speed acceleration.
              FRICTION = 80,                // Speed deceleration.
              MAX_SPEED = 400;              // Max speed reachable.
    /***********************************************/



    /******************** Struct *******************/
    struct Force
    {
        // Defining a struct to simulate
        float direction,                  // the application of a physical force
              strength;                   // to the ship.
    };
    /***********************************************/



    /******************* Members *******************/
    int _size;                               // Ship size.

    bool _forceExist, _canShoot;               // Determines if a force exists at given angle.

    float _x,                                // Ship X coordinate.
          _y,                                // Ship Y coordinate.
          _direction;                            // Ship rotation.

    std::vector<Force> _forces;               // Vector of forces applied to the ship.
    std::vector<sf::CircleShape> _engine;

    sf::VertexArray _shipShape;              // Group of vertices that defines ship.
    sf::Clock _restoreBullet, _timeWait;
    /***********************************************/



    /******************* Methods *******************/
    void AddForceInShipDirection( float& dTParam ); // Create a force in ship direction.
    void ApplyForces ( float& dTParam );            // Apply all the forces to the ship.
    void ForcesUpdate( float& dTParam );            // Updating forces strength.
    void ClampPositionAndRotation( );              // Keeps values within a range.

    void UpdateShipShape( );                       // Update the shape.
    /***********************************************/

};

#endif // SHIP_H
