#pragma once
#include "Point.h"
#include "Painter.h"
class Dust{
    public:
    Point pos;
    Point velocity;
    double lifetime = 1.0;
    Color c;
    bool timeToDead = false;
    
    void draw(Painter& painter) const;  
};