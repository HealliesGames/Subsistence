#include "Utility.h"
#include "Ship.h"

Ship::Ship( float xParam, float yParam ) : _size{16}, _direction{270}, _canShoot{true}
{
    _x = xParam;                                        // Spawn X coordinate.
    _y = yParam;                                        // Spawn Y coordinate.
    _shipShape.setPrimitiveType(sf::TriangleFan);       // Rules how to draw the ship shape with vertices.
    _shipShape.resize(5);                               // Number of vertices used by ship.
    shootUpgrade = false;
}


Ship::~Ship( )
{
}





void Ship::Update( float dTParam )
{

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))          // If left key is pressed...
        _direction -= ROTATION_SPEED * dTParam;                     // ...rotate ship counter-clockwise.

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))         // If right key is pressed...
        _direction += ROTATION_SPEED * dTParam;                     // ...rotate ship clockwise.

    if(!rebuilding) {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))  {           // If key up is pressed...
            AddForceInShipDirection( dTParam );

            if(snMove.getStatus() != sf::Sound::Playing) {
                snMove.play();
                sf::CircleShape smoke(5);
                smoke.setFillColor(sf::Color::White);
                smoke.setPosition(_x, _y);
                smoke.setOrigin( smoke.getRadius(), smoke.getRadius());
                _engine.push_back(smoke);
            }
        }// ...add force.
    } else {
        int _speed = 200;
        _forces.clear();
        float deltaX = SCENE_WIDTH / 2 - _x;
        float deltaY = SCENE_HEIGHT / 1.2 - _y;
        float tmpDir = atan2( deltaY, deltaX );

        if(DistancePoint( sf::Vector2f(_x, _y), sf::Vector2f(SCENE_WIDTH / 2, SCENE_HEIGHT / 1.2)) >= _speed * dTParam) {
            _x += _speed * cos ( tmpDir ) * dTParam;
            _y += _speed * sin ( tmpDir ) * dTParam;
        }
    }

    if(!shootUpgrade) {
        if (_restoreBullet.getElapsedTime().asSeconds() >= .5)
            _canShoot = true;
    } else {
        if (_restoreBullet.getElapsedTime().asSeconds() >= .3)
            _canShoot = true;
    }


    ApplyForces( dTParam );                                             // Apply all the added force to ship.
    ForcesUpdate( dTParam );                                    // Update the strength of the _forces.
    ClampPositionAndRotation();                                 // Keeps values within a range.

}


