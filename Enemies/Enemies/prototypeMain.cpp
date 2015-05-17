#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include< allegro5\allegro_image.h>
#include "Object.h"
#include<vector>
#include <iostream>
using namespace std;

//GLOBALS==============================
const int WIDTH = 800;
const int HEIGHT = 400;
const int NUM_BULLETS = 10;
const int NUM_ENEMY = 2;
enum KEYS{ UP, DOWN, LEFT, RIGHT, SPACE };
bool keys[5] = { false, false, false, false, false };

//prototypes
void InitShip(SpaceShip &ship);
void DrawShip(SpaceShip &ship);
void MoveShipUp(SpaceShip &ship);
void MoveShipDown(SpaceShip &ship);
void MoveShipLeft(SpaceShip &ship);
void MoveShipRight(SpaceShip &ship);

void InitBullet(Bullet bullet[], int size);
void DrawBullet(Bullet bullet[], int size);
void FireBullet(Bullet bullet[], int size, SpaceShip &ship);
void UpdateBullet(Bullet bullet[], int size);
void CollideBulletEnemy(Bullet bullet[], int bSize, Enemy enemy[], int eSize, SpaceShip &ship);

void InitEnemy(Enemy enemy[], int size, ALLEGRO_BITMAP *image);
void DrawEnemy(Enemy enemy[], int size);
void StartEnemy(Enemy enemy[], int size);
void UpdateEnemy(Enemy enemy[], int size);
void CollideEnemy(Enemy enemy[], int cSize, SpaceShip &ship);

int main(void)
{
	//primitive variable
	bool done = false;
	bool redraw = true;
	const int FPS = 60;
	bool isGameOver = false;

	int x = WIDTH / 2, y = HEIGHT / 2;

	const int maxFrame = 8;
	int curFrame = 0;
	int FrameCount = 0;
	int FrameDelay = 5;
	int FrameWidth = 150;
	int FrameHEIGHT = 150;

	//object variables
	SpaceShip ship;
	Bullet bullets[NUM_BULLETS];

	Enemy enemy[NUM_ENEMY];

	//Allegro variables
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *font18 = NULL;
	ALLEGRO_BITMAP  *enemyImage = NULL;

	//Initialization Functions
	if (!al_init())										//initialize Allegro
		return -1;

	display = al_create_display(WIDTH, HEIGHT);			//create our display object

	if (!display)										//test display object
		return -1;

	al_init_primitives_addon();
	al_install_keyboard();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);

	enemyImage = al_load_bitmap("sprite_sheet_demo.bmp");
	al_convert_mask_to_alpha(enemyImage, al_map_rgb(106, 76, 48));

	srand(time(NULL));
	InitShip(ship);
	InitBullet(bullets, NUM_BULLETS);
	InitEnemy(enemy, NUM_ENEMY, enemyImage);

	font18 = al_load_font("m-engravers-old-english-wd-1361498189.ttf", 18, 0);


	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_start_timer(timer);
	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			if (++FrameCount >= FrameDelay)
			{
				if (++curFrame >= FrameDelay)
					curFrame = 0;

				FrameCount = 0;
			}
			x = -5;

			if (x <= 0 - FrameWidth)
				x = WIDTH;

			redraw = true;
			if (keys[UP])
				MoveShipUp(ship);
			if (keys[DOWN])
				MoveShipDown(ship);
			if (keys[LEFT])
				MoveShipLeft(ship);
			if (keys[RIGHT])
				MoveShipRight(ship);

			if (!isGameOver)
			{
				UpdateBullet(bullets, NUM_BULLETS);
				StartEnemy(enemy, NUM_ENEMY);
				UpdateEnemy(enemy, NUM_ENEMY);
				CollideBulletEnemy(bullets, NUM_BULLETS, enemy, NUM_ENEMY, ship);
				CollideEnemy(enemy, NUM_ENEMY, ship);

				if (ship.lives <= 0)
					isGameOver = true;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = true;
				FireBullet(bullets, NUM_BULLETS, ship);
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = false;
				break;
			}
		}

		if (redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;

			if (!isGameOver)
			{
				DrawShip(ship);
				DrawBullet(bullets, NUM_BULLETS);
				DrawEnemy(enemy, NUM_ENEMY);

				al_draw_textf(font18, al_map_rgb(255, 0, 255), 5, 5, 0, "Player has %i lives left. Player has destroyed %i objects", ship.lives, ship.score);
			}
			else
			{
				al_draw_textf(font18, al_map_rgb(0, 255, 255), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "Game Over. Final Score: %i", ship.score);
			}

			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
	}

	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_font(font18);
	al_destroy_bitmap(enemyImage);
	al_destroy_display(display);						//destroy our display object

	return 0;
}

