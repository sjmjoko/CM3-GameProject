#ifndef OBJECTKPLR_H
#define OBJECTKPLR_H

#include<allegro5\allegro.h>
#include<allegro5\allegro_primitives.h>
#include<allegro5\allegro_image.h> 

using namespace std;
class objectKPLR
{
public:
	objectKPLR(){};
	~objectKPLR(){};

	int x;
	int y;
	int degree;
	int lives;
	bool live;
	int speed;
	int boundx; // this is the box that detects bounds
	int boundy;
	int score;

	int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth;
	int frameHeight;

	int animationColumns;
	int animationDirection;
	int animationRow;
	ALLEGRO_BITMAP *image;
};
#endif