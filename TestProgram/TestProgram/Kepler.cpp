#include "GameStates.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

bool keys[7] = { false, false, false, false, false, false, false };

int main()
{
	GameStates Kepler;
	Kepler.gameInit();

	while (!Kepler.done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(Kepler.event_queue, &ev);

		//==============================================
		//INPUT
		//==============================================
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				keys[Kepler.ESCAPE] = true;
				break;
			case ALLEGRO_KEY_ENTER:
				keys[Kepler.ENTER] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[Kepler.LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[Kepler.RIGHT] = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[Kepler.UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[Kepler.DOWN] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[Kepler.SPACE] = true;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				keys[Kepler.ESCAPE] = false;
				break;
			case ALLEGRO_KEY_ENTER:
				keys[Kepler.ENTER] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[Kepler.LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[Kepler.RIGHT] = false;
				break;
			case ALLEGRO_KEY_UP:
				keys[Kepler.UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[Kepler.DOWN] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[Kepler.SPACE] = false;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			Kepler.done = true;
		}
		//==============================================
		//GAME UPDATE
		//==============================================
		else if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			Kepler.updateBackground(Kepler.BG);
			Kepler.updateBackground(Kepler.MG);
			Kepler.updateBackground(Kepler.FG);
			Kepler.render = true;

			//UPDATE FPS===========
			Kepler.frames++;
			if (al_current_time() - Kepler.gameTime >= 1)
			{
				Kepler.gameTime = al_current_time();
				Kepler.gameFPS = Kepler.frames;
				Kepler.frames = 0;
			}
			//=====================

			if (Kepler.state == Kepler.MENU)
			{
				if (keys[Kepler.SPACE])
					Kepler.state = Kepler.PLAYING;
				if (keys[Kepler.ESCAPE])
					Kepler.state = Kepler.GAMEOVER;
			}
			else if (Kepler.state == Kepler.PLAYING)
			{
				if (keys[Kepler.ESCAPE])
					Kepler.state = Kepler.GAMEOVER;
			}
			else if (Kepler.state == Kepler.GAMEOVER)
			{
				if (keys[Kepler.SPACE])
					Kepler.done = true;
				if (keys[Kepler.ENTER])
					Kepler.state = Kepler.MENU;
			}
		}
		//==============================================
		//RENDER
		//==============================================
		if (Kepler.render && al_is_event_queue_empty(Kepler.event_queue))
		{
			Kepler.render = false;
			// al_draw_textf(font18, al_map_rgb(255, 0, 255), 5, 5, 0, "FPS: %i", gameFPS);	//display FPS on screen

			//BEGIN PROJECT RENDER================
			if (Kepler.state == Kepler.MENU)
			{
				Kepler.drawBackground(Kepler.BG);
				Kepler.drawBackground(Kepler.MG);
				Kepler.drawBackground(Kepler.FG);

				al_stop_sample_instance(Kepler.instanceEnd);
				al_draw_text(Kepler.font18, al_map_rgb(255, 255, 255), 0, Kepler.getHEIGHT() - 100, ALLEGRO_ALIGN_LEFT, "CONTROLS");
				al_draw_text(Kepler.font18, al_map_rgb(255, 255, 150), 0, Kepler.getHEIGHT() - 70, ALLEGRO_ALIGN_LEFT, "Arrow Keys : Steer/Thrust");
				al_draw_text(Kepler.font18, al_map_rgb(255, 255, 150), 0, Kepler.getHEIGHT() - 40, ALLEGRO_ALIGN_LEFT, "Space           :  Shoot");
				al_draw_text(Kepler.menuFont, al_map_rgb(100, 255, 150), Kepler.getWIDTH() / 2, Kepler.getHEIGHT() / 3, ALLEGRO_ALIGN_CENTRE, "Kepler");
				al_draw_text(Kepler.font18, al_map_rgb(255, 255, 255), Kepler.getWIDTH() / 2, (Kepler.getHEIGHT() / 2) + 40, ALLEGRO_ALIGN_CENTRE, "Press Space to Play");
				al_draw_text(Kepler.font18, al_map_rgb(255, 255, 255), Kepler.getWIDTH() / 2, (Kepler.getHEIGHT() / 2) + 70, ALLEGRO_ALIGN_CENTRE, "Press Escape to End");

				al_play_sample_instance(Kepler.instance3);
			}
			else if (Kepler.state == Kepler.PLAYING)
			{
				al_stop_sample_instance(Kepler.instance3);

				Kepler.drawPlayingBackground(Kepler.PBG);
				al_draw_text(Kepler.lives, al_map_rgb(255, 255, 255), 0, 10, ALLEGRO_ALIGN_LEFT, "A A A");
				al_draw_text(Kepler.font18, al_map_rgb(255, 255, 255), 0, 40, ALLEGRO_ALIGN_LEFT, "000");
				al_draw_text(Kepler.font18, al_map_rgb(255, 255, 255), Kepler.getWIDTH() - 60, Kepler.getHEIGHT() - 30, ALLEGRO_ALIGN_CENTRE, "Quit = ESC");
				//al_draw_text(font18, al_map_rgb(255, 255, 255), WIDTH / 2, (HEIGHT / 2) + 30, ALLEGRO_ALIGN_CENTRE, "Press ENTER to go to MENU");
			}
			else if (Kepler.state == Kepler.GAMEOVER)
			{
				al_stop_sample_instance(Kepler.instance3);
				al_play_sample_instance(Kepler.instanceEnd);

				Kepler.drawBackground(Kepler.BG);
				al_draw_text(Kepler.font18, al_map_rgb(255, 0, 0), 0, 30, ALLEGRO_ALIGN_LEFT, "Are you sure you want to quit?");
				al_draw_text(Kepler.font18, al_map_rgb(255, 255, 255), Kepler.getWIDTH() / 2, Kepler.getHEIGHT() / 2, ALLEGRO_ALIGN_CENTRE, "Press Space to Exit the Game");
				al_draw_text(Kepler.font18, al_map_rgb(255, 255, 255), Kepler.getWIDTH() / 2, (Kepler.getHEIGHT() / 2) + 30, ALLEGRO_ALIGN_CENTRE, "Press ENTER to go to MENU");
			}

			//FLIP BUFFERS========================
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
	}
	Kepler.destroy();

	return 0;
}