void InitShip(SpaceShip &ship)
{
	ship.x = 20;
	ship.y = HEIGHT / 2;
	ship.ID = PLAYER;
	ship.lives = 3;
	ship.speed = 7;
	ship.boundx = 6;
	ship.boundy = 7;
	ship.score = 0;
}
void DrawShip(SpaceShip &ship)
{
	al_draw_filled_rectangle(ship.x, ship.y - 9, ship.x + 10, ship.y - 7, al_map_rgb(255, 0, 0));
	al_draw_filled_rectangle(ship.x, ship.y + 9, ship.x + 10, ship.y + 7, al_map_rgb(255, 0, 0));

	al_draw_filled_triangle(ship.x - 12, ship.y - 17, ship.x + 12, ship.y, ship.x - 12, ship.y + 17, al_map_rgb(0, 255, 0));
	al_draw_filled_rectangle(ship.x - 12, ship.y - 2, ship.x + 15, ship.y + 2, al_map_rgb(0, 0, 255));
}
void MoveShipUp(SpaceShip &ship)
{
	ship.y -= ship.speed;
	if (ship.y < 0)
		ship.y = 0;
}
void MoveShipDown(SpaceShip &ship)
{
	ship.y += ship.speed;
	if (ship.y > HEIGHT)
		ship.y = HEIGHT;
}
void MoveShipLeft(SpaceShip &ship)
{
	ship.x -= ship.speed;
	if (ship.x < 0)
		ship.x = 0;
}
void MoveShipRight(SpaceShip &ship)
{
	ship.x += ship.speed;
	if (ship.x > 300)
		ship.x = 300;
}

void InitBullet(Bullet bullet[], int size)
{
	for (int i = 0; i < size; i++)
	{
		bullet[i].ID = BULLET;
		bullet[i].speed = 5;
		bullet[i].live = false;
	}
}
void DrawBullet(Bullet bullet[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (bullet[i].live)
			al_draw_filled_circle(bullet[i].x, bullet[i].y, 2, al_map_rgb(255, 255, 255));
	}
}
void FireBullet(Bullet bullet[], int size, SpaceShip &ship)
{
	for (int i = 0; i < size; i++)
	{
		if (!bullet[i].live)
		{
			bullet[i].x = ship.x + 17;
			bullet[i].y = ship.y;
			bullet[i].live = true;
			break;
		}
	}
}
void UpdateBullet(Bullet bullet[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (bullet[i].live)
		{
			bullet[i].x += bullet[i].speed;
			if (bullet[i].x > WIDTH)
				bullet[i].live = false;
		}
	}
}
void CollideBulletEnemy(Bullet bullet[], int bSize, Enemy enemy[], int eSize, SpaceShip &ship)
{
	for (int i = 0; i < bSize; i++)
	{
		if (bullet[i].live)
		{
			for (int j = 0; j < eSize; j++)
			{
				if (enemy[j].live)
				{
					if (bullet[i].x >(enemy[j].x - enemy[j].boundx) &&
						bullet[i].x < (enemy[j].x + enemy[j].boundx) &&
						bullet[i].y >(enemy[j].y - enemy[j].boundy) &&
						bullet[i].y< (enemy[j].y + enemy[j].boundy))
					{
						bullet[i].live = false;
						enemy[j].live = false;

						ship.score++;
					}
				}
			}
		}
	}
}

void InitEnemy(Enemy enemy[], int size, ALLEGRO_BITMAP *image)
{
	for (int i = 0; i < size; i++)
	{
		enemy[i].ID = ENEMY;
		enemy[i].live = false;
		enemy[i].speed = 3;
		enemy[i].boundx = 15;
		enemy[i].boundy = 15;

		enemy[i].maxframe = 6;
		enemy[i].curframe = 0;
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
void DrawEnemy(Enemy enemy[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (enemy[i].live)
		{

			int fx = (enemy[i].curframe % enemy[i].animationColumns)*enemy[i].frameWidth;
			int fy = (enemy[i].curframe / enemy[i].animationColumns)*enemy[i].frameHeight;

			al_draw_bitmap_region(enemy[i].image, fx, fy, enemy[i].frameWidth,
				enemy[i].frameHeight, enemy[i].x - enemy[i].frameWidth / 2, enemy[i].y - enemy[i].frameHeight / 2, 0);

			/*al_draw_filled_rectangle(comets[i].x - comets[i].boundx, comets[i].y - comets[i].boundy, comets[i].x + comets[i].boundx,
			comets[i].y + comets[i].boundy, al_map_rgba(255, 0, 255, 100));

			//al_draw_filled_circle(comets[i].x, comets[i].y, 20, al_map_rgb(255, 0, 0));*/
		}
	}
}
void StartEnemy(Enemy enemy[], int size)
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
void UpdateEnemy(Enemy enemy[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (enemy[i].live)
		{
			if (++enemy[i].x >= enemy[i].frameDelay)
			{
				enemy[i].curframe += enemy[i].animationDirection;
				if (enemy[i].curframe >= enemy[i].maxframe)
					enemy[i].curframe = 0;
				else if (enemy[i].curframe <= 0)
					enemy[i].curframe = enemy[i].maxframe - 1;

				enemy[i].frameCount = 0;
			}

			enemy[i].x -= enemy[i].speed;
		}
	}
}
void CollideEnemy(Enemy enemy[], int cSize, SpaceShip &ship)
{
	for (int i = 0; i < cSize; i++)
	{
		if (enemy[i].live)
		{
			if (enemy[i].x - enemy[i].boundx < ship.x + ship.boundx &&
				enemy[i].x + enemy[i].boundx > ship.x - ship.boundx &&
				enemy[i].y - enemy[i].boundy < ship.y + ship.boundy &&
				enemy[i].y + enemy[i].boundy > ship.y - ship.boundy)
			{
				ship.lives--;
				enemy[i].live = false;
			}
			else if (enemy[i].x < 0)
			{
				enemy[i].live = false;
				//ship.lives--;
			}
		}
	}
}
