#pragma once

#include <graphics.h>
#include <vector>

class Atlas{
    std::vector<IMAGE*> images;
public:
    Atlas()=default;
    ~Atlas()
    {
        for (auto& i : images)
            delete i;
    }
    void load(const wchar_t* path, int num){
        for(int i=0;i<num;i++){
            IMAGE* img=new IMAGE;
            wchar_t fullpath[256];
            swprintf(fullpath, 256, path, i);
            loadimage(img, fullpath);
            images.push_back(img);
        }
    }
    IMAGE* getImage(int index){
        if(index<0 || index>=images.size()) return images.back();
        return images[index];
    }
    int size() const{
        return images.size();
    }
};