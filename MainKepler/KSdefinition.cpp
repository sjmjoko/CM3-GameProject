#include <iostream>

#include<allegro5\allegro.h>
#include<allegro5\allegro_primitives.h>
#include<allegro5\allegro_image.h> 

#include "Keplership.h"
#include "Bullet.h"
#include "objectKPLR.h"
#include "GameStates.h"
#include "Asteroid.h"


using namespace std;

Keplership::Keplership(GameStates &game)
{
	WIDTH = game.getWIDTH();
	HEIGHT = game.getHEIGHT();
}

Keplership::~Keplership()
{

}

//ship bullets and collision
void Keplership::initBullets(Bullet bullet[], int size)
{
	fire = al_load_sample("fire.wav");
	instanceFire = al_create_sample_instance(fire);
	for (int i = 0; i < size; i++)
	{
		bullet[i].speed = 2;
		bullet[i].live = false;
	}
}
void Keplership::drawBullets(Bullet bullet[], int size, objectKPLR &ship)
{
	for (int i = 0; i < size; ++i)
	{
		al_draw_filled_circle(bullet[i].x, bullet[i].y, 2, al_map_rgb(255, 255, 255));
	}
}
void Keplership::fireBullets(Bullet bullet[], int size, objectKPLR &ship)
{
	int R = 15;

	for (int i = 0; i < size; i++)
	{
		if (!bullet[i].live)
		{
			bullet[i].x = ship.x / 2 + R*(sin(ship.degree*3.14259 / 180)); // start bullet at ref point
			bullet[i].y = ship.y / 2 - R*(cos(ship.degree*3.14259 / 180)); // start bullet at ref point
			bullet[i].xx = ship.x / 2 + R*(sin(ship.degree*3.14259 / 180)); // start bullet at ref point
			bullet[i].yy = ship.y / 2 - R*(cos(ship.degree*3.14259 / 180)); // start bullet at ref point
			bullet[i].live = true;
			break;
		}
	}
}
void Keplership::updateBullet(Bullet bullet[], int size, objectKPLR &ship)
{
	int R = 15;


	for (int i = 0; i < size; ++i)
	{


		if (bullet[i].live)
		{
			bullet[i].x += R*(sin(ship.degree*3.14259 / 180));    //one bullet
			bullet[i].y -= R*(cos(ship.degree*3.14259 / 180));

			if (bullet[i].x >WIDTH || bullet[i].x<0 || bullet[i].y>HEIGHT || bullet[i].y<0)
			{
				bullet[i].live = false;
			}
		}
	}
}
void Keplership::collideCBullet(Bullet bullet[], int bSize, objectKPLR comets[], int cSize, objectKPLR &ship, objectKPLR explosions[], int eSize, Asteroid &com)
{
	for (int i = 0; i < bSize; i++)
	{
		if (bullet[i].live)
		{
			for (int j = 0; j < cSize; j++)
			{ //bulet dont have a bounding box
				if (comets[j].live)
				{	//the bullets x position has to be greater then the comets left side ||| * |
					if (bullet[i].x >(comets[j].x - comets[j].boundx) && bullet[i].y < (comets[j].y + comets[j].boundy) && bullet[i].x <(comets[j].x + comets[j].boundx) && bullet[i].y >(comets[j].y - comets[j].boundy))
					{
						//StartExplosions(explosions, eSize, ship.x, ship.y);
						bullet[i].live = false;
						comets[j].live = false;
						ship.score += 2;
						com.StartExplosions(explosions, eSize, bullet[i].x, bullet[i].y);
					}
				}
			}
		}
	}
}
void Keplership::collideEBullet(Bullet bullet[], Bullet bulletE[], int bSize, objectKPLR enemy[], int eSize, objectKPLR &ship, objectKPLR explosions[], int xSize, Asteroid &com)
{
	for (int i = 0; i < bSize; i++)
	{
		if (bullet[i].live)
		{
			for (int j = 0; j < eSize; j++)
			{ //bulet dont have a bounding box
				if (enemy[j].live)
				{	//the bullets x position has to be greater then the comets left side ||| * |
					if (bullet[i].x >(enemy[j].x - enemy[j].boundx) && bullet[i].y < (enemy[j].y + enemy[j].boundy) && bullet[i].x <(enemy[j].x + enemy[j].boundx) && bullet[i].y >(enemy[j].y - enemy[j].boundy))
					{
						com.StartExplosions(explosions, xSize, bullet[i].x, bullet[i].y);
						bullet[i].live = false;
						bullet[i].x = -10;
						bullet[i].y = -10;
						enemy[j].live = false;
						for (int b = 0; b < 2; b++)
						{
							bulletE[b].x = -10;
							bulletE[b].y = -10;
							bulletE[b].live = false;
						}
						ship.score += 5;

						
					}
				}
			}
		}
	}
}

