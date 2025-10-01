#pragma once

#include "Animation.h"
#include "player.h"
#include "bullet.h"
#include <string>

extern bool debug;
extern IMAGE img_shadow_enemy;
extern Atlas atlas_enemy_left;
extern Atlas atlas_enemy_right;

extern Player* player;
extern int score;

class Enemy{
    Vector2 position;
    Vector2 velocity{0,0};
    Vector2 size;
    bool isFacingRight=false;
    Animation leftAnim;
    Animation rightAnim;
    Animation* currentAnim=&leftAnim;
    enum SpawnSide{
        LEFT,
        RIGHT,
        TOP,
        BOTTOM
	};
	int spawnSide = rand()%4;
public:
    bool alive=true;
    Enemy(){
		Vector2 pos;
        switch (spawnSide)
        {
	        case LEFT:
                pos = { -50.0f, (float)(rand() % 720) };
				break;
            case RIGHT:
                pos = { 1280.0f + 50.0f, (float)(rand() % 720) };
                break;
            case TOP:
                pos = { (float)(rand() % 1280), -50.0f };
                break;
            case BOTTOM:
                pos = { (float)(rand() % 1280), 720.0f + 50.0f };
                break;
        }
        position=pos;
        size={(float)atlas_enemy_left.getImage(0)->getwidth(), (float)atlas_enemy_left.getImage(0)->getheight()};
        leftAnim.setAtlas(&atlas_enemy_left);
        leftAnim.setFrameTime(50);
        leftAnim.setLoop(true);
        rightAnim.setAtlas(&atlas_enemy_right);
        rightAnim.setFrameTime(50);
        rightAnim.setLoop(true);
    }
    ~Enemy()=default;
    void Draw() {
        Vector2 pos_shadow;
        pos_shadow.x=position.x+size.x/2-img_shadow_enemy.getwidth()/2;
        pos_shadow.y=position.y+size.y-35;

        putimage(pos_shadow, &img_shadow_enemy);
        currentAnim->draw(position);
        if(debug){
            setlinecolor(RED);
            rectangle(position.x-size.x/2, position.y-size.y/2, position.x+size.x/2, position.y+size.y/2);
        }
    }
    void Update(unsigned int deltaTime) {
		velocity = { 0,0 };
        Vector2 toPlayer = player->getPosition() - position;
        velocity = toPlayer.normalized() * 0.15f;
        if (toPlayer.x > 0) isFacingRight = true;
        else if (toPlayer.x < 0) isFacingRight = false;
        position+=velocity*deltaTime;
        currentAnim=isFacingRight?&rightAnim:&leftAnim;
        currentAnim->update(deltaTime);
        if(isCollisionPlayer(*player)){
            mciSendString(L"play hurt from 0", NULL, 0, NULL);
            mciSendString(L"stop bgm", NULL, 0, NULL);
            std::wstring ws = L"Your score: " + std::to_wstring(score);
            MessageBox(NULL, ws.c_str(), L"Game Over!", MB_OK|MB_ICONINFORMATION|MB_SYSTEMMODAL);
            exit(0);
        }
        for(auto& b: bullets){
            if(b->isActive() && isCollisionBullet(*b)){
                mciSendString(L"play hit from 0", NULL, 0, NULL);
                alive=false;
                score++;
                b->hit();
            }
        }
    }
    bool isCollisionPlayer(const Player& p) const {
        Vector2 checkPos = position + size / 2;
        bool isCollideX = checkPos.x >= p.getPosition().x && checkPos.x <= p.getPosition().x + p.getSize().x;
        bool isCollideY = checkPos.y >= p.getPosition().y && checkPos.y <= p.getPosition().y + p.getSize().y;
        return isCollideX && isCollideY;
	}
    bool isCollisionBullet(const Bullet& b) const {
        bool isCollideX = b.getPosition().x >= position.x && b.getPosition().x <= position.x + size.x;
        bool isCollideY = b.getPosition().y >= position.y && b.getPosition().y <= position.y + size.y;
        return isCollideX && isCollideY;
    }
};