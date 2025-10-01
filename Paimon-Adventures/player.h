#pragma once

#include <graphics.h>

#include <algorithm>
#include <string>

#include "config.h"
#include "Vector2.h"
#include "timer.h"
#include "base.h"

extern bool debug;
extern IMAGE img_shadow_player;
extern int score;

class StatusBar;

class Player{
    friend class StatusBar;
protected:
    Vector2 position{config.get("player.init.position.x").asInt(), 
        config.get("player.init.position.y").asInt()};
    Vector2 velocity{0,0};
    Vector2 size{20,20};
    bool left_key=false;
    bool right_key=false;
    bool up_key=false;
    bool down_key=false;
    bool skill_key=false;
    bool isFacingRight=false;
    int mp=0;
    int maxMp=config.get("player.maxMp").asInt();
    int hp=100;
    int max_hp=100;
    float speed=0.3f; // pixels per millisecond
    bool canAttack=true;
    Timer attackTimer;
    bool drawIMG=true;
    Timer drawSwitchTimer;
public:
    Player(){
        attackTimer.setInterval(config.get("player.invincibleTime").asInt());
        attackTimer.setLoop(false);
        attackTimer.setCallback([this](){
            canAttack=true;
            drawIMG=true;
            drawSwitchTimer.setEnabled(false);
        });
        drawSwitchTimer.setInterval(50);
        drawSwitchTimer.setLoop(true);
        drawSwitchTimer.setCallback([this](){
            drawIMG=!drawIMG;
        });
        attackTimer.setEnabled(false);
        drawSwitchTimer.setEnabled(false);
    }
    ~Player()=default;
    virtual void Draw(){
        static int shadow_offset_y=config.get("player.shadow_offset_y").asInt();
        Vector2 pos_shadow;
        pos_shadow.x=position.x+size.x/2-img_shadow_player.getwidth()/2;
        pos_shadow.y=position.y+size.y-shadow_offset_y;

        if(drawIMG) putimage(pos_shadow, &img_shadow_player);
        if(debug){
            setlinecolor(RED);
            rectangle(position.x-size.x/2, position.y-size.y/2, position.x+size.x/2, position.y+size.y/2);
        }
    }
    virtual void Update(unsigned int deltaTime){
        attackTimer.update(deltaTime);
        drawSwitchTimer.update(deltaTime);
        if(hp<=0) {
            mciSendString(L"stop bgm", NULL, 0, NULL);
            std::wstring ws = L"Your score: " + std::to_wstring(score);
            MessageBox(NULL, ws.c_str(), L"Game Over!", MB_OK|MB_ICONINFORMATION|MB_SYSTEMMODAL);
            exit(0);
        }
		velocity = { 0,0 };
        int dir_x=right_key - left_key;
        int dir_y=down_key - up_key;
        double length=std::sqrt(dir_x*dir_x+dir_y*dir_y);
        if(length!=0){
            double inv_len=dir_x/length;
            double inv_len_y=dir_y/length;
            velocity.x=inv_len*speed;
            velocity.y=inv_len_y*speed;
        }
        if (dir_x > 0) isFacingRight = true;
        else if (dir_x < 0) isFacingRight = false;
        position+=velocity*deltaTime;
		position.x = std::max<float>(position.x, 0);
		position.x = std::min<float>(position.x, 1280);
		position.y = std::max<float>(position.y, 0);
		position.y = std::min<float>(position.y, 720);
    }
    virtual void input(const ExMessage& msg){
        static int up1=config.get("player.key.up1").asInt();
        static int up2=config.get("player.key.up2").asInt();
        static int down1=config.get("player.key.down1").asInt();
        static int down2=config.get("player.key.down2").asInt();
        static int left1=config.get("player.key.left1").asInt();
        static int left2=config.get("player.key.left2").asInt();
        static int right1=config.get("player.key.right1").asInt();
        static int right2=config.get("player.key.right2").asInt();
        static int skill1=config.get("player.key.skill1").asInt();
        static int skill2=config.get("player.key.skill2").asInt();
        switch (msg.message)
        {
        case WM_KEYDOWN:
            if (msg.vkcode == left1 || msg.vkcode == left2) {
                left_key = true;
            } else if (msg.vkcode == right1 || msg.vkcode == right2) {
                right_key = true;
            } else if (msg.vkcode == up1 || msg.vkcode == up2) {
                up_key = true;
            } else if (msg.vkcode == down1 || msg.vkcode == down2) {
                down_key = true;
            } else if (msg.vkcode == skill1 || msg.vkcode == skill2) {
                skill_key = true;
            }
            break;
        case WM_KEYUP:
            if (msg.vkcode == left1 || msg.vkcode == left2) {
                left_key = false;
            } else if (msg.vkcode == right1 || msg.vkcode == right2) {
                right_key = false;
            } else if (msg.vkcode == up1 || msg.vkcode == up2) {
                up_key = false;
            } else if (msg.vkcode == down1 || msg.vkcode == down2) {
                down_key = false;
            } else if (msg.vkcode == skill1 || msg.vkcode == skill2) {
                skill_key = false;
            }
            break;
        }
    }

    void setPosition(const Vector2& pos){position=pos;}
    Vector2 getPosition() const{return position;}
	Vector2 getSize() const { return size; }
    int getMP() const { return mp; }
    void setMP(int magic) { mp = min(magic, maxMp); }
	void addMP(int delta) { mp = min(mp + delta, maxMp); }
    int getHP() const { return hp; }
    void addHP(int health) { 
        hp = min(hp + health, 100); 
        attackTimer.setEnabled(true);
        canAttack=false;
        drawSwitchTimer.setEnabled(true);
    }
    bool canAttackNow() const { return canAttack; }
};
