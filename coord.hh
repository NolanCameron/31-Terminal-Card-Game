#pragma once
#ifndef COORD_HH
#define COORD_HH
struct Coord{
    int y,x;
    Coord(int j = -1, int i = -1): y(j), x(i){}
};
#endif