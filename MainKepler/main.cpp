#include<allegro5\allegro.h>
#include<allegro5\allegro_primitives.h>
#include<allegro5\allegro_image.h> 
#include<iostream>
#include "objects.h"
using namespace std;


//PUBLIC 
const int WIDTH = 640;
const int HEIGHT = 400;
enum KEYS{ A, LEFT, RIGHT };
bool keys[3] = { false, false, false };

//PROTOTYPES
void InitShip(SpaceShip &ship, ALLEGRO_BITMAP *image);
void DrawShip(SpaceShip &ship);
void RotateRight(SpaceShip &ship);
void Accelerate(SpaceShip &ship);
void RotateLeft(SpaceShip &ship);
void ResetShipAnimation(SpaceShip &ship);
void CollideComet(SpaceShip &ship);
void InvisibleShield(SpaceShip &ship);
void SlowDown(SpaceShip &ship);
void Explosion(SpaceShip &ship);



int main()
{
	//PRIMITIVE VAR
	int imagew = 0;
	int imageh = 0;
	int x = 0;
	int y = 0;
	int lives = 3;
	int boundx = 20;
	int boundy = 30;

	int imagew_R = 0;
	int imageh_R = 0;
	int xx = 0;
	int yy = 0;
	int boundxx = 18;
	int boundyy = 18;

	int done = false;
	float degree = 0;
	int imageRad = 20;
	const int fps = 60;
	bool redraw = true;
	bool isGameOver = false;

	//classes
	SpaceShip ship;

	//ALLEGRO OBJECTS
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_BITMAP *image = NULL;
	ALLEGRO_BITMAP *image_R = NULL;
	ALLEGRO_TIMER *timer = NULL;



	if (!al_init())
	{
		return -1;
	}

	//CREATE DISPLAY OBJECTS
	display = al_create_display(WIDTH, HEIGHT);

	if (!display)
	{
		return -1;
	}

	//INTIALIZE EXTERNAL/INTERNAL ADDS ON
	al_init_primitives_addon();	// allows us to use allegro primitives 
	al_install_keyboard();	// allows us to use keyboard inputs
	al_init_image_addon(); // the addon that allows us to work wit different image types

	//CREATE IMAGE HERE
	image = al_load_bitmap("Kepler-sprite.png");
	al_convert_mask_to_alpha(image, al_map_rgb(255, 0, 255)); //clear background 
	image_R = al_load_bitmap("rocky.png");
	//CREATE TIMER 
	timer = al_create_timer(1.0 / fps);
	//INITIALIZE OBJECTS 
	InitShip(ship, image);
	//CREATE QUEUE
	event_queue = al_create_event_queue();

	//EVENT QUEUE REGISTER
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));

	//GET IMAGE PROPERTIES
	imagew = al_get_bitmap_width(image);
	imageh = al_get_bitmap_height(image);
	imagew_R = al_get_bitmap_width(image_R);
	imageh_R = al_get_bitmap_height(image_R);

	int temph = HEIGHT;
	int tempw = WIDTH;

	//////////////////////True X AND Y ,with the middle as our (0,0) 
	//x = WIDTH / 2 - imagew/2; // displays the x pos of the bitmap to the left by imagew/2  ---> WIDTH/2 is our origin
	//y = HEIGHT / 2- imageh/2;
	xx = WIDTH / 2 - imagew_R / 2;
	yy = HEIGHT / 2 - imageh_R / 2;





	cout << WIDTH / 2 << " - " << imagew / 2 << " = " << x << endl;
	cout << temph << "   " << tempw << endl;

	al_start_timer(timer); // start your timer
	//GAMELOOP
	while (!done)
	{

		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;


			/*x = ship.x / 2;
			y = ship.y / 2;*/

			if (keys[LEFT])
			{
				RotateLeft(ship);
				/*degree -= 5;
				if (degree <= 0)
				{
				degree = 360;
				}
				cout << degree << endl;*/
			}

			else if (keys[RIGHT])
			{
				RotateRight(ship);
				/*degree += 5;
				if (degree >= 360)
				{
				degree = 0;
				}
				cout << degree << endl;*/
			}
			else
			{
				ResetShipAnimation(ship);
			}
			if (keys[A])
			{
				Accelerate(ship);
				//if ((temph > 49 && temph <= 750) && (tempw > 49 && tempw <= 1230))
				//{
				//	if ((degree >= 340 && degree <= 360) || (degree >= 0 && degree <= 20)) //MOVE UP
				//	{
				//		temph -= 5;

				//		if (temph < -5) temph = 2 * HEIGHT;
				//		if (temph > 2 * HEIGHT) temph = -5 ;
				//		if (tempw > 2*WIDTH)tempw = -5;
				//		if (tempw <-5)tempw = 2 * WIDTH;

				//		cout << temph << "   " << temph << endl;
				//	}
				//	if (degree >= 160 && degree <= 200)	// MOVE DOWN 
				//	{
				//		temph += 5;
				//		if (temph < -5) temph = 2 * HEIGHT;
				//		if (temph > 2 * HEIGHT) temph = -5;
				//		if (tempw > 2 * WIDTH)tempw = -5;
				//		if (tempw <-5)tempw = 2 * WIDTH;
				//		
				//		cout << temph << "   " << tempw << endl;
				//	}
				//	if (degree > 0 && degree < 90) // MOVE NORTH EAST
				//	{
				//		temph -= 5;
				//		tempw += 5;

				//		if (temph < -5) temph = 2 * HEIGHT;
				//		if (temph > 2 * HEIGHT) temph = -5;
				//		if (tempw > 2 * WIDTH)tempw = -5;
				//		if (tempw <-5)tempw = 2 * WIDTH;
				//		cout << temph << "   " << tempw << endl;
				//	}
				//	if (degree > 90 && degree < 180) //MOVE SOUTH EAST
				//	{
				//		temph += 5;
				//		tempw += 5;
				//		if (temph < -5) temph = 2 * HEIGHT;
				//		if (temph > 2 * HEIGHT) temph = -5;
				//		if (tempw > 2 * WIDTH)tempw = -5;
				//		if (tempw <-5)tempw = 2 * WIDTH;
				//		cout << temph << "   " << tempw << endl;
				//	}
				//	if (degree > 180 && degree < 270) //MOVE SOUTH WEST
				//	{
				//		temph += 5;
				//		tempw -= 5;
				//		if (temph <= 0) temph = 2 * HEIGHT;
				//		if (temph >= 2 * HEIGHT + 1) temph = 45;
				//		if (tempw >= 1300)tempw = -5;
				//		cout << temph << "   " << tempw << endl;
				//	}
				//	if (degree > 270 && degree < 360) // MOVE NORTH WEST
				//	{
				//		temph -= 5;
				//		tempw -= 5;
				//		if (temph < -5) temph = 2 * HEIGHT;
				//		if (temph > 2 * HEIGHT) temph = -5;
				//		if (tempw > 2 * WIDTH)tempw = -5;
				//		if (tempw <-5)tempw = 2 * WIDTH;
				//		cout << temph << "   " << tempw << endl;
				//	}
				//	if (degree >= 70 && degree <= 110) //MOVE RIGHT
				//	{
				//		tempw += 5;
				//		if (temph < -5) temph = 2 * HEIGHT;
				//		if (temph > 2 * HEIGHT) temph = -5;
				//		if (tempw > 2 * WIDTH)tempw = -5;
				//		if (tempw <-5)tempw = 2 * WIDTH;
				//		
				//		cout << temph << "   " << tempw << endl;
				//	}
				//	if (degree >= 250 && degree <= 290)//MOVE LEFT
				//	{
				//		tempw -= 5;
				//		if (temph < -5) temph = 2 * HEIGHT;
				//		if (temph > 2 * HEIGHT) temph = -5;
				//		if (tempw > 2 * WIDTH)tempw = -5;
				//		if (tempw <-5)tempw = 2 * WIDTH;
				//	
				//		cout << temph << "   " << tempw << endl;
				//	}
				//	
				//}
			}

			if (!isGameOver)
			{
				// copy this to collide function 
				if (30 - boundxx < ship.x / 2 + ship.boundx && 30 + boundxx > ship.x / 2 - ship.boundx && HEIGHT / 2 - boundyy < ship.y / 2 + ship.boundy && HEIGHT / 2 + boundyy > ship.y / 2 - ship.boundy)
				{
					ship.lives--;

					ship.x = WIDTH;
					ship.y = HEIGHT;
					ship.degree = 0;

				}
				if (ship.lives <= 0)
				{
					isGameOver = true;
				}
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)// when key is pressed
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_A:
				keys[A] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) /// when key is released
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_A:
				keys[A] = false;
				SlowDown(ship);
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			}

		}
		//DISPAYING THE PRODUCT
		if (redraw && al_is_event_queue_empty(event_queue))
		{

			redraw = false;
			if (!isGameOver)
			{
				DrawShip(ship);
				al_draw_line(WIDTH / 2, 0, WIDTH / 2, HEIGHT, al_map_rgb(199, 4, 30), 5);
				al_draw_bitmap(image_R, (xx - WIDTH / 2) + 30, yy, 0);
				/*al_draw_rotated_bitmap(image, imagew / 2, imageh / 2, tempw / 2, temph / 2, degree*3.14259 / 180, 0);*/

				//al_draw_filled_rectangle(tempw/2 - boundx, temph/2 - boundy, tempw/2 + boundx, temph/2 + boundy, al_map_rgba(255, 0, 255, 100)); //	boundary box;
				//al_draw_filled_rectangle(30 - boundxx, HEIGHT / 2 - boundyy, 30 + boundxx, HEIGHT / 2 + boundxx, al_map_rgba(255, 0, 255, 100));

			}
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}

	}

	//DESTROY OBJECTS
	al_destroy_bitmap(image);
	al_destroy_bitmap(image_R);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	return 0;


}

