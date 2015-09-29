//
//  Ball.cpp
//  NYUCodebase
//
//  Created by Max Lebedev on 9/27/15.
//  Copyright © 2015 Ivan Safrin. All rights reserved.
//

#include "Ball.hpp"


Ball::Ball(float x, float y, float height, float width)	:x(x), y(y),width(width), height(height), xVelocity(0.0f), yVelocity(0.0f),
xAcceleration(1.0f), yAcceleration(1.0f){
    
}


void Ball::Update(float timeElapsed){
    //distance = velocity * time
    x += xVelocity * timeElapsed;
    y += yVelocity * timeElapsed;
    
    //velocity = acclaration * time
    xVelocity += xAcceleration * timeElapsed;
    yVelocity += yAcceleration * timeElapsed;
    
}

void Ball::Draw(){
    
    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    glTranslatef(x, y, 0);
    glScalef(1.0, 1.0, 1.0);
    
    GLfloat points[] = {-width / 4, -height/4, width / 4, -height/4, width / 4, height/4, -width / 4, -height/4, width / 4, height/4, -width / 4, height/4};
    
    glVertexPointer(2, GL_FLOAT, 0, points);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    
}

const std::array<float, 4>& Ball::hitBox()const{
    std::array<float, 4> stuff = { x, y, width, height };
    return stuff;
}

//Play around
void Ball::hitTop(){
    yVelocity = -std::abs(yVelocity);
    xAcceleration = 0.0f;
    yAcceleration = 0.0f;
}

void Ball::hitBottom(){
    yVelocity = std::abs(yVelocity);
    xAcceleration = 0.0f;
    yAcceleration = 0.0f;
}

void Ball::hitRightPaddle(int paddleY,int paddleheight){
    float relativeIntersectY = (paddleY+(paddleheight/2)) - y;
    float normalizedRelativeIntersectionY = (relativeIntersectY/(paddleheight/2));
    float bounceAngle = normalizedRelativeIntersectionY * MAXBOUNCEANGLE;
    
    //TODO use bounceangle
    //xVelocity = xVelocity* cos(bounceAngle);
    //yVelocity = xVelocity*sin(bounceAngle)*-1;
    xVelocity = -1.5f;
    yVelocity = 1.5f;

}

void Ball::hitLeftPaddle(int paddleY,int paddleheight){
    float relativeIntersectY = (paddleY+(paddleheight/2)) - y;
    float normalizedRelativeIntersectionY = (relativeIntersectY/(paddleheight/2));
    float bounceAngle = normalizedRelativeIntersectionY * MAXBOUNCEANGLE;
    //xVelocity = xVelocity* cos(bounceAngle);
    //yVelocity = xVelocity*sin(bounceAngle)*-1;
    xVelocity = 1.5f;
    yVelocity = 1.5f;
    
}

void Ball::launch(){
    float r = (((float)rand()) / (float)RAND_MAX)-0.5f;
    xVelocity = 1.5f*r;
    yVelocity = 1.5f;
    xAcceleration = 0;
    yAcceleration = 0;
}

void Ball::reset(){
    x = 0.0f;
    y = 0.0f;
    xVelocity = 0.0f;
    yVelocity = 0.0f;
    xAcceleration = 0.0f;
    yAcceleration = 0.0f;
}
