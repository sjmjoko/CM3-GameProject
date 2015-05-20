#ifndef ASTEROID_H
#define ASTEROID_H

#include "ObjectKPLR.h"
#include "GameStates.h"
#include<allegro5\allegro.h>
#include<allegro5\allegro_primitives.h>
#include<allegro5\allegro_image.h> 
#include<iostream>
#include <cmath>

class Asteroid
{
public:
	Asteroid();
	ALLEGRO_BITMAP *cometImage;
	ALLEGRO_BITMAP *expImage;
	ALLEGRO_SAMPLE *exp = NULL;
	ALLEGRO_SAMPLE_INSTANCE *instanceExp = NULL;

	void InitComet(objectKPLR comets[], int size, ALLEGRO_BITMAP *image);
	void StartComet(objectKPLR comets[], int size);
	void DrawComet(objectKPLR comets[], int size);
	void UpdateComet(objectKPLR comets[], int size);
	bool isReg(objectKPLR comets[], int size);
	void destroy();
	void CollideComet(objectKPLR comets[], int cSize, objectKPLR &ship, objectKPLR explosions[], int eSize, Asteroid com);

	void initExplosions(objectKPLR explosions[], int size, ALLEGRO_BITMAP *images);
	void DrawExplosions(objectKPLR explosions[], int size);
	void StartExplosions(objectKPLR explosions[], int size, int x, int y);
	void UpdateExplosions(objectKPLR explosions[], int size);



};
#endif
