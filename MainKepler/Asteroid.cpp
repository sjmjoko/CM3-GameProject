#include "Asteroid.h"

Asteroid::Asteroid()
{

}
void Asteroid::InitComet(objectKPLR comets[], int size, ALLEGRO_BITMAP *image)
{


	for (int i = 0; i < size; i++)
	{
		comets[i].live = false;
		comets[i].speed = 1.5;
		comets[i].boundx = 35;
		comets[i].boundy = 35;

		comets[i].maxFrame = 143;
		comets[i].curFrame = 0;
		comets[i].frameCount = 0;
		comets[i].frameDelay = 2;
		comets[i].frameWidth = 96;
		comets[i].frameHeight = 96;
		comets[i].animationColumns = 21;
		comets[i].image = image;
		if (rand())
		{
			comets[i].animationDirection = 1;
		}
		else
		{
			comets[i].animationDirection = -1;
		}
	}
}
void Asteroid::StartComet(objectKPLR comets[], int size)
{
	GameStates object;
	for (int i = 0; i < size; i++)
	{
		if (!comets[i].live)
		{
			if (rand() % 300 == 0)
			{
				comets[i].animationDirection = 3;
				comets[i].live = true;
				comets[i].x = 0;
				comets[i].y = 30 + rand() % (object.getHEIGHT() - 60);
				break;
			}
			if (rand() % 500 == 0)
			{
				comets[i].animationDirection = 5;
				comets[i].live = true;
				comets[i].x = object.getWIDTH();
				comets[i].y = 30 + rand() % (object.getHEIGHT() - 60);
				break;
			}
			if (rand() % 700 == 0)
			{
				comets[i].animationDirection = 7;
				comets[i].live = true;
				comets[i].y = object.getHEIGHT();
				comets[i].x = 30 + rand() % (object.getWIDTH() - 60);
				break;
			}
			if (rand() % 900 == 0)
			{
				comets[i].animationDirection = 9;
				comets[i].live = true;
				comets[i].y = 0;
				comets[i].x = 30 + rand() % (object.getWIDTH() - 60);
				break;
			}
		}
	}
}
void Asteroid::UpdateComet(objectKPLR comets[], int size)
{
	GameStates object;
	for (int i = 0; i < size; i++)
	{
		if (comets[i].live)
		{
			if (++comets[i].frameCount >= comets[i].frameDelay)
			{
				comets[i].curFrame += comets[i].animationDirection;
				if (comets[i].curFrame >= comets[i].maxFrame)
					comets[i].curFrame = 0;
				else if (comets[i].curFrame <= 0)
					comets[i].curFrame = comets[i].maxFrame - 1;

				comets[i].frameCount = 0;
			}
			if (comets[i].animationDirection == 3)
			{
				comets[i].x += comets[i].speed;
				if (comets[i].x == object.getWIDTH())
				{
					comets[i].live = false;
				}
			}
			if (comets[i].animationDirection == 5)
			{
				comets[i].x -= comets[i].speed;
				if (comets[i].x == 0)
				{
					comets[i].live = false;
				}
			}
			if (comets[i].animationDirection == 7)
			{
				comets[i].y -= comets[i].speed;
				if (comets[i].y == 0)
				{
					comets[i].live = false;
				}
			}
			if (comets[i].animationDirection == 9)
			{
				comets[i].y += comets[i].speed;
				if (comets[i].y == object.getHEIGHT())
				{
					comets[i].live = false;
				}
			}
		}
	}
}
void Asteroid::DrawComet(objectKPLR comets[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (comets[i].live)
		{
			int fx = (comets[i].curFrame % comets[i].animationColumns) * comets[i].frameWidth;
			int fy = (comets[i].curFrame / comets[i].animationColumns) * comets[i].frameHeight;
			al_draw_bitmap_region(comets[i].image, fx, fy, comets[i].frameWidth, comets[i].frameHeight, comets[i].x - comets[i].frameWidth / 2, comets[i].y - comets[i].frameHeight / 2, 0);
		}
	}
}
void Asteroid::CollideComet(objectKPLR comets[], int cSize, objectKPLR &ship, objectKPLR explosions[], int eSize, Asteroid com)
{
	GameStates object;
	for (int i = 0; i < cSize; i++)
	{
		if (comets[i].live)
		{
			if (comets[i].x - comets[i].boundx < ship.x/2 + ship.boundx &&
				comets[i].x + comets[i].boundx > ship.x/2 - ship.boundx &&
				comets[i].y - comets[i].boundy < ship.y/2 + ship.boundy &&
				comets[i].y + comets[i].boundy > ship.y/2 - ship.boundy)
			{ 
				com.StartExplosions(explosions, eSize, ship.x/2, ship.y/2);
				ship.x = object.getWIDTH();
				ship.y = object.getHEIGHT();
				ship.degree = 0;
				ship.lives--;
				comets[i].live = false;
				
			}
			else if (comets[i].x < 0)
			{
				comets[i].live = false;
			}
		}
	}
}
bool Asteroid::isReg(objectKPLR comets[], int size)
{
	bool x = true;
	for (int i = 0; i < size; i++)
	{
		if (comets[i].live == true)
		{
			x = false;
		}
	}
	return x;
}
void Asteroid::initExplosions(objectKPLR explosions[], int size, ALLEGRO_BITMAP *image)
{
	for (int i = 0; i < size; i++)
	{
		explosions[i].live = false;

		explosions[i].maxFrame = 31;
		explosions[i].curFrame = 0;
		explosions[i].frameCount = 0;
		explosions[i].frameDelay = 1;
		explosions[i].frameWidth = 128;
		explosions[i].frameHeight = 128;
		explosions[i].animationColumns = 8;
		explosions[i].animationDirection = 1;

		explosions[i].image = image;
	}
}
void Asteroid::DrawExplosions(objectKPLR explosions[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (explosions[i].live)
		{
			int fx = (explosions[i].curFrame % explosions[i].animationColumns) * explosions[i].frameWidth;
			int fy = (explosions[i].curFrame / explosions[i].animationColumns) * explosions[i].frameHeight;

			al_draw_bitmap_region(explosions[i].image, fx, fy, explosions[i].frameWidth,
				explosions[i].frameHeight, explosions[i].x - explosions[i].frameWidth / 2, explosions[i].y - explosions[i].frameHeight / 2, 0);
		}
	}
}
void Asteroid::StartExplosions(objectKPLR explosions[], int size, int x, int y)
{
	for (int i = 0; i < size; i++)
	{
		if (!explosions[i].live)
		{
			explosions[i].live = true;
			explosions[i].x = x;
			explosions[i].y = y;
			break;
		}
	}
}
void Asteroid::UpdateExplosions(objectKPLR explosions[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (explosions[i].live)
		{
			if (++explosions[i].frameCount >= explosions[i].frameDelay)
			{
				explosions[i].curFrame += explosions[i].animationDirection;
				if (explosions[i].curFrame >= explosions[i].maxFrame)
				{
					explosions[i].curFrame = 0;
					explosions[i].live = false;
				}

				explosions[i].frameCount = 0;
			}
		}
	}
}
void Asteroid::destroy()
{
	al_destroy_bitmap(expImage);
	al_destroy_bitmap(cometImage);
}