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
    
    /*bottom = y - height/2.0f;
    top = y + height/2.0f;
    left = x - width/2.0f;
    right = x + width/2.0f;
     
     
    bottom = y;
    top = y + height;
    left = x;
    right = x + width;
     */
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

/*
void Entity::Draw(ShaderProgram* program){
    float sizeX = width;
    float sizeY = height;
    float vertices[] = {x, y, x+sizeX, y+sizeY, x, y+sizeY, x+sizeX, y+sizeY, x, y, x+sizeX, y};
    

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    float texCoords[] = {0.0, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0};
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, sprite.textureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}*/