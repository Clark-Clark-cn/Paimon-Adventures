#pragma once 

#include <functional>

class Timer
{
    std::function<void()> Callback;
    unsigned int Interval=0; // milliseconds
    unsigned int pastTime=0;
    bool loop=false;
public:
    Timer()=default;
    ~Timer()=default;

    void setInterval(unsigned int ms){
        Interval=ms;
    }
    void setCallback(const std::function<void()>& cb){
        Callback=cb;
    }
    void setLoop(bool lp){
        loop=lp;
    }
    void update(unsigned int deltaTime){
        if(Interval==0 || !Callback) return;
        pastTime+=deltaTime;
        while(pastTime>=Interval){
            Callback();
            if(loop){
                pastTime-=Interval;
            }else{
                pastTime=0;
            }
        }
    }
};
