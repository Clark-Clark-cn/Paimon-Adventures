#include <graphics.h>
#include <iostream>
#include <windows.h>
#include "player.h"
#include "paimon.h"
#include "timer.h"
#include "enemy.h"
#include "button.h"

#pragma comment(lib, "winmm.lib")


bool debug = false;
IMAGE img_shadow_player;
IMAGE img_shadow_enemy;
Atlas atlas_enemy_left;
Atlas atlas_enemy_right;
Player* player;
std::vector<std::shared_ptr<Enemy>> enemies;
std::vector<std::shared_ptr<Bullet>> bullets(3);
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
	atlas_enemy_left.load(L"img\\boar_left_%d.png", 6);
	atlas_enemy_right.load(L"img\\boar_right_%d.png", 6);
	Atlas leftAtlas;
	leftAtlas.load(L"img\\paimon_left_%d.png", 6);
	Atlas rightAtlas;
	rightAtlas.load(L"img\\paimon_right_%d.png", 6);

	mciSendString(L"open mus/bgm.mp3 alias bgm", NULL, 0, NULL);
	mciSendString(L"open mus/hit.wav alias hit", NULL, 0, NULL);
	mciSendString(L"open mus/hurt.wav alias hurt", NULL, 0, NULL);
	player = new Paimon(&leftAtlas, &rightAtlas);
	unsigned int start_time = 0;
	unsigned int delta_time = 0;
	unsigned int end_time = 0;
	unsigned int last_update = GetTickCount();
	Timer timer;
	timer.setInterval(1000);
	timer.setLoop(true);
	timer.setCallback([](){
		std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>();
		enemies.push_back(enemy);
	});
	for(auto& b : bullets) {
		b = std::make_shared<Bullet>();
	}

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
		mciSendString(L"play bgm repeat from 0", NULL, 0, NULL);
		start = true;
	});
	IMAGE img_start_bg;
	loadimage(&img_start_bg, L"img\\menu.png");

	while (running)
	{
		start_time = GetTickCount();
		if(!start){
			if(peekmessage(&msg))
			{
				button.update(msg);
			}
			BeginBatchDraw();

			putimage(0, 0, &img_start_bg);
			button.draw();
			FlushBatchDraw();
		}
		else{
			if (peekmessage(&msg))
			{
				player->input(msg);	
			}
			unsigned int update_delta = GetTickCount() - last_update;
			player->Update(update_delta);
			for (auto& enemy : enemies)
			{
				enemy->Update(update_delta);
			}
			enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const std::shared_ptr<Enemy>& e) { return !e->alive; }), enemies.end());
			Bullet::UpdateBullets(update_delta);
			timer.update(update_delta);
			last_update = GetTickCount();
			BeginBatchDraw();

			putimage(0, 0, &bg);
			player->Draw();
			for (auto& enemy : enemies)enemy->Draw();
			for (auto& bullet : bullets)bullet->Draw();

			{
				TCHAR buf[50];
				swprintf(buf, 50, L"Score: %d", score);
				settextcolor(RGB(255,85,185));
				setbkmode(TRANSPARENT);
				outtextxy(10, 10, buf);
			}

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