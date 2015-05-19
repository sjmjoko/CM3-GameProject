#include "Asteroid.h"
#include "GameStates.h"

GameStates::GameStates()
{
	// DO NOTHING
}

void GameStates::initBackground(Background& back, float x, float y, float velX, float velY, int width, int height, int dirX, int dirY, ALLEGRO_BITMAP* image)
{
	back.x = x;
	back.y = y;
	back.velX = velX;
	back.velY = velY;
	back.width = width;
	back.height = height;
	back.dirX = dirX;
	back.dirY = dirY;
	back.image = image;
}

void GameStates::updateBackground(Background& back)
{
	back.x += back.velX*back.dirX;
	if ((back.x + back.width) <= 0)
		back.x = 0;
}

void GameStates::drawBackground(Background& back)
{
	al_draw_bitmap(back.image, back.x, back.y, 0);

	if (back.x + back.width < WIDTH)
		al_draw_bitmap(back.image, back.x + back.width, back.y, 0);
}

void GameStates::drawPlayingBackground(Background& back)
{
	al_draw_bitmap(back.image, 0, 0, 0);
}

int GameStates::gameInit()
{
	if (!al_init())										//initialize Allegro
		return -1;

	// al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	display = al_create_display(WIDTH, HEIGHT);			//create our display object

	if (!display)										//test display object
		return -1;

	al_install_keyboard();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_install_audio();
	al_init_acodec_addon();

	bgImage = al_load_bitmap("starBG.png");
	mgImage = al_load_bitmap("starMG.png");
	fgImage = al_load_bitmap("starFG.png");
	playImage = al_load_bitmap("galaxy-wallpapers-hd-wallpaper-space-photo-galaxy-wallpaper.jpg");
	shipImage = al_load_bitmap("spacecraft_2_T.png");

	initBackground(BG, 0, 0, 1, 0, WIDTH, HEIGHT, -1, 1, bgImage);
	initBackground(MG, 0, 0, 3, 0, WIDTH * 2, HEIGHT, -1, 1, mgImage);
	initBackground(FG, 0, 0, 5, 0, WIDTH, HEIGHT, -1, 1, fgImage);
	initBackground(PBG, 0, 0, 10, 0, WIDTH * 2, HEIGHT * 2, -1, 1, playImage);
	initBackground(SSG, 0, getHEIGHT() / 8, 10, 0, WIDTH, HEIGHT * 2, -1, 1, shipImage);

	al_reserve_samples(10);

	sample3 = al_load_sample("15-talib_kweli-the_perfect_beat_feat.ogg");
	sampleEnd = al_load_sample("14-talib_kweli-space_fruit_interlude_preformed_by_.ogg");

	instance3 = al_create_sample_instance(sample3);
	instanceEnd = al_create_sample_instance(sampleEnd);

	al_set_sample_instance_playmode(instance3, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_playmode(instanceEnd, ALLEGRO_PLAYMODE_LOOP);

	al_attach_sample_instance_to_mixer(instance3, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(instanceEnd, al_get_default_mixer());

	//==============================================
	//PROJECT INIT
	//==============================================
	font18 = al_load_font("orbitron-black.ttf", 18, 0);
	lives = al_load_font("arial.ttf", 18, 0);
	menuFont = al_load_font("Navia.ttf", 150, 0);
	thanks = al_load_font("arial.ttf", 10, 0);

	//==============================================
	//TIMER INIT AND STARTUP
	//==============================================
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_start_timer(timer);
	gameTime = al_current_time();

	return 1;
}

int GameStates::getHEIGHT()
{
	return HEIGHT;
}

int GameStates::getWIDTH()
{
	return WIDTH;
}

void GameStates::destroy()
{
	Asteroid destroyAst;
	//==============================================
	//DESTROY PROJECT OBJECTS
	//==============================================

	//SHELL OBJECTS=================================
	al_destroy_display(display);
	al_destroy_sample_instance(instance3);
	al_destroy_sample_instance(instanceEnd);
	al_destroy_sample(sample3);
	al_destroy_sample(sampleEnd);
	al_destroy_bitmap(bgImage);
	al_destroy_bitmap(mgImage);
	al_destroy_bitmap(fgImage);
	al_destroy_bitmap(shipImage);
	al_destroy_font(font18);
	al_destroy_font(thanks);
	al_destroy_font(lives);
	al_destroy_font(menuFont);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	destroyAst.destroy();
}