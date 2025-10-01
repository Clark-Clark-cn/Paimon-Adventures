#pragma once

#include "player.h"
#include "Animation.h"
#include "bullet.h"

class Paimon: public Player{
    Animation leftAnim;
    Animation rightAnim;
    Animation* currentAnim=&leftAnim;
    Atlas* leftAtlas;
    Atlas* rightAtlas;
    std::vector<std::shared_ptr<Bullet>> MyBullets;
public:
    Paimon(Atlas* left, Atlas* right):leftAtlas(left), rightAtlas(right){
        leftAnim.setAtlas(leftAtlas);
        leftAnim.setFrameTime(100);
        leftAnim.setLoop(true);

        rightAnim.setAtlas(rightAtlas);
        rightAnim.setFrameTime(100);
        rightAnim.setLoop(true);

        size = leftAnim.getSize();
    }
    ~Paimon()=default;
    void Draw() override{
        Player::Draw();
        currentAnim->draw(position);
    }
    void Update(unsigned int deltaTime) override{
        Player::Update(deltaTime);
        currentAnim=isFacingRight?&rightAnim:&leftAnim;
        currentAnim->update(deltaTime);
        if(mp>=maxMp){
            skill();
        }
    }
    void skill(){
        mp=0;

    }
};