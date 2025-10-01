#pragma once

#include "player.h"
#include "Animation.h"
#include "bullet.h"

extern std::vector<std::shared_ptr<Bullet>> bullets_custom;


class Warrior: public Player{
    Animation leftIdleAnim;
    Animation rightIdleAnim;
    Animation leftRunAnim;
    Animation rightRunAnim;
    Animation* currentAnim=&leftIdleAnim;
    Atlas leftIdleAtlas;
    Atlas rightIdleAtlas;
    Atlas leftRunAtlas;
    Atlas rightRunAtlas;
    IMAGE img_attack_up, img_attack_down, img_attack_left, img_attack_right;
public:
    Warrior():Player(){
        speed=config.get("player.warrior.speed").asFloat();
        max_hp=config.get("player.warrior.hp").asInt();
        hp=max_hp;
        leftIdleAtlas.load(L"img\\warrior_idle_left_%d.png", 4);
        rightIdleAtlas.load(L"img\\warrior_idle_right_%d.png", 4);
        leftRunAtlas.load(L"img\\warrior_walk_left_%d.png", 8);
        rightRunAtlas.load(L"img\\warrior_walk_right_%d.png", 8);
        loadimage(&img_attack_up, L"img\\attack_up.png");
        loadimage(&img_attack_down, L"img\\attack_down.png");
        loadimage(&img_attack_left, L"img\\attack_left.png");
        loadimage(&img_attack_right, L"img\\attack_right.png");
        leftIdleAnim.setAtlas(&leftIdleAtlas);
        leftIdleAnim.setFrameTime(150);
        leftIdleAnim.setLoop(true);

        rightIdleAnim.setAtlas(&rightIdleAtlas);
        rightIdleAnim.setFrameTime(150);
        rightIdleAnim.setLoop(true);

        leftRunAnim.setAtlas(&leftRunAtlas);
        leftRunAnim.setFrameTime(100);
        leftRunAnim.setLoop(true);

        rightRunAnim.setAtlas(&rightRunAtlas);
        rightRunAnim.setFrameTime(100);
        rightRunAnim.setLoop(true);

        size = leftIdleAnim.getSize();

        bullets_around.resize(config.get("player.warrior.around_bullets").asInt());
        int addMp=config.get("player.warrior.around_bullets_addMp").asInt();
        for(int i=0;i<bullets_around.size();i++){
            bullets_around[i]=std::make_shared<Bullet>();
            bullets_around[i]->setOnHit([this, addMp](){
                this->addMP(addMp);
            });
        }
    }
    ~Warrior()=default;
    void Draw()override{
        Player::Draw();
        if(drawIMG&&currentAnim){
            currentAnim->draw(position);
        }
    }
    void Update(unsigned int deltaTime)override{
        Player::Update(deltaTime);
        currentAnim=&(isFacingRight)?
        (velocity.x==0?&rightIdleAnim:&rightRunAnim):
        (velocity.x==0?&leftIdleAnim:&leftRunAnim);
        if(currentAnim) currentAnim->update(deltaTime);
        if(mp>=maxMp&&skill_key)skill();
    }
    void skill(){
        if(mp<maxMp) return;
        mp=0;
        static int addMp=config.get("player.warrior.skill_addMp").asInt();
        float bulletSpeed=config.get("player.custom_bullets.speed").asFloat();
        bullets_custom.push_back(std::make_shared<Bullet>(&img_attack_up));
        bullets_custom.back()->setPosition({position.x, position.y - size.y / 2});
        bullets_custom.back()->setVelocity({0, -bulletSpeed});
        bullets_custom.back()->setOnHit([this](){
            this->addMP(addMp);
        });
        bullets_custom.push_back(std::make_shared<Bullet>(&img_attack_down));
        bullets_custom.back()->setPosition({position.x, position.y + size.y / 2});
        bullets_custom.back()->setVelocity({0, bulletSpeed});
        bullets_custom.back()->setOnHit([this](){
            this->addMP(addMp);
        });
        bullets_custom.push_back(std::make_shared<Bullet>(&img_attack_left));
        bullets_custom.back()->setPosition({position.x - size.x / 2, position.y});
        bullets_custom.back()->setVelocity({-bulletSpeed, 0});
        bullets_custom.back()->setOnHit([this](){
            this->addMP(addMp);
        });
        bullets_custom.push_back(std::make_shared<Bullet>(&img_attack_right));
        bullets_custom.back()->setPosition({position.x + size.x / 2, position.y});
        bullets_custom.back()->setVelocity({bulletSpeed, 0});
        bullets_custom.back()->setOnHit([this](){
            this->addMP(addMp);
        });
    }
};