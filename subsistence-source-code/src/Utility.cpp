#include "Utility.h"

bool fulls = false, gameOver = false, showRestart = false, slowDown = false, rebuilding = false;

int spawnAsteroidSize = 160;
int bulletSpeed = 400;

sf::Music msAtmosphere;
sf::SoundBuffer sbSplit, sbStart, sbUpgrade, sbSelection, sbDeath, sbMove, sbShoot;
sf::Sound snSplit, snStart, snUpgrade, snSelection, snDeath, snMove, snShoot;


sf::Font font;

float DegToRad( float deg )
{
    return (deg * PI) / 180;
}

void LoadResources( ) {

    sbStart.loadFromFile("start.wav");
    snStart.setBuffer(sbStart);

    sbSelection.loadFromFile("selection.wav");
    snSelection.setBuffer(sbSelection);

    sbUpgrade.loadFromFile("powerup.wav");
    snUpgrade.setBuffer(sbUpgrade);

    sbSplit.loadFromFile("splitting.wav");
    snSplit.setBuffer(sbSplit);

    sbDeath.loadFromFile("death.wav");
    snDeath.setBuffer(sbDeath);

    sbShoot.loadFromFile("shoot.wav");
    snShoot.setBuffer(sbShoot);

    sbMove.loadFromFile("move.wav");
    snMove.setBuffer(sbMove);

    msAtmosphere.openFromFile("atmosphere.ogg");
    msAtmosphere.setLoop(true);
    msAtmosphere.setVolume(50);

    font.loadFromFile("BPdotsUnicaseSquare.otf");
}



sf::Vector2f PointRotation(sf::Vector2f pinPoint, sf::Vector2f point, float angle)
{

    angle = DegToRad(angle);

    float xRotated = cos(angle) * (point.x - pinPoint.x) - sin(angle) * (point.y - pinPoint.y) + pinPoint.x;
    float yRotated = sin(angle) * (point.x - pinPoint.x) + cos(angle) * (point.y - pinPoint.y) + pinPoint.y;

    sf::Vector2f rotatedPoint(xRotated, yRotated);

    return rotatedPoint;
}




sf::Vector2f PointOfIntersection ( sf::Vector2f lineOneBeg, sf::Vector2f lineOneEnd, sf::Vector2f lineTwoBeg, sf::Vector2f lineTwoEnd )
{
        float a1 = lineOneEnd.y - lineOneBeg.y;
        float b1 = lineOneBeg.x - lineOneEnd.x;
        float c1 = a1*(lineOneBeg.x) + b1*(lineOneBeg.y);

        float a2 = lineTwoEnd.y - lineTwoBeg.y;
        float b2 = lineTwoBeg.x - lineTwoEnd.x;
        float c2 = a2*(lineTwoBeg.x)+ b2*(lineTwoBeg.y);

        float determinant = a1*b2 - a2*b1;

        if (determinant == 0)
        {
            return sf::Vector2f(NOT_SETTED, NOT_SETTED);
        }
        else
        {
            float x = (b2*c1 - b1*c2)/determinant;
            float y = (a1*c2 - a2*c1)/determinant;

            if (DistancePointLine(lineOneBeg.x, lineOneBeg.y, lineOneEnd.x, lineOneEnd.y, x, y) < .01)
            return sf::Vector2f(x, y);
            else
            return sf::Vector2f(NOT_SETTED, NOT_SETTED);

        }

}

float DistancePointLine(float x1,float y1,float x2,float y2,float x3,float y3)
    {
        float px=x2-x1;
        float py=y2-y1;
        float temp=(px*px)+(py*py);
        float u=((x3 - x1) * px + (y3 - y1) * py) / (temp);
        if(u>1){
            u=1;
        }
        else if(u<0){
            u=0;
        }
        float x = x1 + u * px;
        float y = y1 + u * py;

        float dx = x - x3;
        float dy = y - y3;
        double dist = sqrt(dx*dx + dy*dy);
        return dist;
    }


float DistancePoint( sf::Vector2f pointA, sf::Vector2f pointB ){
    float dist;
    dist = sqrt( pow( pointB.x - pointA.x, 2 ) + pow( pointB.y - pointA.y, 2 ) );
    return dist;
}


void SaveRecord( int record ) {
    std::fstream file;
    file.open("subsistence.sav", std::ofstream::out | std::ofstream::trunc);
    file << record << std::endl << "Why should you cheat? That doesn't make sense.";
    file.close();
}

int LoadRecord( ) {
    int record = 0;

    std::fstream file;
    file.open("subsistence.sav", std::ofstream::in );
    file >> record;
    file.close();

    return record;
}
