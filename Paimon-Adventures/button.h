#pragma once

#include <graphics.h>
#include <functional>

class Button{
    RECT rect;
    IMAGE* img_idle;
    IMAGE* img_hover;
    IMAGE* img_pressed;
    enum Status{
        IDLE,
        HOVER,
        PRESSED
    };
    Status status=IDLE;
    std::function<void()> onClick=nullptr;
public:
    Button()=default;
    Button(RECT r, IMAGE* idle, IMAGE* hover, IMAGE* pressed){
        rect=r;
        img_idle=idle;
        img_hover=hover;
        img_pressed=pressed;
    }
    ~Button()=default;
    void setRect(RECT r){rect=r;}
    void setImages(IMAGE* idle, IMAGE* hover, IMAGE* pressed){
        img_idle=idle;
        img_hover=hover;
        img_pressed=pressed;
    }
    void setOnClick(const std::function<void()>& cb){onClick=cb;}
    void update(ExMessage& msg){
        switch(msg.message){
        case WM_MOUSEMOVE:
            if(status==IDLE&&PtInRect(&rect, {msg.x, msg.y}))
                status=HOVER;
            else if(status==HOVER&&!PtInRect(&rect, {msg.x, msg.y}))
                status=IDLE;
            break;
        case WM_LBUTTONDOWN:
            if(status==HOVER)
                status=PRESSED;
            break;
        case WM_LBUTTONUP:
            if(status==PRESSED){
                if(onClick) onClick();
            }
            break;
        }
    }
    void draw(){
        IMAGE* img=nullptr;
        switch(status){
            case IDLE: img=img_idle; break;
            case HOVER: img=img_hover; break;
            case PRESSED: img=img_pressed; break;
        }
        if(img){
            putimage(rect.left, rect.top, img);
        }
    }
};