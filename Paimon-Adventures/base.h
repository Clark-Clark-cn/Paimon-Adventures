#pragma once

#include <graphics.h>
#include "Vector2.h"

#pragma comment(lib, "MSIMG32.LIB")

void putimage(Vector2 pos, IMAGE* img){
    if(!img)MessageBox(NULL, L"putimage: img is null", L"Error", MB_OK);
    else{
        int w=img->getwidth();
        int h=img->getheight();
        AlphaBlend(GetImageHDC(NULL), pos.x, pos.y, w, h, 
            GetImageHDC(img), 0, 0, w, h, {AC_SRC_OVER,0,255,AC_SRC_ALPHA});
    }
}