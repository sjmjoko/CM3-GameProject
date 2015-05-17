#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_image.h>
#include "Asteroids.h"

const int WIDTH = 800;
const int HEIGHT = 400;
const int NUM_BULLETS = 5;
const int NUM_COMETS = 10;
const int NUM_EXPLOSIONS = 5;

void InitComet(Comet comets[], int size, ALLEGRO_BITMAP *image);
void DrawComet(Comet comets[], int size);
void StartComet(Comet comets[], int size);
void UpdateComet(Comet comets[], int size);

/*void InitExplosions(Explosions explosions[], int size, ALLEGRO_BITMAP *picture);
void DrawExplosions(Explosions explosions[], int size);
void StartExplosions(Explosions explosions[], int size, int x, int y);
void UpdateExplosions(Explosions explosions[], int size);*/

int main()
{
	bool done = false;
	bool redraw = true;
	const int FPS = 60;
	bool isGameOver = false;

	Comet comets[NUM_COMETS];
	//Explosions explosions[NUM_EXPLOSIONS];

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *cometImage;
	//ALLEGRO_BITMAP *expImage;
	if (!al_init())										//initialize Allegro
		return -1;

	display = al_create_display(WIDTH, HEIGHT);			//create our display object

	if (!display)										//test display object
		return -1;

	al_init_primitives_addon();

	al_init_image_addon();

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);

	cometImage = al_load_bitmap("asteroid-1-96.png");
	//expImage = al_load_bitmap("explosion.png");

	srand(time(NULL));
	InitComet(comets, NUM_COMETS, cometImage);
	//InitExplosions(explosions, NUM_EXPLOSIONS, expImage);

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_start_timer(timer);
	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;
			if (!isGameOver)
			{
				StartComet(comets, NUM_COMETS);
				UpdateComet(comets, NUM_COMETS);
				//UpdateExplosions(explosions, NUM_EXPLOSIONS);
			}
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		if (redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;

			if (!isGameOver)
			{
				DrawComet(comets, NUM_COMETS);
				//DrawExplosions(explosions, NUM_EXPLOSIONS);
			}
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
	}
	//al_destroy_bitmap(expImage);
	al_destroy_bitmap(cometImage);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_display(display);
	return 0;
};
void InitComet(Comet comets[], int size, ALLEGRO_BITMAP *image)
{
	for (int i = 0; i < size; i++)
	{
		comets[i].ID = ENEMY;
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
	}
}
void StartComet(Comet comets[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (!comets[i].live)
		{
			if (rand() % 300 == 0)
			{
				comets[i].direction = 3;
				comets[i].live = true;
				comets[i].x = 0;
				comets[i].y = 30 + rand() % (HEIGHT - 60);
				break;
			}
			if (rand() % 500 == 0)
			{
				comets[i].direction = 5;
				comets[i].live = true;
				comets[i].x = WIDTH;
				comets[i].y = 30 + rand() % (HEIGHT - 60);
				break;
			}
			if (rand() % 700 == 0)
			{
				comets[i].direction = 7;
				comets[i].live = true;
				comets[i].y = HEIGHT;
				comets[i].x = 30 + rand() % (WIDTH - 60);
				break;
			}
			if (rand() % 900 == 0)
			{
				comets[i].direction = 9;
				comets[i].live = true;
				comets[i].y = 0;
				comets[i].x = 30 + rand() % (WIDTH - 60);
				break;
			}
		}
	}
}
void UpdateComet(Comet comets[], int size)
{
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
			if (comets[i].direction == 3)
			{
				comets[i].x += comets[i].speed;
			}
			if (comets[i].direction == 5)
			{
				comets[i].x -= comets[i].speed;
			}
			if (comets[i].direction == 7)
			{
				comets[i].y -= comets[i].speed;
			}
			if (comets[i].direction == 9)
			{
				comets[i].y += comets[i].speed;
			}
		}
	}
}
void DrawComet(Comet comets[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (comets[i].live)
		{
			int fx = (comets[i].curFrame % comets[i].animationColumns) * comets[i].frameWidth;
			int fy = (comets[i].curFrame / comets[i].animationColumns) * comets[i].frameHeight;

			al_draw_bitmap_region(comets[i].image, fx, fy, comets[i].frameWidth,
				comets[i].frameHeight, comets[i].x - comets[i].frameWidth / 2, comets[i].y - comets[i].frameHeight / 2, 0);
		}
	}
}