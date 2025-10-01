#pragma once

#include <graphics.h>
#include <vector>
#include "config.h"
#include "Vector2.h"
#include <memory>
#include "base.h"

double RADIAL_SPEED=config.get("bullet.radial_speed").asFloat();
double TANGENT_SPEED=config.get("bullet.tangent_speed").asFloat();
extern Player* player;
extern std::vector<std::shared_ptr<class Bullet>> bullets_around;

class Bullet{
    Vector2 position;
    Vector2 velocity{0,0};
    IMAGE* img=nullptr;
    bool active=true;
    std::function<void()> onHit=nullptr;
public:
    Bullet(){}
    Bullet(IMAGE* image): img(image){}
    ~Bullet()=default;
    static void UpdateBulletsAround(unsigned int deltaTime){
        double radian_interval=2*3.1415926/bullets_around.size();
        double radius=100+25*sin(GetTickCount()*RADIAL_SPEED);
        for(int i=0;i<bullets_around.size();i++){
            double radian=i*radian_interval+GetTickCount()*TANGENT_SPEED;
            bullets_around[i]->position.x=player->getPosition().x+player->getSize().x/2+radius*sin(radian);
            bullets_around[i]->position.y=player->getPosition().y+player->getSize().y/2+radius*cos(radian);
        }
    }
    void update(unsigned int deltaTime){
        if(!active) return;
        position+=velocity*deltaTime;
        if(position.x<0 || position.x>1280 || position.y<0 || position.y>720){
            active=false;
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
    void setPosition(const Vector2& pos){position=pos;}
    void setVelocity(const Vector2& vel){velocity=vel;}
};