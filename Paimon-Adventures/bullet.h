#pragma once

#include <graphics.h>
#include <vector>
#include "Vector2.h"
#include <memory>
#include "base.h"

double RADIAL_SPEED=0.0045;
double TANGENT_SPEED=0.0015;
extern Player* player;
extern std::vector<std::shared_ptr<class Bullet>> bullets;

class Bullet{
    Vector2 position;
    IMAGE* img=nullptr;
    bool active=true;
    std::function<void()> onHit=nullptr;
public:
    Bullet(){}
    Bullet(IMAGE* image): img(image){}
    ~Bullet()=default;
    static void UpdateBullets(unsigned int deltaTime){
        double radian_interval=2*3.1415926/bullets.size();
        double radius=100+25*sin(GetTickCount()*RADIAL_SPEED);
        for(int i=0;i<bullets.size();i++){
            double radian=i*radian_interval+GetTickCount()*TANGENT_SPEED;
            bullets[i]->position.x=player->getPosition().x+player->getSize().x/2+radius*sin(radian);
            bullets[i]->position.y=player->getPosition().y+player->getSize().y/2+radius*cos(radian);
        }
    }
    void Draw(){
        if(!active) return;
        if(img){
            putimage(position, img);
        }
        else{
            setfillcolor(RED);
            fillcircle(position.x, position.y, 5);
        }
    }
	bool isActive() const { return active; }
    Vector2 getPosition() const { return position; }
	void deactivate() { active = false; }
    void setOnHit(const std::function<void()>& cb){onHit=cb;}
    void hit(){
        if(onHit) onHit();
    }
};