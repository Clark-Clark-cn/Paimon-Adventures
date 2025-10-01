#pragma once

#include "Atlas.h"
#include "Vector2.h"
#include "base.h"
#include <functional>

class Animation{
    Atlas* atlas;
    int currentFrame=0;
    unsigned int frameTime=100; // milliseconds
    unsigned int pastTime=0;
    bool loop=true;
    std::function<void()> onComplete=nullptr;

public:
    Animation()=default;
    ~Animation()=default;
    void setAtlas(Atlas* at){
        atlas=at;
    }
    void setFrameTime(unsigned int ms){
        frameTime=ms;
    }
    void setLoop(bool lp){
        loop=lp;
    }
    void setOnComplete(const std::function<void()>& cb){
        onComplete=cb;
    }
    void update(unsigned int deltaTime){
        if(!atlas) return;
        pastTime+=deltaTime;
        while(pastTime>=frameTime){
            pastTime-=frameTime;
            currentFrame++;
            if(currentFrame>=atlas->size()){
                if(loop){
                    currentFrame=0;
                }else{
                    currentFrame=atlas->size()-1;
                    if(onComplete){
                        onComplete();
                    }
                }
            }
        }
    }
    void draw(Vector2 pos){
        if(!atlas) return;
        IMAGE* img=atlas->getImage(currentFrame);
        if(img){
            putimage(pos, img);
        }
    }
    IMAGE* getCurrentFrame(){
        if(!atlas) return nullptr;
        return atlas->getImage(currentFrame);
    }
    void reset(){
        currentFrame=0;
        pastTime=0;
    }
    int getCurrentFrameIndex() const{
        return currentFrame;
    }
    int getFrameCount() const{
        if(!atlas) return 0;
        return atlas->size();
    }
    bool isFinished() const{
        if(!atlas) return true;
        return !loop && currentFrame==atlas->size()-1;
    }
    Vector2 getSize()
    {
        return { (float)atlas->getImage(0)->getwidth(),(float)atlas->getImage(0)->getheight() };
    }
};