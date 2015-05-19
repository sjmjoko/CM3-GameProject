#ifndef BULLET_H
#define BULLET_H

#include<allegro5\allegro.h>
#include<allegro5\allegro_primitives.h>
#include<allegro5\allegro_image.h> 

class Bullet
{
public:
	Bullet(){};
	~Bullet(){};

	int ID;
	int x;
	int y;
	int xx;
	int yy;
	int degree;
	bool live;
	int speed;

	ALLEGRO_BITMAP *image;

private:

};

#endif

