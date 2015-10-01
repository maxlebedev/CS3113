//
//  Paddle.cpp
//  NYUCodebase
//
//  Created by Max Lebedev on 9/27/15.
//  Copyright © 2015 Ivan Safrin. All rights reserved.
//

#include "Paddle.hpp"
#include <iostream>
using namespace std;

Paddle::Paddle(float x, float y, float width, float height):
x(x), y(y), width(width), height(height)   {
    xVelocity = 0.0f;
    yVelocity = 0.0f;
    xAcceleration =0.0f;
    yAcceleration =0.0f;
}

void Paddle::Update(float timeElapsed){
    y += yVelocity * timeElapsed;
    yAcceleration *= 0.99;
}

void Paddle::Draw(ShaderProgram program,GLuint paddleTextureID){
    float paddleSizeX = width;
    float paddleSizeY = height;
    float vertices[] = {x, y, x+paddleSizeX, y+paddleSizeY, x, y+paddleSizeY, x+paddleSizeX, y+paddleSizeY, x, y, x+paddleSizeX, y};
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    
    float texCoords[] = {0.0, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0};
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, paddleTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
}

float Paddle::top(){
    return y+height;
}

float Paddle::bottom(){
    return y;
}

float Paddle::left(){
    return x;
}
float Paddle::right(){
    return x+width;
}


void Paddle::Up(){
    yVelocity = 1.5f;
    if (yAcceleration == 0.0f){
        yAcceleration = -1.0f;
    }
}

void Paddle::Down(){
    yVelocity = -1.5f;
    if (yAcceleration == 0.0f){
        yAcceleration = 1.0f;
    }
}

void Paddle::Stop(){
    yAcceleration = 0.0f;
    if (yVelocity != 0.0f){
        if (y > 1.0f - height){
            y = 1.0f - height;
        }
        else if (y < -1.0f){
            y = -1.0f;
        }
        yVelocity = 0.0f;
    }
}