//ship movements
void Keplership::initKeplership(objectKPLR &ship, ALLEGRO_BITMAP *image)
{
	thrust = al_load_sample("asteroids_thrust.wav");
	instanceThrust = al_create_sample_instance(thrust);
	al_attach_sample_instance_to_mixer(instanceThrust, al_get_default_mixer());
	ship.x = WIDTH;
	ship.y = HEIGHT;
	ship.lives = 4;
	ship.speed = 6;
	ship.degree = 0;     //of each block
	ship.boundx = 10;
	ship.boundy = 12;
	ship.score = 0;

	ship.maxFrame = 1;
	ship.curFrame = 0;
	ship.frameCount = 0;
	ship.frameDelay = 50;
	ship.frameWidth = 48;
	ship.frameHeight = 43;

	ship.animationColumns = 1;
	ship.animationDirection = 1;
	ship.animationRow = 0;

	ship.image = image;
}
void Keplership::drawKeplership(objectKPLR &ship)
{
	int fx = (ship.curFrame % ship.animationColumns)*ship.frameWidth;
	int fy = ship.animationRow*ship.frameHeight;

	ALLEGRO_BITMAP *sub_bitmap = NULL;
	sub_bitmap = al_create_sub_bitmap(ship.image, fx, fy, ship.frameWidth, ship.frameHeight);
	al_draw_rotated_bitmap(sub_bitmap, ship.frameWidth / 2, ship.frameHeight / 2, ship.x / 2, ship.y / 2, ship.degree*3.14259 / 180, 0);
}
void Keplership::rotateRight(objectKPLR &ship)
{
	ship.animationRow = 2;
	ship.degree += 5;
	if (ship.degree >= 360)
	{
		ship.degree = 0;
	}
}
void Keplership::rotateLeft(objectKPLR &ship)
{
	ship.animationRow = 2;
	ship.degree -= 5;
	if (ship.degree <= 0)
	{
		ship.degree = 360;
	}
}
void Keplership::accKeplership(objectKPLR &ship)
{
	ship.animationRow = 1;


	if ((ship.degree >= 340 && ship.degree <= 360) || (ship.degree >= 0 && ship.degree <= 20)) //MOVE UP
	{
		ship.y -= 5;

		if (ship.y < -5) ship.y = 2 * HEIGHT;
		if (ship.y > 2 * HEIGHT) ship.y = -5;
		if (ship.x > 2 * WIDTH)ship.x = -5;
		if (ship.x <-5)ship.x = 2 * WIDTH;
	}
	if (ship.degree >= 160 && ship.degree <= 200)	// MOVE DOWN 
	{
		ship.y += 5;
		if (ship.y < -5) ship.y = 2 * HEIGHT;
		if (ship.y > 2 * HEIGHT) ship.y = -5;
		if (ship.x > 2 * WIDTH)ship.x = -5;
		if (ship.x <-5)ship.x = 2 * WIDTH;
	}
	if (ship.degree > 0 && ship.degree < 90) // MOVE NORTH EAST
	{
		ship.y -= 5;
		ship.x += 5;

		if (ship.y < -5) ship.y = 2 * HEIGHT;
		if (ship.y > 2 * HEIGHT) ship.y = -5;
		if (ship.x > 2 * WIDTH)ship.x = -5;
		if (ship.x <-5)ship.x = 2 * WIDTH;
	}
	if (ship.degree > 90 && ship.degree < 180) //MOVE SOUTH EAST
	{
		ship.y += 5;
		ship.x += 5;
		if (ship.y < -5) ship.y = 2 * HEIGHT;
		if (ship.y > 2 * HEIGHT) ship.y = -5;
		if (ship.x > 2 * WIDTH)ship.x = -5;
		if (ship.x <-5)ship.x = 2 * WIDTH;
	}
	if (ship.degree > 180 && ship.degree < 270) //MOVE SOUTH WEST
	{
		ship.y += 5;
		ship.x -= 5;
		if (ship.y < -5) ship.y = 2 * HEIGHT;
		if (ship.y > 2 * HEIGHT) ship.y = -5;
		if (ship.x > 2 * WIDTH)ship.x = -5;
		if (ship.x <-5)ship.x = 2 * WIDTH;
	}
	if (ship.degree > 270 && ship.degree < 360) // MOVE NORTH WEST
	{
		ship.y -= 5;
		ship.x -= 5;
		if (ship.y < -5) ship.y = 2 * HEIGHT;
		if (ship.y > 2 * HEIGHT) ship.y = -5;
		if (ship.x > 2 * WIDTH)ship.x = -5;
		if (ship.x <-5)ship.x = 2 * WIDTH;
	}
	if (ship.degree >= 70 && ship.degree <= 110) //MOVE RIGHT
	{
		ship.x += 5;
		if (ship.y < -5) ship.y = 2 * HEIGHT;
		if (ship.y > 2 * HEIGHT) ship.y = -5;
		if (ship.x > 2 * WIDTH)ship.x = -5;
		if (ship.x <-5)ship.x = 2 * WIDTH;

	}
	if (ship.degree >= 250 && ship.degree <= 290)//MOVE LEFT
	{
		ship.x -= 5;
		if (ship.y < -5) ship.y = 2 * HEIGHT;
		if (ship.y > 2 * HEIGHT) ship.y = -5;
		if (ship.x > 2 * WIDTH)ship.x = -5;
		if (ship.x < -5)ship.x = 2 * WIDTH;
	}

}
void Keplership::resetKeplership(objectKPLR &ship)
{
	ship.animationRow = 0;
}

bool Keplership::isWon(objectKPLR ship)
{
	if (ship.score >= 50)
		return true;
	else return false;
}






