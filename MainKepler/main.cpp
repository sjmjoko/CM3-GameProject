#include<allegro5\allegro.h>
#include<allegro5\allegro_primitives.h>
#include<allegro5\allegro_image.h> 
#include<iostream>
#include <cmath>
#include "objects.h"
using namespace std;


//PUBLIC 
const int WIDTH = 800;
const int HEIGHT = 600;
const int NUM_BULLETS = 10;
const int NUM_ENEMY = 2;
const int NUM_COMETS = 10;
const int NUM_EXPLOSIONS = 5;
enum KEYS{ A, LEFT, RIGHT,S};
bool keys[4] = { false, false, false,false};

//PROTOTYPES
void InitShip(SpaceShip &ship, ALLEGRO_BITMAP *image);
void DrawShip(SpaceShip &ship);
void RotateRight(SpaceShip &ship);
void Accelerate(SpaceShip &ship);
void RotateLeft(SpaceShip &ship);
void ResetShipAnimation(SpaceShip &ship);

void DrawShieldTablet();
void InvisibleShield(SpaceShip &ship);
void InitBullet(Bullet bullet[], int size, ALLEGRO_BITMAP *image);
void DrawBullet(Bullet bullet[], int size, SpaceShip &ship);
void FireBullet(Bullet bullet[], int size, SpaceShip &ship);
void UpdateBullet(Bullet bullet[], int size, SpaceShip &ship);
void CollideBullet(Bullet bullet[], int bSize, Comet comets[], int cSize, SpaceShip &ship, Explosion explosions[], int eSize);
void CollideBullet_E(Bullet bullet[],Bullet bulletE[],int bSize, Enemy enemy[], int eSize, SpaceShip &ship);

//Enemy
void InitEnemy(Enemy enemy[], int size, ALLEGRO_BITMAP *image);
void DrawEnemy(Enemy enemy[], int size);
void DrawEnemyBullet(Bullet bullet[]);
void StartEnemy(Enemy enemy[], int size);
void UpdateEnemy(Enemy enemy[], int size,SpaceShip ship);
void InitEnemyBullet(Bullet bullet[]);
void FireEnemyBullet(Bullet bullet[],Enemy enemy[], SpaceShip &ship);
void UpdateEnemyBullet(Bullet bullet[], Enemy enemy[], SpaceShip &ship);
void CollideEnemy(Enemy enemy[], Bullet bulletE[], int cSize, SpaceShip &ship);
void EnemyCollideBullet(Bullet bullet[], int bSize, SpaceShip &ship);

//Comets
void InitComet(Comet comets[], int size, ALLEGRO_BITMAP *image);
void DrawComet(Comet comets[], int size);
void StartComet(Comet comets[], int size);
void UpdateComet(Comet comets[], int size);
bool isReg(Comet comets[], int size);
void CollideComet(Comet comets[], int cSize, SpaceShip &ship, Explosion explosions[], int eSize);

