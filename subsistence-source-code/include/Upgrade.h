#ifndef UPGRADE_H
#define UPGRADE_H

class Upgrade
{
    public:
        Upgrade( int typeParam );
        virtual ~Upgrade();

        void Update (float dTParam);
        void Draw (sf::RenderWindow &winParam);
        void JumpToEdge();

        sf::Vector2f GetPosition();
        int radius, type;
        bool active;


        enum UpgradeType{
            SHOOTANDPOINT,
            REBUILD
        };

    private:

        const int _SPEED = 100;

        enum Direction{
            UP,
            DOWN,
            LEFT,
            RIGHT
        };

        float _x, _y, _timeChange;
        int _direction;
};

#endif // UPGRADE_H
