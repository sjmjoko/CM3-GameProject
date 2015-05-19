#include "GameStates.h"
#include "Asteroid.h"
#include "Keplership.h"
#include "Enemyship.h"
#include "Bullet.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <cmath>

bool keys[7] = { false, false, false, false, false, false, false };

int main()
{
	int const NUM_COMETS = 10;
	int const NUM_EXPLOSIONS = 5;
	int score, life;
	bool isGameOver = false;
	bool redraw = false;
	GameStates Kepler;
	Kepler.gameInit();

	//Classes
	Keplership Kepship(Kepler);
	Enemyship Enship(Kepler);
	Asteroid Comet;

	//objects
	objectKPLR obShip;
	objectKPLR obEnemy[2];
	objectKPLR comets[NUM_COMETS];
	objectKPLR explosions[NUM_EXPLOSIONS];

	//Bullet classes
	Bullet bullets[10];
	Bullet bullets_E[5];

	Kepship.shipImage = al_load_bitmap("Kepler-sprite.png");
	al_convert_mask_to_alpha(Kepship.shipImage, al_map_rgb(255, 0, 255)); //clear background
	Enship.enemyImage = al_load_bitmap("sprite_sheet_demo.png");
	al_convert_mask_to_alpha(Enship.enemyImage, al_map_rgb(255, 0, 255)); //fix mafu's sprite

	Comet.cometImage = al_load_bitmap("asteroid-1-96.png");
	Comet.expImage = al_load_bitmap("explosion.png");

	srand(time(NULL));
	
	Kepler.timer = al_create_timer(1.0 / 60);
	//INITIALIZE OBJECTS 
	Kepship.initKeplership(obShip, Kepship.shipImage);//InitShip(ship, image);
	Kepship.initBullets(bullets, Kepship.Bullets);//InitBullet(bullets, NUM_BULLETS,image_B);
	Enship.initship(obEnemy, Enship.ENEMY, Enship.enemyImage);//InitEnemy(enemy, NUM_ENEMY, enemyImage);
	Enship.initBullets(bullets_E, Enship.Bullets);//InitEnemyBullet(bullets_E);
	Comet.InitComet(comets, NUM_COMETS, Comet.cometImage);
	Comet.initExplosions(explosions, NUM_EXPLOSIONS, Comet.expImage);


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
				Kepship.fireBullets(bullets, Kepship.Bullets, obShip);
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

			redraw = true;
			if (!isGameOver)
			{
				Enship.collideEBullet(bullets_E, Enship.Bullets, obShip, explosions, NUM_EXPLOSIONS,Comet);//EnemyCollideBullet(bullets_E,1, ship);
				Enship.collide(obEnemy, bullets_E, Enship.ENEMY, obShip, explosions, NUM_EXPLOSIONS, Comet);//CollideEnemy(enemy,bullets_E, NUM_ENEMY, ship);
				Kepship.collideEBullet(bullets, bullets_E, Kepship.Bullets, obEnemy, Enship.ENEMY, obShip, explosions, NUM_EXPLOSIONS, Comet);//CollideBullet_E(bullets,bullets_E, NUM_BULLETS, enemy, NUM_ENEMY, ship);
				Kepship.collideCBullet(bullets, Kepship.Bullets, comets, NUM_COMETS, obShip, explosions, NUM_EXPLOSIONS, Comet);//CollideComet(comets, NUM_COMETS, ship,explosions,NUM_EXPLOSIONS);
				Comet.CollideComet(comets, NUM_COMETS, obShip, explosions, NUM_EXPLOSIONS, Comet);
				Enship.fireBullets(bullets_E, Enship.Bullets, obEnemy, obShip);//FireEnemyBullet(bullets_E, enemy, ship);
				Kepship.updateBullet(bullets, Kepship.Bullets, obShip);//UpdateBullet(bullets, NUM_BULLETS,ship);
				Enship.startEnemy(obEnemy, Enship.ENEMY);//StartEnemy(enemy, NUM_ENEMY);
				Enship.updateEnemy(obEnemy, Enship.ENEMY, obShip);//UpdateEnemy(enemy, NUM_ENEMY,ship);
				Comet.StartComet(comets, NUM_COMETS);
				Comet.UpdateComet(comets, NUM_COMETS);
				Comet.UpdateExplosions(explosions, NUM_EXPLOSIONS);

				if (obShip.lives <= 0)
				{
					isGameOver = true;
				}
			}

			if (Kepler.state == Kepler.MENU)
			{
				if (keys[Kepler.SPACE])
					Kepler.state = Kepler.PLAYING;
				if (keys[Kepler.ESCAPE])
					Kepler.state = Kepler.GAMEOVER;
			}
			else if (Kepler.state == Kepler.PLAYING)
			{
				if (keys[Kepler.LEFT])
				{
					//RotateLeft(ship);
					Kepship.rotateLeft(obShip);
				}

				else if (keys[Kepler.RIGHT])
				{
					//RotateRight(ship);
					Kepship.rotateRight(obShip);
				}
				else
				{
					//ResetShipAnimation(ship);
					Kepship.resetKeplership(obShip);
				}
				if (keys[Kepler.UP])
				{
					//Accelerate(ship);
					Kepship.accKeplership(obShip);
				}
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

			//BEGIN PROJECT RENDER================
			if (Kepler.state == Kepler.MENU)
			{
				Kepler.drawBackground(Kepler.BG);
				Kepler.drawBackground(Kepler.MG);
				Kepler.drawBackground(Kepler.FG);
				Kepler.drawBackground(Kepler.SSG);

				al_stop_sample_instance(Kepler.instanceEnd);
				al_draw_text(Kepler.font18, al_map_rgb(255, 255, 255), 0, Kepler.getHEIGHT() - 110, ALLEGRO_ALIGN_LEFT, "CONTROLS");
				al_draw_text(Kepler.font18, al_map_rgb(255, 255, 150), 0, Kepler.getHEIGHT() - 80, ALLEGRO_ALIGN_LEFT, "Arrow Keys : Steer");
				al_draw_text(Kepler.font18, al_map_rgb(255, 255, 150), 0, Kepler.getHEIGHT() - 50, ALLEGRO_ALIGN_LEFT, "S           :  Shoot");
				al_draw_text(Kepler.font18, al_map_rgb(255, 255, 150), 0, Kepler.getHEIGHT() - 20, ALLEGRO_ALIGN_LEFT, "A           : Accelerate");
				al_draw_text(Kepler.menuFont, al_map_rgb(100, 255, 150), Kepler.getWIDTH() / 2, Kepler.getHEIGHT() / 3, ALLEGRO_ALIGN_CENTRE, "Kepler");
				al_draw_text(Kepler.font18, al_map_rgb(255, 255, 255), Kepler.getWIDTH() / 2, (Kepler.getHEIGHT() / 2) + 40, ALLEGRO_ALIGN_CENTRE, "Press Space to Play");
				al_draw_text(Kepler.font18, al_map_rgb(255, 255, 255), Kepler.getWIDTH() / 2, (Kepler.getHEIGHT() / 2) + 70, ALLEGRO_ALIGN_CENTRE, "Press Escape to End");
				al_draw_text(Kepler.font18, al_map_rgb(255, 50, 5), Kepler.getWIDTH() - 10, Kepler.getHEIGHT() - 20, ALLEGRO_ALIGN_RIGHT, "TEAM DOPE!");

				al_play_sample_instance(Kepler.instance3);
			}
			else if (Kepler.state == Kepler.PLAYING)
			{
				al_stop_sample_instance(Kepler.instance3);

				Kepler.drawPlayingBackground(Kepler.PBG);

				if (redraw && al_is_event_queue_empty(Kepler.event_queue))
				{
					redraw = false;
					if (!isGameOver)
					{
						
						Comet.DrawComet(comets, NUM_COMETS);

						Enship.drawship(obEnemy, Enship.ENEMY);//DrawEnemy(enemy, NUM_ENEMY);
						Enship.drawBullets(bullets_E);//DrawEnemyBullet(bullets_E);

						Kepship.drawBullets(bullets, Kepship.Bullets, obShip);//DrawBullet(bullets, NUM_BULLETS, ship);
						Kepship.drawKeplership(obShip);	//DrawShip(ship);

						Comet.DrawExplosions(explosions, NUM_EXPLOSIONS);////DrawExplosions(explosions, NUM_EXPLOSIONS);
					}
				}
				switch (obShip.lives)
				{
				case 0:
					isGameOver = true;
					Kepler.state = Kepler.GAMEOVER;
					break;
				case 1:
					al_draw_text(Kepler.lives, al_map_rgb(255, 255, 255), 0, 10, ALLEGRO_ALIGN_LEFT, "A");
					break;
				case 2:
					al_draw_text(Kepler.lives, al_map_rgb(255, 255, 255), 0, 10, ALLEGRO_ALIGN_LEFT, "A A");
					break;
				case 3:
					al_draw_text(Kepler.lives, al_map_rgb(255, 255, 255), 0, 10, ALLEGRO_ALIGN_LEFT, "A A A");
					break;
				case 4:
					al_draw_text(Kepler.lives, al_map_rgb(255, 255, 255), 0, 10, ALLEGRO_ALIGN_LEFT, "A A A A");
					break;
				default:
					break;
				}
				al_draw_textf(Kepler.font18, al_map_rgb(255, 255, 255), 0, 40, ALLEGRO_ALIGN_LEFT, "%i",obShip.score);
				al_draw_text(Kepler.font18, al_map_rgb(255, 255, 255), Kepler.getWIDTH() - 60, Kepler.getHEIGHT() - 30, ALLEGRO_ALIGN_CENTRE, "Quit = ESC");
			}
			else if (Kepler.state == Kepler.GAMEOVER)
			{
				al_stop_sample_instance(Kepler.instance3);
				al_play_sample_instance(Kepler.instanceEnd);

				Kepler.drawBackground(Kepler.BG);
				al_draw_text(Kepler.font18, al_map_rgb(255, 0, 0), 0, 30, ALLEGRO_ALIGN_LEFT, "Are you sure you want to quit?");
				al_draw_text(Kepler.font18, al_map_rgb(255, 255, 255), Kepler.getWIDTH() / 2, Kepler.getHEIGHT() / 2, ALLEGRO_ALIGN_CENTRE, "Press Space to Exit the Game");
				al_draw_text(Kepler.font18, al_map_rgb(255, 255, 255), Kepler.getWIDTH() / 2, (Kepler.getHEIGHT() / 2) + 30, ALLEGRO_ALIGN_CENTRE, "Press ENTER to go to MENU");
				al_draw_text(Kepler.thanks, al_map_rgb(255, 150, 255), 0, Kepler.getHEIGHT() - 100, ALLEGRO_ALIGN_LEFT, "Thank you for playing Kepler");
				al_draw_text(Kepler.thanks, al_map_rgb(255, 150, 150), 0, Kepler.getHEIGHT() - 90, ALLEGRO_ALIGN_LEFT, "Created by:");
				al_draw_text(Kepler.thanks, al_map_rgb(255, 150, 255), 0, Kepler.getHEIGHT() - 80, ALLEGRO_ALIGN_LEFT, "Sesethu Mhlana");
				al_draw_text(Kepler.thanks, al_map_rgb(255, 150, 255), 0, Kepler.getHEIGHT() - 70, ALLEGRO_ALIGN_LEFT, "Matsobane Makhura");
				al_draw_text(Kepler.thanks, al_map_rgb(255, 150, 255), 0, Kepler.getHEIGHT() - 60, ALLEGRO_ALIGN_LEFT, "Sithembile Dubazane");
				al_draw_text(Kepler.thanks, al_map_rgb(255, 150, 255), 0, Kepler.getHEIGHT() - 50, ALLEGRO_ALIGN_LEFT, "Mafu Mxenge");
				al_draw_text(Kepler.thanks, al_map_rgb(255, 150, 255), 0, Kepler.getHEIGHT() - 40, ALLEGRO_ALIGN_LEFT, "Sithembiso Mjoko");

				if (obShip.lives == 0)
				{
					al_draw_text(Kepler.font18, al_map_rgb(255, 0, 0), Kepler.getWIDTH() / 2, (Kepler.getHEIGHT() / 2)-30, ALLEGRO_ALIGN_CENTRE, "GAME OVER!");
				}
			}

			//FLIP BUFFERS========================
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
	}
	Kepler.destroy();

	return 0;
}