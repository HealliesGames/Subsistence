#include "Utility.h"
#include "Bullet.h"

Bullet::Bullet( float xParam, float yParam, float dirParam, float spdParam ) : collide {false}//, _timeLive{0}
{
    _x = xParam;
    _y = yParam;
    _direction = dirParam;
    _speed = spdParam;
    dPoint.x = NOT_SETTED;
    dPoint.y = NOT_SETTED;
    previousPoint.x = _x;
    previousPoint.y = _y;
}

Bullet::~Bullet()
{
    //dtor
}
sf::Vector2f Bullet::GetPosition( ) {
    return sf::Vector2f(_x, _y);
}

void Bullet::SetPosition( sf::Vector2f pos ) {
    _x = pos.x;
    _y = pos.y;
}

sf::Vector2f Bullet::GetTrajectory ( ) {
    sf::Vector2f trajectory;

    trajectory.x = _x;
    trajectory.y = _y;
    do {

        trajectory.x += cos(DegToRad(_direction)) * 4;
        trajectory.y += sin(DegToRad(_direction)) * 4;

    } while( trajectory.x > 0 && trajectory.x < SCENE_WIDTH &&
             trajectory.y > 0 && trajectory.y < SCENE_HEIGHT);

    return trajectory;
}

void Bullet::Update( float dTParam ) {

    if(slowDown)
        if(collide)
        _speed = bulletSpeed / 2;
        else
        _speed = 0;
    else {
        _speed = bulletSpeed;
    }

    previousPoint.x = _x;
    previousPoint.y = _y;

    _x += _speed * cos (DegToRad(_direction)) * dTParam;
    _y += _speed * sin (DegToRad(_direction)) * dTParam;

   /* if(collide)
    _timeLive += dTParam * 10; */
}

bool Bullet::OutScene ( ) {


    if( (_x > SCENE_WIDTH || _x < 0 || _y > SCENE_HEIGHT  || _y < 0)) {

        if(!collide) //{
        return true;
        else {
           /*  if(_timeLive >= 20) {
                slowDown = false;
                return true;
            }*/
         if(DistancePoint(GetPosition(), dPoint ) > spawnAsteroidSize * 2)
        {
            slowDown = false;
            return true;
        }

        }
    }

    return false;
}

void Bullet::Draw ( sf::RenderWindow& winParam ) {
    sf::CircleShape bulletShape(2);

    bulletShape.setPosition(_x, _y);
    bulletShape.setOrigin(bulletShape.getRadius(), bulletShape.getRadius());
    bulletShape.setFillColor( sf::Color::Yellow );

    winParam.draw( bulletShape );

    if(collide && slowDown) {

        if(snSplit.getStatus() != sf::Sound::Playing)
            snSplit.play();

        sf::VertexArray trail;
        trail.setPrimitiveType(sf::Lines);
        trail.resize(2);

        trail[0].color = sf::Color::Yellow;
        trail[1].color = sf::Color::Yellow;

        trail[0].position = startColPoint;
        trail[1].position = GetPosition();

        winParam.draw( trail );
    }
}
