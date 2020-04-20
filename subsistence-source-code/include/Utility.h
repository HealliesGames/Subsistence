#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#define NOT_SETTED -999
#define PI 3.1459

#include <time.h>
#include <fstream>
#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#endif // UTILITY_H_INCLUDED


const int SCENE_WIDTH = 1024,
          SCENE_HEIGHT = 768;

extern bool gameOver, slowDown, fulls, showRestart, rebuilding;
extern int spawnAsteroidSize;
extern int bulletSpeed;

extern sf::Font font;
extern sf::Music msAtmosphere;
extern sf::SoundBuffer sbSplit, sbStart, sbUpgrade, sbSelection, sbDeath, sbMove, sbShoot;
extern sf::Sound snSplit, snStart, snUpgrade, snSelection, snDeath, snMove, snShoot;



void LoadResources( );

float DegToRad( float deg );
float DistancePoint( sf::Vector2f pointA, sf::Vector2f pointB );

/** https://stackoverflow.com/questions/2259476/rotating-a-point-about-another-point-2d **/
sf::Vector2f PointRotation( sf::Vector2f pinPoint, sf::Vector2f point, float angle );

/** geeksforgeeks.org/program-for-point-of-intersection-of-two-lines/ **/
sf::Vector2f PointOfIntersection ( sf::Vector2f lineOneBeg, sf::Vector2f lineOneEnd, sf::Vector2f lineTwoBeg, sf::Vector2f lineTwoEnd );

/** https://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment **/
float DistancePointLine(float x1,float y1,float x2,float y2,float x3,float y3);

void SaveRecord( int record );

int LoadRecord( );