void InitShip(SpaceShip &ship, ALLEGRO_BITMAP *image)
{
	ship.x = WIDTH;
	ship.y = HEIGHT;
	ship.ID = PLAYER;
	ship.lives = 4;
	ship.speed = 6;
	ship.degree = 90;
	ship.boundx = 10;
	ship.boundy = 12;
	ship.score = 0;

	ship.maxFrame = 1;
	ship.curFrame = 0;
	ship.frameCount = 0;
	ship.frameDelay = 50;
	ship.frameWidth = 48; //of each block
	ship.frameHeight = 43;//of each block

	ship.animationColumns = 1;
	ship.animationDirection = 1;
	ship.animationRow = 0;

	ship.image = image;
}
void DrawShip(SpaceShip &ship)
{
	int fx = (ship.curFrame % ship.animationColumns)*ship.frameWidth;
	int fy = ship.animationRow*ship.frameHeight;

	ALLEGRO_BITMAP *sub_bitmap = NULL;
	sub_bitmap = al_create_sub_bitmap(ship.image, fx, fy, ship.frameWidth, ship.frameHeight);
	al_draw_rotated_bitmap(sub_bitmap, ship.frameWidth / 2, ship.frameHeight / 2, ship.x / 2, ship.y / 2, ship.degree*3.14259 / 180, 0);
}
void RotateRight(SpaceShip &ship)
{
	ship.animationRow = 2;
	ship.degree += 5;
	if (ship.degree >= 360)
	{
		ship.degree = 0;
	}
}
void Accelerate(SpaceShip &ship)
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
		/*if (temph <= 0) temph = 2 * HEIGHT;
		if (temph >= 2 * HEIGHT + 1) temph = 45;
		if (tempw >= 1300)tempw = -5;*/
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
void RotateLeft(SpaceShip &ship)
{
	ship.animationRow = 2;
	ship.degree -= 5;
	if (ship.degree <= 0)
	{
		ship.degree = 360;
	}
}
void ResetShipAnimation(SpaceShip &ship)
{
	ship.animationRow = 0;

}
void CollideComet(SpaceShip &ship)
{
	//waiting for sthera with his rocks
	/*if (30 - boundxx < x + boundx && 30 + boundxx > x - boundx && HEIGHT / 2 - boundyy < y + boundy && HEIGHT / 2 + boundyy > y - boundy)
	{
	lives--;

	tempw = WIDTH;
	temph = HEIGHT;
	degree = 0;

	}*/
}
void InvisibleShield(SpaceShip &ship)
{
}
void SlowDown(SpaceShip &ship)
{
	int j;
	for (int i = 0; i < 200; i++)
	{
		j = 0;
		while (j <= 200000)
		{
			j++;
		}
		ship.animationRow = 1;

		if ((ship.degree >= 340 && ship.degree <= 360) || (ship.degree >= 0 && ship.degree <= 20)) //MOVE UP
		{
			ship.y -= 1;

			if (ship.y < -5) ship.y = 2 * HEIGHT;
			if (ship.y > 2 * HEIGHT) ship.y = -5;
			if (ship.x > 2 * WIDTH)ship.x = -5;
			if (ship.x < -5)ship.x = 2 * WIDTH;
		}
		if (ship.degree >= 160 && ship.degree <= 200)	// MOVE DOWN 
		{
			ship.y += 1;
			if (ship.y < -5) ship.y = 2 * HEIGHT;
			if (ship.y > 2 * HEIGHT) ship.y = -5;
			if (ship.x > 2 * WIDTH)ship.x = -5;
			if (ship.x < -5)ship.x = 2 * WIDTH;
		}
		if (ship.degree > 0 && ship.degree < 90) // MOVE NORTH EAST
		{
			ship.y -= 1;
			ship.x += 1;

			if (ship.y < -5) ship.y = 2 * HEIGHT;
			if (ship.y > 2 * HEIGHT) ship.y = -5;
			if (ship.x > 2 * WIDTH)ship.x = -5;
			if (ship.x < -5)ship.x = 2 * WIDTH;
		}
		if (ship.degree > 90 && ship.degree < 180) //MOVE SOUTH EAST
		{
			ship.y += 1;
			ship.x += 1;
			if (ship.y < -5) ship.y = 2 * HEIGHT;
			if (ship.y > 2 * HEIGHT) ship.y = -5;
			if (ship.x > 2 * WIDTH)ship.x = -5;
			if (ship.x < -5)ship.x = 2 * WIDTH;
		}
		if (ship.degree > 180 && ship.degree < 270) //MOVE SOUTH WEST
		{
			ship.y += 1;
			ship.x -= 1;
			if (ship.y < -5) ship.y = 2 * HEIGHT;
			if (ship.y > 2 * HEIGHT) ship.y = -5;
			if (ship.x > 2 * WIDTH)ship.x = -5;
			if (ship.x < -5)ship.x = 2 * WIDTH;
		}
		if (ship.degree > 270 && ship.degree < 360) // MOVE NORTH WEST
		{
			ship.y -= 1;
			ship.x -= 1;
			if (ship.y < -5) ship.y = 2 * HEIGHT;
			if (ship.y > 2 * HEIGHT) ship.y = -5;
			if (ship.x > 2 * WIDTH)ship.x = -5;
			if (ship.x < -5)ship.x = 2 * WIDTH;
		}
		if (ship.degree >= 70 && ship.degree <= 110) //MOVE RIGHT
		{
			ship.x += 1;
			if (ship.y < -5) ship.y = 2 * HEIGHT;
			if (ship.y > 2 * HEIGHT) ship.y = -5;
			if (ship.x > 2 * WIDTH)ship.x = -5;
			if (ship.x < -5)ship.x = 2 * WIDTH;

		}
		if (ship.degree >= 250 && ship.degree <= 290)//MOVE LEFT
		{
			ship.x -= 1;
			if (ship.y < -5) ship.y = 2 * HEIGHT;
			if (ship.y > 2 * HEIGHT) ship.y = -5;
			if (ship.x > 2 * WIDTH)ship.x = -5;
			if (ship.x < -5)ship.x = 2 * WIDTH;
		}

	}
}//Gota work on this.