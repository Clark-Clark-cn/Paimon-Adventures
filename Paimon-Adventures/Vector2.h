#pragma once

#include <cmath>

class Vector2{
public:
    float x=0;
    float y=0;
    Vector2()=default;
    Vector2(float _x, float _y):x(_x),y(_y){}
    Vector2 operator+(const Vector2& v) const{
        return Vector2(x+v.x, y+v.y);
    }
    Vector2 operator-(const Vector2& v) const{
        return Vector2(x-v.x, y-v.y);
    }
    Vector2 operator*(float f) const{
        return Vector2(x*f, y*f);
    }
    Vector2 operator/(float f) const{
        return Vector2(x/f, y/f);
    }
    Vector2& operator+=(const Vector2& v){
        x+=v.x;
        y+=v.y;
        return *this;
    }
    Vector2& operator-=(const Vector2& v){
        x-=v.x;
        y-=v.y;
        return *this;
    }
    Vector2& operator*=(float f){
        x*=f;
        y*=f;
        return *this;
    }
    Vector2& operator/=(float f){
        x/=f;
        y/=f;
        return *this;
    }
    float length() const{
        return std::sqrt(x*x+y*y);
    }
    Vector2 normalized() const{
        float len=length();
        if(len==0) return Vector2(0,0);
        return Vector2(x/len, y/len);
    }
};