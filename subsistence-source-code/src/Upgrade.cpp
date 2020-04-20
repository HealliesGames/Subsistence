#include "Utility.h"
#include "Upgrade.h"

Upgrade::Upgrade( int typeParam) : radius{10}, active{false}, _timeChange {0}
{
    _x = 128;
    _y = 128;
    type = typeParam;
    JumpToEdge();
}

Upgrade::~Upgrade()
{
    //dtor
}

void Upgrade::Update (float dTParam) {

    if(active) {
        _timeChange += dTParam * 10;

        if(_timeChange >= 10) {
            type = !type;
            _timeChange = 0;
        }

        switch(_direction) {
        case UP:
            _y -= _SPEED * dTParam;
            break;

        case DOWN:
            _y += _SPEED * dTParam;
            break;

        case LEFT:
            _x -= _SPEED * dTParam;
            break;

        case RIGHT:
            _x += _SPEED * dTParam;
            break;
        }

    }

}

void Upgrade::JumpToEdge() {

    int rEdge = rand() % 4;

    switch(rEdge){
    case 0:
        _x = SCENE_WIDTH;
        _y = rand() %(SCENE_HEIGHT - 32) + 32;
        _direction = LEFT;
        break;
    case 1:
        _y = 0;
        _x = rand() %(SCENE_WIDTH - 32) + 32;
        _direction = DOWN;
        break;
    case 2:
        _x = 0;
        _y = rand() %(SCENE_HEIGHT - 32) + 32;
        _direction = RIGHT;
        break;
    case 3:
        _y = SCENE_HEIGHT;
        _x = rand() %(SCENE_WIDTH - 32) + 32;
        _direction = UP;
        break;
    }

}


sf::Vector2f Upgrade::GetPosition( )
{
    return sf::Vector2f(_x, _y);
}

void Upgrade::Draw (sf::RenderWindow &winParam)
{

    sf::CircleShape circle (radius);
    circle.setPosition(_x, _y);
    circle.setOutlineThickness(1);
    circle.setOrigin( circle.getRadius(), circle.getRadius());

    switch(type) {

    case Upgrade::REBUILD:
            circle.setOutlineColor( sf::Color::Blue);
            circle.setFillColor(sf::Color::Cyan);
        break;

    case Upgrade::SHOOTANDPOINT:
            circle.setOutlineColor(sf::Color::Red);
            circle.setFillColor(sf::Color(255, 128, 0));
        break;

    }

    if(active)
    winParam.draw( circle );
}
