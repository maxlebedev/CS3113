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
    isAlive = true;
    isStatic = false;
    friction = 0.01f;
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

float lerp(float v0, float v1, float t) {
    return (1.0-t)*v0 + t*v1;
}

void Entity::Update(float elapsed){
    if (isStatic) {
        //do nothing
        return;
    }
    if (type == BULLET){
        y += velocity_y * FIXED_TIMESTEP;
        return;
    }
    
    //The following screws with aliens
    velocity_x = lerp(velocity_x, 0.0f, FIXED_TIMESTEP * friction);
    velocity_y = lerp(velocity_y, 0.0f, FIXED_TIMESTEP * friction);
    velocity_x += acceleration_x * FIXED_TIMESTEP;
    velocity_y += acceleration_y * FIXED_TIMESTEP;
    x += velocity_x * FIXED_TIMESTEP;
    y += velocity_y * FIXED_TIMESTEP;

    velocity_y += GRAVITY * elapsed;
    
    if(type == PLAYER){
        if(x > 1.5777f && velocity_x > 0){
            velocity_x = 0;
            x = 1.577;
        }
        if(x < -1.7777f && velocity_x < 0){
            velocity_x = 0;
            x = -1.777;
        }
    }
}

bool Entity::collidesWith(Entity other){
    if (left() > other.right() || right() < other.left() || top() < other.bottom() || bottom() > other.top())
        return false;
    return true;
}

void Entity::uncollide(Entity other){
    //TODO do some actual uncollision here.
}

void Entity::Draw(ShaderProgram* program){
    float vertices[] = {x, y, x+width, y+height, x, y+height, x+width, y+height, x, y, x+width, y};
    sprite.Draw(program, vertices);
}