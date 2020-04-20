#include "Utility.h"
#include "Asteroid.h"

Asteroid::Asteroid( float xParam, float yParam, sf::VertexArray asteroidShapeParam )
{
    _x = xParam;
    _y = yParam;
    _asteroidShape.resize(asteroidShapeParam.getVertexCount());
    _asteroidShape.setPrimitiveType(sf::TriangleFan);

    _direction = rand() % 360;

    _speed = 50 + rand() %21;

    for(int i = 0; i < asteroidShapeParam.getVertexCount(); i++)
    {
        _asteroidShapeVertices.push_back(asteroidShapeParam[i].position);
        _asteroidShape[i].position.x = _asteroidShapeVertices[i].x +_x;
        _asteroidShape[i].position.y = _asteroidShapeVertices[i].y +_y;
    }
}





Asteroid::~Asteroid( )
{
}





void Asteroid::Update( float dTParam )
{
    if(!rebuilding) {
        if(!slowDown)
        {
            _x += _speed * cos (DegToRad(_direction)) * dTParam;
            _y += _speed * sin (DegToRad(_direction)) * dTParam;
        }
    } else {
            _speed = 200;
            float deltaX = rebuildPoint.x - _x;
            float deltaY = rebuildPoint.y - _y;
            float tmpDir = atan2( deltaY, deltaX );

            if(DistancePoint( sf::Vector2f(_x, _y), rebuildPoint) >= _speed * dTParam) {
                _x += _speed * cos ( tmpDir ) * dTParam;
                _y += _speed * sin ( tmpDir ) * dTParam;
            }

    }

    for( int i = 0; i < _asteroidShapeVertices.size(); i++)
    {
        _asteroidShape[i].position.x = _asteroidShapeVertices[i].x +_x;
        _asteroidShape[i].position.y = _asteroidShapeVertices[i].y +_y;
    }
    ClampPosColRot();
}

sf::Vector2f Asteroid::GetPosition()
{
    return sf::Vector2f(_x, _y);
}


void Asteroid::ClampPosColRot( )
{

    _x = _x > SCENE_WIDTH + spawnAsteroidSize ? -spawnAsteroidSize : _x;                             // If X coordinate is greater than scene width, teleport to 0.
    _x = _x < -spawnAsteroidSize ? SCENE_WIDTH + spawnAsteroidSize : _x;                             // If X coordinate is less than 0, teleport to scene width.
    _y = _y > SCENE_HEIGHT +spawnAsteroidSize ? -spawnAsteroidSize : _y;                            // If Y coordinate is greater than scene height, teleport to 0.
    _y = _y < -spawnAsteroidSize ? SCENE_HEIGHT + spawnAsteroidSize: _y;                            // If Y coordinate is less than 0, teleport to scene height.

    _direction = _direction > 360 ? 0 : _direction;                         // If direction is greater than 360 degrees, set direction to 0.
    _direction = _direction < 0 ? 360 : _direction;                         // If direction is less than 0 degrees, set direction to 360.

}

void Asteroid::GetSplitPoint ( sf::Vector2f a, sf::Vector2f b )
{
    sf::Vector2f intersect;

    _splitPointA.x = NOT_SETTED;
    _splitPointB.x = NOT_SETTED;

    for(int i = 0; i < _asteroidShape.getVertexCount(); i++)
    {
        if(i < _asteroidShape.getVertexCount() - 1)
            intersect = PointOfIntersection( _asteroidShape[i + 1].position, _asteroidShape[i].position, a, b);
        else
            intersect = PointOfIntersection( _asteroidShape[i].position, _asteroidShape[0].position, a, b);

        //std::cout << "Intersect x: " << intersect.x << " - " << "Intersect y: " << intersect.y << std::endl;
        if(intersect.x > NOT_SETTED)
        {
            if(_splitPointA.x > NOT_SETTED)
            {
                _splitPointB = intersect;
            }
            else
            {
                _splitPointA = intersect;
            }
        }
    }

    //std::cout << "Split Point A:" << _splitPointA.x << "x - " << _splitPointA.y << "y \n";
    //std::cout << "Split Point B:" << _splitPointB.x << "x - " << _splitPointB.y << "y \n";

}

void Asteroid::Draw( sf::RenderWindow& winParam )
{
    for(int i = 0; i < _asteroidShape.getVertexCount(); i++)
        _asteroidShape[i].color = sf::Color::Red;

    winParam.draw( _asteroidShape );


    sf::VertexArray outline;
    outline = _asteroidShape;
    outline.setPrimitiveType(sf::LinesStrip);

    /// outline.append(outline[0]) crashes.
    outline.resize(outline.getVertexCount() + 1);
    outline[outline.getVertexCount() - 1].position = outline[0].position;

    for(int i = 0; i < outline.getVertexCount(); i++)
    {
        outline[i].color = sf::Color(128, 0, 0);
    }

    winParam.draw( outline );

}

