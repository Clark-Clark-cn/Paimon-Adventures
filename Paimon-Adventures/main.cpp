#include <graphics.h>
#include <iostream>
#include <windows.h>
#include "config.h"
Config config;
#include "player.h"
#include "paimon.h"
#include "timer.h"
#include "enemy.h"
#include "button.h"
#include "statusBar.h"
#include "warrior.h"

#pragma comment(lib, "winmm.lib")


bool debug = false;
IMAGE img_shadow_player;
IMAGE img_shadow_enemy;
Atlas atlas_boar_left;
Atlas atlas_boar_right;
Atlas atlas_snail_left;
Atlas atlas_snail_right;
Atlas atlas_bee_left;
Atlas atlas_bee_right;
Player* player;
std::vector<std::shared_ptr<Enemy>> enemies;
std::vector<std::shared_ptr<Bullet>> bullets_around(3);
std::vector<std::shared_ptr<Bullet>> bullets_custom;
int score=0;

void main()
{
	initgraph(1280,720);

	bool running = true;
	bool start = false;
	srand(time(nullptr));
	ExMessage msg;
	IMAGE bg;
	loadimage(&bg, L"img\\background.png");
	loadimage(&img_shadow_player, L"img\\shadow_player.png");
	loadimage(&img_shadow_enemy, L"img\\shadow_enemy.png");
	atlas_boar_left.load(L"img\\boar_left_%d.png", 6);
	atlas_boar_right.load(L"img\\boar_right_%d.png", 6);
	atlas_snail_left.load(L"img\\snail_left_%d.png", 8);
	atlas_snail_right.load(L"img\\snail_right_%d.png", 8);
	atlas_bee_left.load(L"img\\bee_left_%d.png", 4);
	atlas_bee_right.load(L"img\\bee_right_%d.png", 4);

	mciSendString(L"open mus/bgm.mp3 alias bgm", NULL, 0, NULL);
	mciSendString(L"open mus/hit.wav alias hit", NULL, 0, NULL);
	mciSendString(L"open mus/hurt.wav alias hurt", NULL, 0, NULL);
	unsigned int start_time = 0;
	unsigned int delta_time = 0;
	unsigned int end_time = 0;
	unsigned int last_update = GetTickCount();
	Timer timer;
	timer.setInterval(1000);
	timer.setLoop(true);
	int initial_interval=config.get("enemy.initial_summon_interval").asInt();
	int decrease_interval=config.get("enemy.summon_interval_decrease").asInt();
	int least_interval=config.get("enemy.least_summon_interval").asInt();
	timer.setCallback([&](){
		std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>();
		enemies.push_back(enemy);
		timer.setInterval(max(initial_interval - score*decrease_interval, least_interval));
	});
	IMAGE img_avatar_paimon, img_avatar_warrior;
	loadimage(&img_avatar_paimon, L"img\\avatar_paimon.png");
	loadimage(&img_avatar_warrior, L"img\\avatar_warrior.png");
	IMAGE img_button_idle, img_button_hover, img_button_pressed;
	loadimage(&img_button_idle, L"img\\ui_start_idle.png");
	loadimage(&img_button_hover, L"img\\ui_start_hovered.png");
	loadimage(&img_button_pressed, L"img\\ui_start_pressed.png");
	RECT rect;
	rect.left = (1280 - 192) / 2;
	rect.right = rect.left + 192;
	rect.top = 430;
	rect.bottom = rect.top + 96;
	Button button(rect, &img_button_idle, &img_button_hover, &img_button_pressed);
	button.setOnClick([&start](){
		start = true;
	});
	StatusBar* statusBar = nullptr;
	float statusBarX = config.get("pos.status_bar.x").asFloat();
	float statusBarY = config.get("pos.status_bar.y").asFloat();
	rect.left = 1280/2+200;
	rect.right = rect.left + 84;
	rect.top = 530;
	rect.bottom = rect.top + 84;
	Button button2(rect, &img_avatar_warrior);
	button2.setOnClick([&](){
		player = new Warrior();
		mciSendString(L"play bgm repeat from 0", NULL, 0, NULL);
		start = true;
		statusBar = new StatusBar({statusBarX,statusBarY}, &img_avatar_warrior);
		timer.reset();
	});
	rect.left = 1280/2-284;
	rect.right = rect.left + 84;
	Button button3(rect, &img_avatar_paimon);
	button3.setOnClick([&](){
		player = new Paimon();
		mciSendString(L"play bgm repeat from 0", NULL, 0, NULL);
		start = true;
		statusBar = new StatusBar({statusBarX,statusBarY}, &img_avatar_paimon);
		timer.reset();
	});
	IMAGE img_start_bg;
	loadimage(&img_start_bg, L"img\\menu.png");

	while (running)
	{
		start_time = GetTickCount();
		if(!start){
			while(peekmessage(&msg))
			{
				button.update(msg);
			}
			BeginBatchDraw();

			putimage(0, 0, &img_start_bg);
			button.draw();
			FlushBatchDraw();
		}
		else if(player==nullptr){
			while(peekmessage(&msg))
			{
				button2.update(msg);
				button3.update(msg);
			}
			BeginBatchDraw();
			putimage(0, 0, &bg);
			button2.draw();
			button3.draw();

			FlushBatchDraw();
		}
		else{
			while (peekmessage(&msg))
			{
				player->input(msg);	
			}
			unsigned int update_delta = GetTickCount() - last_update;
			player->Update(update_delta);
			for (auto& enemy : enemies)
			{
				enemy->Update(update_delta);
			}
			statusBar->update(update_delta);
			enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const std::shared_ptr<Enemy>& e) { return !e->alive; }), enemies.end());
			Bullet::UpdateBulletsAround(update_delta);
			for(auto& bullet:bullets_custom) bullet->update(update_delta);
			timer.update(update_delta);
			last_update = GetTickCount();
			BeginBatchDraw();

			putimage(0, 0, &bg);
			player->Draw();
			for (auto& enemy : enemies)enemy->Draw();
			for (auto& bullet : bullets_around)bullet->Draw();
			for (auto& bullet : bullets_custom)bullet->Draw();

			statusBar->Draw();

			FlushBatchDraw();
		}

		end_time = GetTickCount();
		delta_time = end_time - start_time;
		if (delta_time < 1000/240)
		{
			Sleep(1000/240 - delta_time);
		}
	}
	EndBatchDraw();
}