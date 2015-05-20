#ifndef ENEMYSHIP_H
#define ENEMYSHIP_H

#include<allegro5\allegro.h>
#include<allegro5\allegro_primitives.h>
#include<allegro5\allegro_image.h> 
#include "objectKPLR.h"
#include "Bullet.h"
#include "GameStates.h"
#include "Asteroid.h"
class Enemyship
{
public:
	Enemyship(GameStates &);
	~Enemyship();
	ALLEGRO_BITMAP *enemyImage;
	//ship bullets
	void initBullets(Bullet[], int);
	void drawBullets(Bullet[]);
	void fireBullets(Bullet[], int, objectKPLR [],objectKPLR&);
	void collide(objectKPLR[], Bullet[], int, objectKPLR&, objectKPLR[], int, Asteroid&);
	void collideEBullet(Bullet[], int, objectKPLR &, objectKPLR[], int, Asteroid&);
	void updateBullet(Bullet[], objectKPLR[], objectKPLR&);

	//ship movement
	void initship(objectKPLR[],int,ALLEGRO_BITMAP*);
	void drawship(objectKPLR[],int );
	void startEnemy(objectKPLR[],int);
	void updateEnemy(objectKPLR[],int, objectKPLR&);
	
	//


	const int ENEMY = 2;
	const int Bullets = 8;
	int iRand;

protected:
	int HEIGHT;
	int WIDTH;
	
};

#endif