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
x(x), y(y), width(width), height(height), xVelocity(0.0f), yVelocity(0.0f), xAcceleration(0.0f), yAcceleration(0.0f){}

void Paddle::Update(float timeElapsed){
    y += yVelocity * timeElapsed;
    yAcceleration *= 0.99;
}

void Paddle::Draw(ShaderProgram program,GLuint paddleTextureID){
    float paddleSizeX = 0.050f;
    float paddleSizeY = 0.10f;
    float vertices[] = {-1*paddleSizeX, -1*paddleSizeY, 1*paddleSizeX, 1*paddleSizeY, -1*paddleSizeX, 1*paddleSizeY, 1*paddleSizeX, 1*paddleSizeY, -1*paddleSizeX, -1*paddleSizeY, 1*paddleSizeX, -1*paddleSizeY};
    vertices[0] += x;
    vertices[1] += y;
    vertices[2] += x;
    vertices[3] += y;
    vertices[4] += x;
    vertices[5] += y;
    vertices[6] += x;
    vertices[7] += y;
    vertices[8] += x;
    vertices[9] += y;
    vertices[10] += x;
    vertices[11] += y;
    
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
        if (y > 1.0f - height / 2){
            y = 1.0f - height / 2;
        }
        else if (y < -1.0f + height / 2){
            y = -1.0f + height / 2;
        }
        yVelocity = 0.0f;
    }
}

float Paddle::getSpin() const{
    return yAcceleration;
}
