#pragma once

#include "player.h"
#include "Animation.h"
#include "bullet.h"

class Paimon: public Player{
    Animation leftAnim;
    Animation rightAnim;
    Animation* currentAnim=&leftAnim;
    Atlas leftAtlas;
    Atlas rightAtlas;
public:
    Paimon():Player(){
        speed=config.get("player.paimon.speed").asFloat();
        max_hp=config.get("player.paimon.hp").asInt();
        hp=max_hp;
        leftAtlas.load(L"img\\paimon_left_%d.png", 6);
        rightAtlas.load(L"img\\paimon_right_%d.png", 6);
        leftAnim.setAtlas(&leftAtlas);
        leftAnim.setFrameTime(100);
        leftAnim.setLoop(true);

        rightAnim.setAtlas(&rightAtlas);
        rightAnim.setFrameTime(100);
        rightAnim.setLoop(true);

        size = leftAnim.getSize();
        bullets_around.resize(config.get("player.paimon.around_bullets").asInt());
        int addMp=config.get("player.paimon.around_bullets_addMp").asInt();
        for(auto& b : bullets_around) {
            b = std::make_shared<Bullet>();
            b->setOnHit([this, &b, addMp]() {
                this->addMP(addMp);
            });
        }
    }
    ~Paimon()=default;
    void Draw() override{
        Player::Draw();
        if(drawIMG)currentAnim->draw(position);
    }
    void Update(unsigned int deltaTime) override{
        Player::Update(deltaTime);
        currentAnim=isFacingRight?&rightAnim:&leftAnim;
        currentAnim->update(deltaTime);
        if(mp>=maxMp&&skill_key){
            skill();
        }
    }
    void skill(){
        mp=0;
        static int addMp=config.get("player.paimon.skill_addMp").asInt();
        bullets_around.emplace_back(std::make_shared<Bullet>());
        bullets_around.back()->setOnHit([this]() {
            this->addMP(addMp);
        });
    }
};