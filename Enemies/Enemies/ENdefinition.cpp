#include <iostream>
#include <cmath>

#include<allegro5\allegro.h>
#include<allegro5\allegro_primitives.h>
#include<allegro5\allegro_image.h> 

#include "Enemyship.h"
#include "Bullet.h"
#include "objectKPLR.h"
#include "GameStates.h"
#include "Asteroid.h"


using namespace std;

Enemyship::Enemyship(GameStates &game)
{
	WIDTH = game.getWIDTH();
	HEIGHT = game.getHEIGHT();
}

Enemyship::~Enemyship()
{

}

//ship bullets and collisions
void Enemyship::initBullets(Bullet bullet[], int size)
{
	for (int i = 0; i < size; i++)
	{
		bullet[i].speed = 2;
		bullet[i].live = false;
	}
}
void Enemyship::drawBullets(Bullet bullet[])
{
	for (int i = 0; i < 1; ++i)
	{
		al_draw_filled_circle(bullet[i].x, bullet[i].y, 2, al_map_rgb(255, 255, 255));
	}
}
void Enemyship::fireBullets(Bullet bullet[], int size, objectKPLR enemy[],objectKPLR &ship)
{
	for(int i = 0; i < size; i++)
	{
		for (int j = 0; j < 1; ++j)
		{
			if (!bullet[j].live && enemy[i].live)
			{
			
				bullet[j].x = enemy[i].x;
				bullet[j].y = enemy[i].y + 17;
				bullet[j].live = true;
				break;
			}
		}



		updateBullet(bullet, enemy, ship);
	}
}
void Enemyship::updateBullet(Bullet bullet[],objectKPLR[], objectKPLR &ship)
{
	for (int j = 0; j < 1; ++j)
	{
		if (bullet[j].live)
		{
			
				bullet[j].y += bullet[j].speed;
			   //one bullet

			if (bullet[j].x > WIDTH || bullet[j].x<0 || bullet[j].y>HEIGHT || bullet[j].y < 0)
			{
				bullet[j].live = false;
			}
		}
	}
}
void Enemyship::collide(objectKPLR enemy[], Bullet bulletE[], int eSize, objectKPLR &ship, objectKPLR explosions[], int xSize, Asteroid &com)
{
	for (int i = 0; i < eSize; i++)
	{
		if (enemy[i].live)
		{
			if (enemy[i].x - enemy[i].boundx < ship.x / 2 + ship.boundx &&
				enemy[i].x + enemy[i].boundx > ship.x / 2 - ship.boundx &&
				enemy[i].y - enemy[i].boundy < ship.y / 2 + ship.boundy &&
				enemy[i].y + enemy[i].boundy > ship.y / 2 - ship.boundy)
			{
				com.StartExplosions(explosions, xSize, ship.x / 2, ship.y / 2);

				ship.x = WIDTH;
				ship.y = HEIGHT;
				ship.degree = 0;
				ship.lives--;
				enemy[i].live = false;
				for (int b = 0; b < 2; b++)
				{

					bulletE[i].x = -10;
					bulletE[i].y = -10;
					bulletE[b].live = false;
					
				}
			}
			else if (enemy[i].x < 0)
			{
				enemy[i].live = false;
				for (int b = 0; b < 2; b++)
				{
					bulletE[b].live = false;
				}
			}
		}
	}
}
void Enemyship::collideEBullet(Bullet bullet[], int bSize, objectKPLR &ship, objectKPLR explosions[], int xSize, Asteroid &com)
{
	for (int i = 0; i < bSize; i++)
	{
		if (bullet[i].live)
		{
			//bulet dont have a bounding box
			{	//the bullets x position has to be greater then the comets left side ||| * |
				if (bullet[i].x >(ship.x / 2 - ship.boundx) && bullet[i].y < (ship.y / 2 + ship.boundy) && bullet[i].x <(ship.x / 2 + ship.boundx) && bullet[i].y >(ship.y / 2 - ship.boundy))
				{	
					com.StartExplosions(explosions, xSize, bullet[i].x, bullet[i].y);
					bullet[i].live = false;
					bullet[i].x = -10;
					bullet[i].y = -10;
					ship.lives--;
					ship.x = WIDTH;
					ship.y = HEIGHT;
					ship.degree = 0;
				}
			}
		}
	}
}

//ship movements
void Enemyship::initship(objectKPLR enemy[], int size, ALLEGRO_BITMAP *image)
{

	for (int i = 0; i < size; i++)
	{
		enemy[i].live = false;
		enemy[i].speed = 3;
		enemy[i].boundx = 15;
		enemy[i].boundy = 15;

		enemy[i].maxFrame = 6;
		enemy[i].curFrame = 0;
		enemy[i].frameCount = 0;
		enemy[i].frameDelay = 2;
		enemy[i].frameWidth = 40;
		enemy[i].frameHeight = 40;

		enemy[i].animationColumns = 6;

		if (rand() % 2)
			enemy[i].animationDirection = 1;
		else
			enemy[i].animationDirection = -1;

		enemy[i].image = image;
	}
}
void Enemyship::drawship(objectKPLR enemy[],int size)
{
	for (int i = 0; i < size; i++)
	{
		if (enemy[i].live)
		{

			int fx = (enemy[i].curFrame % enemy[i].animationColumns)*enemy[i].frameWidth;
			int fy = (enemy[i].curFrame / enemy[i].animationColumns)*enemy[i].frameHeight;

			al_draw_bitmap_region(enemy[i].image, fx, fy, enemy[i].frameWidth,
				enemy[i].frameHeight, enemy[i].x - enemy[i].frameWidth / 2, enemy[i].y - enemy[i].frameHeight / 2, 0);

		}
	}
}
void Enemyship::updateEnemy(objectKPLR enemy[], int size, objectKPLR &ship)
{
	for (int i = 0; i < size; i++)
	{
		if (enemy[i].live)
		{
			if (++enemy[i].x >= enemy[i].frameDelay)
			{
				enemy[i].curFrame += enemy[i].animationDirection;
				if (enemy[i].curFrame >= enemy[i].maxFrame)
					enemy[i].curFrame = 0;
				else if (enemy[i].curFrame <= 0)
					enemy[i].curFrame = enemy[i].maxFrame - 1;

				enemy[i].frameCount = 0;
			}

			enemy[i].x -= enemy[i].speed;

		}
	}
}
void Enemyship::startEnemy(objectKPLR enemy[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (!enemy[i].live)
		{
			if (rand() % 500 == 0)
			{
				enemy[i].live = true;
				enemy[i].x = WIDTH;
				enemy[i].y = 30 + rand() % (HEIGHT - 60);
				break;
			}
		}
	}
}