int Asteroid::BulletCollision ( std::vector<Bullet*> &bVec, float dTParam )
{
    int vToCheck;
    int collideBullet = NOT_SETTED;

    for( int i = 0; i < bVec.size(); i++ )
    {
        for( int j = 0; j < _asteroidShape.getVertexCount(); j++)
        {
            if(j < _asteroidShape.getVertexCount() - 1)
                vToCheck = j + 1;
            else
                vToCheck = 0;

            //DistancePointLine(_asteroidShape[j].position.x, _asteroidShape[j].position.y, _asteroidShape[vToCheck].position.x, _asteroidShape[vToCheck].position.y, bVec[i]->GetPosition().x, bVec[i]->GetPosition().y)

            if( PointOfIntersection(_asteroidShape[j].position, _asteroidShape[vToCheck].position, bVec[i]->previousPoint, bVec[i]->GetPosition()).x > NOT_SETTED &&
                DistancePointLine(_asteroidShape[j].position.x, _asteroidShape[j].position.y, _asteroidShape[vToCheck].position.x, _asteroidShape[vToCheck].position.y, bVec[i]->GetPosition().x, bVec[i]->GetPosition().y) <= bulletSpeed * dTParam)
            {
                collideBullet = i;
            }
        }
    }

    return collideBullet;
}

sf::VertexArray Asteroid::Split( )
{
    int vToCheck = 0;
    bool firstPlaced = false;
    std::vector<sf::Vector2f> nVertices;

    for( int i = 0; i < _asteroidShape.getVertexCount(); i++)
    {
        nVertices.push_back( _asteroidShapeVertices[i] );

        if(i < _asteroidShape.getVertexCount() - 1)
            vToCheck = i + 1;
        else
            vToCheck = 0;

        if(!firstPlaced)
        {
            if( DistancePointLine(_asteroidShape[i].position.x, _asteroidShape[i].position.y,
                                  _asteroidShape[vToCheck].position.x, _asteroidShape[vToCheck].position.y,
                                  _splitPointA.x, _splitPointA.y) <= .01 )
            {
                _splitPointA.x -= _x;
                _splitPointA.y -= _y;

                nVertices.push_back( _splitPointA);
                firstPlaced = true;
            }
        }
        else
        {

            if( DistancePointLine(_asteroidShape[i].position.x, _asteroidShape[i].position.y,
                                  _asteroidShape[vToCheck].position.x, _asteroidShape[vToCheck].position.y,
                                  _splitPointB.x, _splitPointB.y) <= .01 )
            {
                _splitPointB.x -= _x;
                _splitPointB.y -= _y;

                nVertices.push_back( _splitPointB);
            }

        }

    }

    sf::VertexArray aSplitted;
    bool foundEndSplit = false;

    for( int i = 0; i < nVertices.size(); i++)
    {

        if(!foundEndSplit)
        {
            aSplitted.append(nVertices[i]);
        }

        if( _splitPointA.x == nVertices[i].x && _splitPointA.y == nVertices[i].y )
        {
            foundEndSplit = true;
        }

        if( _splitPointB.x == nVertices[i].x && _splitPointB.y == nVertices[i].y )
        {
            foundEndSplit = false;
            aSplitted.append(nVertices[i]);
        }
    }


    int resCounter = 0;
    _asteroidShape.clear();
    _asteroidShapeVertices.clear();
    bool startedFromSplit = false;

    for( int i = 0; i < nVertices.size(); i++)
    {
        if(!startedFromSplit)
        {
            if( _splitPointA.x == nVertices[i].x && _splitPointA.y == nVertices[i].y )
            {
                _asteroidShapeVertices.push_back(nVertices[i]);
                resCounter ++;
                startedFromSplit = true;
            }
        }
        else
        {
            _asteroidShapeVertices.push_back(nVertices[i]);
            resCounter ++;
            if( _splitPointB.x == nVertices[i].x && _splitPointB.y == nVertices[i].y )
                break;
        }
    }

    _asteroidShape.resize(resCounter);
    for( int i = 0; i < _asteroidShapeVertices.size(); i++)
    {
        _asteroidShape[i].position.x = _asteroidShapeVertices[i].x +_x;
        _asteroidShape[i].position.y = _asteroidShapeVertices[i].y +_y;
    }

    return aSplitted;
}