void Ship::Death( float dTParam )
{

    for(int i = 0; i < _engine.size(); i++) {
        _engine[i].setRadius(_engine[i].getRadius() - 7 * dTParam);

        if(_engine[i].getRadius() <= 0)
            _engine.erase(_engine.begin() + i );
    }

    if(!gameOver) {
        _timeWait.restart().asSeconds();
    } else {
        _shipShape.setPrimitiveType(sf::LinesStrip);

        _x += 10 * cos( DegToRad( _direction )) * dTParam;
        _y += 10 * cos( DegToRad( _direction )) * dTParam;
        _direction += 10 * dTParam;

        if(_timeWait.getElapsedTime().asSeconds() >= 3) {

            showRestart = true;
            if(msAtmosphere.getVolume() < 50) {
                msAtmosphere.setVolume( msAtmosphere.getVolume() + dTParam * 10);

            }

        }
    }
}

    void Ship::Shoot ( std::vector<Bullet*> &bVec )
    {
        if( sf::Keyboard::isKeyPressed( sf::Keyboard::Z ) && _canShoot)
        {
            snShoot.play();

            if(!shootUpgrade) {
                Bullet* bullet = new Bullet( _x, _y, _direction, bulletSpeed);
                bVec.push_back( bullet );
            } else {
                for(int i = 0; i < 4; i++) {
                    Bullet* bullet = new Bullet( _x, _y, _direction + (i * 90), bulletSpeed);
                    bVec.push_back( bullet );
                }
            }

            _canShoot = false;
            _restoreBullet.restart().asSeconds();
        }
    }




    void Ship::Draw( sf::RenderWindow& winParam )
    {
        for(int i = 0; i < _engine.size(); i++)
            winParam.draw(_engine[i]);

        if(shootUpgrade) {
            for(int i = 0; i < _shipShape.getVertexCount(); i++)
                _shipShape[i].color = ( sf::Color(255, 128, 0));
        } else {
            for(int i = 0; i < _shipShape.getVertexCount(); i++)
                _shipShape[i].color = ( sf::Color::White);
        }


        UpdateShipShape();                                          // Update the shape.
        winParam.draw(_shipShape);                                  // Draw the ship.



        if(slowDown) {
            _restoreBullet.restart().asSeconds();
            snMove.pause();
        }
    }





    sf::Vector2f Ship::GetPosition()
    {
        return sf::Vector2f(_x, _y);
    }





    float Ship::GetAngle()
    {
        return _direction;
    }





    void Ship::AddForceInShipDirection( float& dTParam )
    {

        _forceExist = false;                                         // Set the existence of the force to false.

        for(int i = 0; i < _forces.size(); i++)                      // Check each active force.
        {
            if(_direction == _forces[i].direction)                           // If the force at ship angle already exists...
            {
                if(_forces[i].strength < MAX_SPEED)                  // ... and the force strength is less than max strength...
                    _forces[i].strength += ACCELERATION * dTParam;   // ... Give acceleration to that force.

                _forceExist = true;                                  // Confirm that the force exist.
                break;                                              // Leaving for loop.
            }
        }

        if(!_forceExist)                                             // If the force at ship angle doesn't exists...
        {
            Force force;                                            // Create a new force.

            force.direction = _direction;                                   // Set the angle to ship angle.
            force.strength = ACCELERATION * dTParam;                // Set the acceleration.
            _forces.push_back(force);                                // Add force to the vector.
        }

    }





    void Ship::ApplyForces ( float& dTParam )
    {

        for(int i = 0; i < _forces.size(); i++)                              // For each force in vector...
        {
            _x += _forces[i].strength * cos( DegToRad( _forces[i].direction )) * dTParam;   // Move the X coordinate at stored angle by stored strength.
            _y += _forces[i].strength * sin( DegToRad( _forces[i].direction )) * dTParam;   // Move the Y coordinate at stored angle by stored strength.
        }

    }





    void Ship::ForcesUpdate( float& dTParam )
    {

        for(int i = 0; i < _forces.size(); i++)                      // For each force in vector...
        {
            if(_forces[i].strength > 0)                             // ...If the force strength is greater than 0...
                _forces[i].strength -= FRICTION * dTParam;           // ...Impair it by friction.
            else                                                   // If the force is less than 0...
                _forces.erase(_forces.begin() + i);                   // ...erase it from vector.
        }

    }





    void Ship::ClampPositionAndRotation( )
    {

        _x = _x > SCENE_WIDTH ? 0 : _x;                             // If X coordinate is greater than scene width, teleport to 0.
        _x = _x < 0 ? SCENE_WIDTH : _x;                             // If X coordinate is less than 0, teleport to scene width.
        _y = _y > SCENE_HEIGHT ? 0 : _y;                            // If Y coordinate is greater than scene height, teleport to 0.
        _y = _y < 0 ? SCENE_HEIGHT : _y;                            // If Y coordinate is less than 0, teleport to scene height.


        _direction = _direction > 360 ? 0 : _direction;                         // If angle is greater than 360 degrees, set angle to 0.
        _direction = _direction < 0 ? 360 : _direction;                         // If angle is less than 0 degrees, set angle to 360.

    }

    void Ship::HasCollided( std::vector<Asteroid*> &aVec )
    {
        int vToCheck;

        for(int i = 0; i < aVec.size(); i++ )
        {

            for( int j = 0; j < aVec[i]->_asteroidShape.getVertexCount(); j++)
            {
                if(j < aVec[i]->_asteroidShape.getVertexCount() - 1)
                    vToCheck = j + 1;
                else
                    vToCheck = 0;


                if(DistancePointLine( aVec[i]->_asteroidShape[j].position.x,
                                      aVec[i]->_asteroidShape[j].position.y,
                                      aVec[i]->_asteroidShape[vToCheck].position.x,
                                      aVec[i]->_asteroidShape[vToCheck].position.y,
                                      _x,
                                      _y) < 8)
                {
                        gameOver = true;
                        snDeath.play();
                        msAtmosphere.setVolume(0);
                }


            }
        }
    }



    void Ship::UpdateShipShape( )
    {
        // Rotate and place each vertex by given angle around the real ship anchor point.
        _shipShape[0].position = PointRotation(sf::Vector2f(_x, _y), sf::Vector2f(_x + _size, _y), _direction);
        _shipShape[1].position = PointRotation(sf::Vector2f(_x, _y), sf::Vector2f(_x - _size / 2, _y - _size / 2), _direction);
        _shipShape[2].position = PointRotation(sf::Vector2f(_x, _y), sf::Vector2f(_x - _size / 4, _y), _direction);
        _shipShape[3].position = PointRotation(sf::Vector2f(_x, _y), sf::Vector2f(_x - _size / 2, _y + _size / 2), _direction);
        _shipShape[4].position = PointRotation(sf::Vector2f(_x, _y), sf::Vector2f(_x + _size, _y), _direction);
    }
