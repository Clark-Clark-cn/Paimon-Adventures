#pragma once

#include <string>
#include <graphics.h>

#include "Vector2.h"
#include "base.h"
#include "player.h"

extern Player* player;
extern int score;

class StatusBar{
    Vector2 position;
    Vector2 size{100,10};
    IMAGE* avatar=nullptr;
    Atlas* F_btn=nullptr;
    Animation F_btn_anim;
public:
    StatusBar(Vector2 pos, IMAGE* img) : position(pos), avatar(img) {
        F_btn = new Atlas();
        F_btn->load(L"img\\F_btn_%d.png", 2);
        F_btn_anim.setAtlas(F_btn);
        F_btn_anim.setFrameTime(200);
        F_btn_anim.setLoop(true);
    }

    void Draw() {
        if(avatar) {
            putimage(position, avatar);
        }
        Vector2 posHP = position;
        Vector2 posMp=position;
        posMp.x+=90;
        posHP.x+=90;
        posMp.y=posHP.y+size.y+5;
        setfillcolor(RGB(0, 0, 0));
        fillrectangle(posHP.x, posHP.y, posHP.x + size.x, posHP.y + size.y);
        fillrectangle(posMp.x, posMp.y, posMp.x + size.x, posMp.y + size.y);

        setfillcolor(RGB(255, 0, 0));
        int hpBarWidth = (size.x - 4) * player->hp / player->max_hp; // 4 pixels padding
        fillrectangle(posHP.x + 2, posHP.y + 2, posHP.x + 2 + hpBarWidth, posHP.y + size.y - 2);
        setfillcolor(RGB(0, 0, 255));
        int mpBarWidth = (size.x - 4) * player->mp / player->maxMp; // 4 pixels padding
        fillrectangle(posMp.x + 2, posMp.y + 2, posMp.x + 2 + mpBarWidth, posMp.y + size.y - 2);

        Vector2 posF=posMp;
        posF.x+=size.x+5;
        if(player->mp>=player->maxMp)
            F_btn_anim.draw(posF);
        posMp.y+=size.y+5;
        TCHAR buf[50];
        swprintf(buf, 50, L"Score: %d", score);
        settextcolor(RGB(255,85,185));
        setbkmode(TRANSPARENT);
        outtextxy(posMp.x, posMp.y, buf);
    }
    void update(unsigned int deltaTime){
        F_btn_anim.update(deltaTime);
    }
};
