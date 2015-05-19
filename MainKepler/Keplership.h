#ifndef KEPLERSHIP_H
#define KEPLERSHIP_H

#include<allegro5\allegro.h>
#include<allegro5\allegro_primitives.h>
#include<allegro5\allegro_image.h> 
#include "objectKPLR.h"
#include "Bullet.h"
#include "GameStates.h"
#include "Asteroid.h"

using namespace std;

class Keplership
{
	
public:
	Keplership(GameStates &);
	~Keplership();
	//ship bullets
	ALLEGRO_BITMAP *shipImage;
	void initBullets(Bullet [], int );
	void drawBullets(Bullet[], int, objectKPLR&);
	void fireBullets(Bullet[], int, objectKPLR&);
	void updateBullet(Bullet[], int, objectKPLR&);
	void collideCBullet(Bullet[], int, objectKPLR[], int, objectKPLR&, objectKPLR[], int, Asteroid &);
	void collideEBullet(Bullet[], Bullet[], int, objectKPLR[], int, objectKPLR&,objectKPLR[], int, Asteroid &);
	//ship movement
	void initKeplership(objectKPLR &,ALLEGRO_BITMAP*);
	void drawKeplership(objectKPLR &);
	void rotateRight(objectKPLR &);
	void rotateLeft(objectKPLR &);
	void accKeplership(objectKPLR &);
	void resetKeplership(objectKPLR &);

	const int Bullets = 10;

protected:
	int HEIGHT;
	int WIDTH;
	

};
#endif