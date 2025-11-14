#pragma once
#ifndef RECTANGLEAREA_HH
#define RECTANGLE_HH
struct rectangleArea{
    int height;
    int width;
    int x;
    int y;
    rectangleArea(int h,int w,int j, int i): height(h), width(w), x(i), y(j){}
};
#endif