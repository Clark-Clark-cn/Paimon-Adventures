#pragma once

#include <graphics.h>

#include <algorithm>
#include "Atlas.h"
#include "Vector2.h"
#include "base.h"

extern bool debug;
extern IMAGE img_shadow_player;

class Player{
protected:
    Vector2 position{640,360};
    Vector2 velocity{0,0};
    Vector2 size{20,20};
    bool left_key=false;
    bool right_key=false;
    bool up_key=false;
    bool down_key=false;
    bool skill_key=false;
    bool isFacingRight=false;
    int mp=0;
    int maxMp=100;
    int hp=100;
    const float speed=0.2f; // pixels per millisecond
public:
    Player()=default;
    ~Player()=default;
    virtual void Draw(){
        Vector2 pos_shadow;
        pos_shadow.x=position.x+size.x/2-img_shadow_player.getwidth()/2;
        pos_shadow.y=position.y+size.y-8;

        putimage(pos_shadow, &img_shadow_player);
        if(debug){
            setlinecolor(RED);
            rectangle(position.x-size.x/2, position.y-size.y/2, position.x+size.x/2, position.y+size.y/2);
        }
    }
    virtual void Update(unsigned int deltaTime){
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
        switch (msg.message)
        {
        case WM_KEYDOWN:
            switch (msg.vkcode)
            {
            case VK_LEFT:
            case 'A':
                left_key = true;
                break;
            case VK_RIGHT:
            case 'D':
                right_key = true;
                break;
            case VK_UP:
            case 'W':
                up_key = true;
                break;
            case VK_DOWN:
            case 'S':
                down_key = true;
                break;
            case VK_SPACE:
            case 'E':
                skill_key = true;
                break;
            }
            break;
        case WM_KEYUP:
            switch (msg.vkcode)
            {
            case VK_LEFT:
            case 'A':
                left_key = false;
                break;
            case VK_RIGHT:
            case 'D':
                right_key = false;
                break;
            case VK_UP:
            case 'W':
                up_key = false;
                break;
            case VK_DOWN:
            case 'S':
                down_key = false;
                break;
            case VK_SPACE:
            case 'E':
                skill_key = false;
                break;
            }
            break;
        
        default:
            break;
        }
    }

    void setPosition(const Vector2& pos){position=pos;}
    Vector2 getPosition() const{return position;}
	Vector2 getSize() const { return size; }
};
