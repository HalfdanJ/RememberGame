#pragma once
#include "ofMain.h"


class Tile {    
public:
    void draw(float x, float y);
    
    ofImage image;
    ofImage collisionImage;
    int id;
    
    int neighbors[4];
    
};