#ifndef ENEMY_H
#define ENEMY_H

#include<allegro5\allegro.h>
#include<allegro5\allegro_primitives.h>
#include<allegro5\allegro_image.h> 

class Enemy
{
public:
	Enemy();
	~Enemy();

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

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

#endif;