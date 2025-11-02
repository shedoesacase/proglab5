#pragma once
#include "Painter.h"
#include "Point.h"
#include "Velocity.h"

class Ball {
private:
    Velocity velocity_;
    Point center_;
    double mass_;
    double radius_;
    Color color_;
    bool isCollidable_;

public:
    Ball() : center_(0,0), radius_(20), color_(255, 255, 0) {}
    Ball(Point center, Velocity velocity, double radius, Color color, bool isCollidable){
        center_ = center;
        velocity_ = velocity;
        radius_ = radius;
        color_ = color;
        isCollidable_ = isCollidable;
        mass_ = radius * radius;
    }
    bool isCollidable() const { return isCollidable_; }

    void setVelocity(const Velocity& velocity);
    Velocity getVelocity() const;
    void draw(Painter& painter) const;
    void setCenter(const Point& center);
    Point getCenter() const;
    double getRadius() const;
    double getMass() const;
};
