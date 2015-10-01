//
//  Ball.cpp
//  NYUCodebase
//
//  Created by Max Lebedev on 9/27/15.
//  Copyright © 2015 Ivan Safrin. All rights reserved.
//

#include "Ball.hpp"


Ball::Ball(float x, float y, float height, float width)	:x(x), y(y),width(width), height(height), xVelocity(0.0f), yVelocity(0.0f) {
}

void Ball::Update(float timeElapsed){
    //distance = velocity * time
    x += xVelocity * timeElapsed;
    y += yVelocity * timeElapsed;
    
    
}
//TODO: modelmatrix?
void Ball::Draw(ShaderProgram program,GLuint ballTextureID){
    float ballSize = 0.045f;
    float vertices[] = {-1*ballSize, -1*ballSize, 1*ballSize, 1*ballSize, -1*ballSize, 1*ballSize, 1*ballSize, 1*ballSize, -1*ballSize, -1*ballSize, 1*ballSize, -1*ballSize};
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
    
    glBindTexture(GL_TEXTURE_2D, ballTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
}


void Ball::hitTop(){
    yVelocity = -std::abs(yVelocity);
}

void Ball::hitBottom(){
    yVelocity = std::abs(yVelocity);
}

void Ball::hitRightPaddle(int paddleY,int paddleheight){
    float relativeIntersectY = (paddleY+(paddleheight/2)) - y;
    float normalizedRelativeIntersectionY = (relativeIntersectY/(paddleheight/2));
    float bounceAngle = normalizedRelativeIntersectionY * MAXBOUNCEANGLE;
    std::cout << "bounceAngle: " << bounceAngle << std::endl;
    //TODO use bounceangle look up slides
    xVelocity = -1.0f * (((float)rand()) / (float)RAND_MAX);
    yVelocity = 1.5f;

}

void Ball::hitLeftPaddle(int paddleY,int paddleheight){
    float relativeIntersectY = (paddleY+(paddleheight/2)) - y;
    float normalizedRelativeIntersectionY = (relativeIntersectY/(paddleheight/2));
    float bounceAngle = normalizedRelativeIntersectionY * MAXBOUNCEANGLE;
    std::cout << "bounceAngle: " << bounceAngle << std::endl;
    xVelocity = 1.0f * (((float)rand()) / (float)RAND_MAX);
    yVelocity = 1.5f;
    
}

void Ball::launch(){
    float r = (((float)rand()) / (float)RAND_MAX)-0.5f;
    xVelocity = 1.5f;//*r
    yVelocity = 1.5f;
}

void Ball::reset(){
    x = 0.0f;
    y = 0.0f;
    xVelocity = 0.0f;
    yVelocity = 0.0f;
}
