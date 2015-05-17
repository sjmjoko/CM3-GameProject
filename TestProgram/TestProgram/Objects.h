//Objrct IDS
enum IDS{ PLAYER, BULLET, ENEMY }; // purpose is that every object in the game should be in this list , so it has a unique ID

//Our Player
struct SpaceShip  // this can be written as a class
{
	int ID;
	int x;
	int y;
	int degree;
	int lives;
	int speed;
	int boundx; // this is the box that detects bounds
	int boundy;
	int score;

	////For your sprites.
	int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int animationColumns;
	int animationDirection;

	int animationRow;
	ALLEGRO_BITMAP *image;
};

struct Bullet
{
	int ID;
	int x;
	int y;
	int xx;
	int yy;
	int degree;
	bool live;
	int speed;

	ALLEGRO_BITMAP *image;

};

struct Enemy
{
	int ID;
	int x;
	int y;
	bool live;
	int speed;
	int boundx;
	int boundy;

	int maxframe;
	int curframe;
	int frameCount;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int animationColumns;
	int animationDirection;

	int animationRow;

	ALLEGRO_BITMAP *image;
};

struct Comet {
	int ID;
	int x;
	int y;
	bool live;
	int speed;
	int boundx;
	int boundy;

	int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int animationColumns;
	int animationDirection;
	int direction;
	ALLEGRO_BITMAP *image;
};

struct Explosion{
	int x;
	int y;
	bool live;

	int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int animationColumns;
	int animationDirection;

	ALLEGRO_BITMAP *images;
};