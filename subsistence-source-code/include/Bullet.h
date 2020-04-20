#ifndef BULLET_H
#define BULLET_H


class Bullet
{
    public:
        Bullet( float xParam, float yParam, float dirParam, float spdParam );
        virtual ~Bullet();

        bool OutScene ( );

        sf::Vector2f GetPosition( );
        void SetPosition( sf::Vector2f pos );

        sf::Vector2f GetTrajectory ( );
        sf::Vector2f dPoint;
        sf::Vector2f startColPoint;
        sf::Vector2f previousPoint;

        bool collide;
        void Update (float dTParam);
        void Draw( sf::RenderWindow& winParam );

    private:

        float _x,
              _y,
              _direction,
              _speed;/*,
              _timeLive;*/

};

#endif // BULLET_H
