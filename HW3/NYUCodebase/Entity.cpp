//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Max Lebedev on 10/4/15.
//  Copyright © 2015 Ivan Safrin. All rights reserved.
//

#include "Entity.hpp"


Entity::Entity(float x, float y, float width, float height):
x(x), y(y), width(width), height(height) {
    step = 0.0f;
    halfStep = 0;
    isAlive = true;
}

float Entity::bottom(){
    return y;
}
float Entity::top(){
    return y + height;
}
float Entity::left(){
    return x;
}
float Entity::right(){
    return x + width;
}

void Entity::Render(ShaderProgram *program){
    if (isAlive) {
        Draw(program);
    }
}

void Entity::Update(float elapsed){
    if(type == PLAYER){
        if(x > 1.5777f && velocity_x > 0){
            velocity_x = 0;
            x = 1.577;
        }
        if(x < -1.7777f && velocity_x < 0){
            velocity_x = 0;
            x = -1.777;
        }
        
        x += velocity_x * FIXED_TIMESTEP;
        y += velocity_y * FIXED_TIMESTEP;
    }
    else if (type == ALIEN){
        halfStep++;
        if(halfStep == 40){
            x += step;
            halfStep = 0;
        }
    }
    else if (type == BULLET){
        y += velocity_y * FIXED_TIMESTEP;
    }
}

bool Entity::collidesWith(Entity other){
    if (left() > other.right() || right() < other.left() || top() < other.bottom() || bottom() > other.top())
        return false;
    return true;
}

void Entity::Draw(ShaderProgram* program){
    float vertices[] = {x, y, x+width, y+height, x, y+height, x+width, y+height, x, y, x+width, y};
    sprite.Draw(program, vertices);
}