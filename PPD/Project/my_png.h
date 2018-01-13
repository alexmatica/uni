//
// Created by fmatica on 1/13/18.
//

#ifndef PROJECT_MY_PNG_H
#define PROJECT_MY_PNG_H

#endif //PROJECT_MY_PNG_H

#include <png.h>

class MyPng{
public:
    MyPng();
    ~MyPng();
    explicit MyPng(const char *fileName);
    void write_png(const char *fileName);

    png_bytep* row_pointers;
    png_bytep* copy;
    int getWidth(){
        return this->width;
    }
    int getHeight(){
        return this->height;
    }

private:
    int height;
    int width;
    png_byte color_type;
    png_byte bit_depth;

    void read_png(const char *fileName);
};