void initExplosions(Explosion explosions[], int size, ALLEGRO_BITMAP *images);
void DrawExplosions(Explosion explosions[], int size);
void StartExplosions(Explosion explosions[], int size, int x, int y);
void UpdateExplosions(Explosion explosions[], int size);



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
	Bullet bullets[NUM_BULLETS];
	Bullet bullets_E[1];
	Enemy enemy[NUM_ENEMY];
	Comet comets[NUM_COMETS];
	Explosion explosions[NUM_EXPLOSIONS];

	//ALLEGRO OBJECTS
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_BITMAP *image = NULL;
	ALLEGRO_BITMAP *image_B = NULL;
	ALLEGRO_BITMAP *image_R = NULL;
	ALLEGRO_BITMAP  *enemyImage = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *cometImage;
	ALLEGRO_BITMAP *expImage;




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
	image_B = al_load_bitmap("Bullet.png");
	al_convert_mask_to_alpha(image, al_map_rgb(255, 0, 255)); //clear background 
	image_R = al_load_bitmap("rocky.png");
	enemyImage = al_load_bitmap("sprite_sheet_demo.png");
	al_convert_mask_to_alpha(enemyImage, al_map_rgb(106, 76, 48)); //clear background 
	cometImage = al_load_bitmap("asteroid-1-96.png");
	expImage = al_load_bitmap("explosion.png");

	//Random
	srand(time(NULL));

	//CREATE TIMER 
	timer = al_create_timer(1.0 / fps);
	//INITIALIZE OBJECTS 
	InitShip(ship, image);
	InitBullet(bullets, NUM_BULLETS,image_B);
	InitEnemyBullet(bullets_E);
	InitEnemy(enemy, NUM_ENEMY, enemyImage);
	InitComet(comets, NUM_COMETS, cometImage);
	initExplosions(explosions, NUM_EXPLOSIONS, expImage);

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

			if (keys[LEFT])
			{
				RotateLeft(ship);
			}

			else if (keys[RIGHT])
			{
				RotateRight(ship);
			}
			else
			{
				ResetShipAnimation(ship);
			}
			if (keys[A])
			{
				Accelerate(ship);
			}

			if (!isGameOver)
			{	
				CollideEnemy(enemy,bullets_E, NUM_ENEMY, ship);
				CollideBullet_E(bullets,bullets_E, NUM_BULLETS, enemy, NUM_ENEMY, ship);
				CollideComet(comets, NUM_COMETS, ship,explosions,NUM_EXPLOSIONS);
				CollideBullet(bullets, NUM_BULLETS, comets, NUM_COMETS,ship,explosions,NUM_EXPLOSIONS);
				EnemyCollideBullet(bullets_E,1, ship);
				FireEnemyBullet(bullets_E, enemy, ship);
				UpdateBullet(bullets, NUM_BULLETS,ship);
				StartEnemy(enemy, NUM_ENEMY);
				UpdateEnemy(enemy, NUM_ENEMY,ship);
				StartComet(comets, NUM_COMETS);
				UpdateComet(comets, NUM_COMETS);
				UpdateExplosions(explosions, NUM_EXPLOSIONS);
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
				cout << ship.degree << endl;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				cout << ship.degree << endl;
				cout << ship.frameWidth << "           " << ship.x << endl;
				break;
			case ALLEGRO_KEY_S:
				FireBullet(bullets, NUM_BULLETS, ship);
				keys[S] = true;
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
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_S:
				keys[S] = false;
				break;
			}

		}
		//DISPAYING THE PRODUCT
		if (redraw && al_is_event_queue_empty(event_queue))
		{

			redraw = false;
			if (!isGameOver)
			{	
				DrawComet(comets, NUM_COMETS);
				DrawExplosions(explosions, NUM_EXPLOSIONS);
				DrawEnemy(enemy, NUM_ENEMY);
				DrawEnemyBullet(bullets_E);
				DrawBullet(bullets, NUM_BULLETS, ship);
				DrawShip(ship);
				
				InvisibleShield(ship);
				//al_draw_line(WIDTH / 2, 0, WIDTH / 2, HEIGHT, al_map_rgb(199, 4, 30), 5);
				al_draw_bitmap(image_R, (xx - WIDTH / 2) + 30, yy, 0);
				
				//al_draw_rotated_bitmap(image_B, ship.frameWidth / 2, ship.frameHeight / 2,ship.x/2 ,ship.y/2,(ship.degree)*3.14259 / 180, 0);
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
	ship.degree = 0;     //of each block
	ship.boundx = 10;
	ship.boundy = 12;
	ship.score = 0;

	ship.maxFrame = 1;
	ship.curFrame = 0;
	ship.frameCount = 0;
	ship.frameDelay = 50;
	ship.frameWidth = 48;  //of each block
	ship.frameHeight = 43; //of each block

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
void InvisibleShield(SpaceShip &ship)
{
	al_draw_circle(ship.x / 2, ship.y / 2, 75, al_map_rgb(255, 255, 0), 3);

}
void DrawTablet()
{
	// draw a triangle block randomly anywhere...
}

void InitBullet(Bullet bullet[], int size,ALLEGRO_BITMAP *image)
{
	for (int i = 0; i < size; i++)
	{
		bullet[i].ID = BULLET;
		bullet[i].speed = 2;
		bullet[i].live = false;
		bullet[i].image = image;

	}
}
void DrawBullet(Bullet bullet[], int size,SpaceShip &ship )
{
	
	for (int i = 0; i < size; ++i)
	{
		al_draw_filled_circle(bullet[i].x, bullet[i].y, 2, al_map_rgb(255, 255, 255));
		//al_draw_filled_circle(bullet[i].xx, bullet[i].yy, 2, al_map_rgb(255, 255, 255));
		//al_draw_rotated_bitmap(bullet[i].image, ship.frameWidth / 2, ship.frameHeight / 2, bullet[i].x/ 2, bullet[i].y/ 2, (ship.degree + 50)*3.14259 / 180, 0);
	}
}
void FireBullet(Bullet bullet[], int size, SpaceShip &ship)
{
	int R =15;

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
void UpdateBullet(Bullet bullet[], int size,SpaceShip &ship)
{
	int R = 15;
	

		for (int i = 0; i < size; ++i)
	{
		

		if (bullet[i].live)
		{
			bullet[i].x += R*(sin(ship.degree*3.14259/180));    //one bullet
			bullet[i].y -= R*(cos(ship.degree*3.14259 / 180));

			//bullet[i].x += R*(sin((ship.degree+15)*3.14259 / 180)); // two bullets
			//bullet[i].y -= R*(cos((ship.degree + 15)*3.14259 / 180));
			//bullet[i].xx += R*(sin((ship.degree - 15)*3.14259 / 180));
			//bullet[i].yy -= R*(cos((ship.degree - 15)*3.14259 / 180));


			if (bullet[i].x >WIDTH||bullet[i].x<0||bullet[i].y>HEIGHT||bullet[i].y<0)
			{
				bullet[i].live = false;
			}
		}
	}

}
void CollideBullet(Bullet bullet[], int bSize, Comet comets[], int cSize, SpaceShip &ship, Explosion explosions[], int eSize)
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
						ship.score+=2;
					}
				}
			}
		}
	}
}
void CollideBullet_E(Bullet bullet[],Bullet bulletE[],int bSize, Enemy enemy[], int eSize, SpaceShip &ship)
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
						bullet[i].live = false;
						enemy[j].live = false;
						for (int b = 0; b < 2; b++)
						{
							bulletE[b].live = false;
						}
						ship.score+=5;
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
void UpdateEnemy(Enemy enemy[], int size,SpaceShip ship)
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
void InitEnemyBullet(Bullet bullet[])
{	
	for (int i = 0; i < 1; ++i)
	{
		bullet[i].ID = BULLET;
		bullet[i].speed = 5;
		bullet[i].live = false;
	}
}
void FireEnemyBullet(Bullet bullet[], Enemy enemy[], SpaceShip &ship)
{
	

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 1; ++j)
		{
			if (!bullet[j].live && enemy[i].live)
			{
				bullet[j].x = enemy[i].x;
				bullet[j].y = enemy[i].y + 17 ;
				bullet[j].live = true;
				break;
			}
		}

		

		UpdateEnemyBullet(bullet, enemy, ship);
	}
}
void UpdateEnemyBullet(Bullet bullet[], Enemy enemy[], SpaceShip &ship)
{
	
		for (int j = 0; j < 1; ++j)
		{
			if (bullet[j].live)
			{
				
				bullet[j].y += bullet[j].speed;    //one bullet


				//bullet[i].x += R*(sin((ship.degree+15)*3.14259 / 180)); // two bullets
				//bullet[i].y -= R*(cos((ship.degree + 15)*3.14259 / 180));
				//bullet[i].xx += R*(sin((ship.degree - 15)*3.14259 / 180));
				//bullet[i].yy -= R*(cos((ship.degree - 15)*3.14259 / 180));


				if (bullet[j].x > WIDTH || bullet[j].x<0 || bullet[j].y>HEIGHT || bullet[j].y < 0)
				{
					bullet[j].live = false;
				}
			}
		}
}
void DrawEnemyBullet(Bullet bullet[])
{
	for (int i = 0; i < 1; ++i)
	{
		al_draw_filled_circle(bullet[i].x, bullet[i].y, 2, al_map_rgb(255, 255, 255));
		//al_draw_filled_circle(bullet[i].xx, bullet[i].yy, 2, al_map_rgb(255, 255, 255));
		//al_draw_rotated_bitmap(bullet[i].image, ship.frameWidth / 2, ship.frameHeight / 2, bullet[i].x/ 2, bullet[i].y/ 2, (ship.degree + 50)*3.14259 / 180, 0);
	}

}
void CollideEnemy(Enemy enemy[], Bullet bulletE[],int eSize, SpaceShip &ship)
{
	for (int i = 0; i < eSize; i++)
	{
		if (enemy[i].live)
		{
			if (enemy[i].x - enemy[i].boundx < ship.x/2 + ship.boundx &&
				enemy[i].x + enemy[i].boundx > ship.x/2 - ship.boundx &&
				enemy[i].y - enemy[i].boundy < ship.y/2 + ship.boundy &&
				enemy[i].y + enemy[i].boundy > ship.y/2 - ship.boundy)
			{
				ship.x = WIDTH;
				ship.y = HEIGHT;
				ship.degree = 0;
				ship.lives--;
				enemy[i].live = false;
				for (int b = 0; b < 2; b++)
				{
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

void EnemyCollideBullet(Bullet bullet[], int bSize, SpaceShip &ship)
{
	for (int i = 0; i < bSize; i++)
	{
		if (bullet[i].live)
		{
			//bulet dont have a bounding box
				{	//the bullets x position has to be greater then the comets left side ||| * |
					if (bullet[i].x >(ship.x / 2 - ship.boundx) && bullet[i].y < (ship.y / 2 + ship.boundy) && bullet[i].x <(ship.x/2+ ship.boundx) && bullet[i].y >(ship.y/2 - ship.boundy))
					{
						bullet[i].live = false;
						ship.lives--;

						ship.x = WIDTH;
						ship.y = HEIGHT;
						ship.degree = 0;
					}
				}
		}
	}
}

bool isReg(Comet comets[], int size)
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
				if (comets[i].x == WIDTH)
				{
					comets[i].live = false;
				}
			}
			if (comets[i].direction == 5)
			{
				comets[i].x -= comets[i].speed;
				if (comets[i].x == 0)
				{
					comets[i].live = false;
				}
			}
			if (comets[i].direction == 7)
			{
				comets[i].y -= comets[i].speed;
				if (comets[i].y == 0)
				{
					comets[i].live = false;
				}
			}
			if (comets[i].direction == 9)
			{
				comets[i].y += comets[i].speed;
				if (comets[i].y == HEIGHT)
				{
					comets[i].live = false;
				}
			}
		}
	}
}
void CollideComet(Comet comets[], int cSize, SpaceShip &ship, Explosion explosions[], int eSize)
{
	for (int i = 0; i < cSize; i++)
	{
		if (comets[i].live)
		{
			if (comets[i].x - comets[i].boundx < ship.x/2 + ship.boundx &&
				comets[i].x + comets[i].boundx > ship.x/2 - ship.boundx &&
				comets[i].y - comets[i].boundy < ship.y/2 + ship.boundy &&
				comets[i].y + comets[i].boundy > ship.y/2 - ship.boundy)
			{
				ship.lives--;
				comets[i].live = false;
				//StartExplosions(explosions, eSize, ship.x, ship.y);
			}
			else if (comets[i].x < 0)
			{
				comets[i].live = false;
				ship.lives--;
			}
		}
	}
}
void initExplosions(Explosion explosions[], int size, ALLEGRO_BITMAP *image)
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

		explosions[i].images = image;
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
void DrawExplosions(Explosion explosions[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (explosions[i].live)
		{
			int fx = (explosions[i].curFrame % explosions[i].animationColumns) * explosions[i].frameWidth;
			int fy = (explosions[i].curFrame / explosions[i].animationColumns) * explosions[i].frameHeight;

			al_draw_bitmap_region(explosions[i].images, fx, fy, explosions[i].frameWidth,
				explosions[i].frameHeight, explosions[i].x - explosions[i].frameWidth / 2, explosions[i].y - explosions[i].frameHeight / 2, 0);
		}
	}
}
void StartExplosions(Explosion explosions[], int size, int x, int y)
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
void UpdateExplosions(Explosion explosions[], int size)
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