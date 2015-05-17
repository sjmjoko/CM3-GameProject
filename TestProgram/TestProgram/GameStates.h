#ifndef GAMESTATES_H
#define GAMESTATES_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <array>

class GameStates
{
	const int WIDTH = 800;
	const int HEIGHT = 600;

public:
	GameStates();
	struct Background
	{
		float x;
		float y;
		float velX;
		float velY;
		int dirX;
		int dirY;

		int width;
		int height;

		ALLEGRO_BITMAP* image;
	};

	void initBackground(Background&, float, float, float, float, int, int, int, int, ALLEGRO_BITMAP*);
	void updateBackground(Background&);
	void drawBackground(Background&);
	void drawPlayingBackground(Background&);
	int gameInit();
	int getWIDTH();
	int getHEIGHT();

	void destroy();
	
	enum KEYS{ UP, DOWN, LEFT, RIGHT, SPACE, ESCAPE, ENTER };

	enum STATE{ MENU, PLAYING, GAMEOVER };

	bool done = false;
	bool render = false;

	Background BG;
	Background MG;
	Background FG;
	Background PBG;
	Background SSG;

	float gameTime = 0;
	int frames = 0;
	int gameFPS = 0;

	int state = MENU;

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer;
	ALLEGRO_FONT *font18;
	ALLEGRO_FONT *lives;
	ALLEGRO_FONT *thanks;
	ALLEGRO_FONT *menuFont;
	ALLEGRO_SAMPLE *sample3 = NULL;
	ALLEGRO_SAMPLE *sampleEnd = NULL;
	ALLEGRO_SAMPLE_INSTANCE *instance3 = NULL;
	ALLEGRO_SAMPLE_INSTANCE *instanceEnd = NULL;
	ALLEGRO_BITMAP* bgImage = NULL;
	ALLEGRO_BITMAP* mgImage = NULL;
	ALLEGRO_BITMAP* fgImage = NULL;
	ALLEGRO_BITMAP* playImage = NULL;
	ALLEGRO_BITMAP* shipImage = NULL;


};
#endif