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
    friction = 1.5f;
    
    //just player
    leftContact = false;
    rightContact = false;
//    topContact = false;
    bottomContact = false;

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
float Entity::xmid(){
    return x + width/2;
}
float Entity::ymid(){
    return y + height/2;
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
    float oldx = x;

    velocity_x = lerp(velocity_x, 0.0f, FIXED_TIMESTEP * friction);
    velocity_y = lerp(velocity_y, 0.0f, FIXED_TIMESTEP * friction);
    velocity_x += acceleration_x * FIXED_TIMESTEP;
    velocity_y += acceleration_y * FIXED_TIMESTEP;
    x += velocity_x * FIXED_TIMESTEP;
    y += velocity_y * FIXED_TIMESTEP;

  
    velocity_y += GRAVITY * elapsed;
    if (fabs(velocity_x) < 0.001f)
        velocity_x = 0;
    
    if (x != oldx){//the theory is that this stops random locks
        leftContact = false;
        rightContact = false;
    }
        
}

bool Entity::collidesWith(Entity other){
    if (left() > other.right() || right() < other.left() || top() < other.bottom() || bottom() > other.top())
        return false;
    return true;
}
/* old
void Entity::uncollide(Entity other){
    //TODO do this for all four sides
    if (bottom() < other.top() && bottom() > other.bottom()) {//move up
        float ydist = fabs(( ymid() )-(other.ymid()));
        printf("move up: %f\n", ydist);
        //float ypen = fabs(ydist-(height/2)-(other.height/2));
        //y += ypen + 0.0001f;
        y = other.top()+0.001f;
        velocity_y = 0;
    }
    else if (top() > other.bottom() && top() < other.top()) {//move down
        float ydist = fabs(( ymid() )-(other.ymid()));
        printf("move down: %f\n", ydist);
        //float ypen = fabs(ydist-(height/2)-(other.height/2));
        //y -= ypen - 0.0001f;
        y = other.bottom()-height-0.001f;
        velocity_y = 0;
    }
    else if(left() < other.right() && left() > other.left()){//move right
        float xdist = fabs(( xmid() )-(other.xmid()));
        printf("move right: %f\n", xdist);
        //float xpen = fabs(xdist-(width/2)-(other.width/2));
        //x += xpen + 0.0001f;
        x = other.right() + 0.001f;
        velocity_x = 0;
    }
    else if(right() < other.right() && right() > other.left()){//move right
        float xdist = fabs(( xmid() )-(other.xmid()));
        printf("move left: %f\n", xdist);
        //float xpen = fabs(xdist-(width/2)-(other.width/2));
        //x -= xpen - 0.01f;
        x = other.left()-width-0.001f;
        velocity_x = 0;
    }
}
 //*/

//*
void Entity::uncollide(Entity other){
        //Determine which corner has penetrated. Push out to the closes side
        //Worst case scnario I "fall through"
    if ((bottom() < other.top() && bottom() > other.bottom()) && (left() < other.right() && left() > other.left()) ) {
        //top right coner
        float xdiff = other.right()-left();
        float ydiff = other.top()-bottom();
        if (xdiff < ydiff) {
            //push right
            x = other.right() + COLLISION_OFFSET;
            ///velocity_x = 0;
            leftContact = true;
            rightContact = false;
        }
        else{
            //push up
            y = other.top() + COLLISION_OFFSET;
            velocity_y = 0;
            bottomContact = true;
        }
    }
    else if ((bottom() < other.top() && bottom() > other.bottom()) && (right() > other.left() && right() < other.right())) {
        //top left coner
        float xdiff = right()-other.left();
        float ydiff = other.top()-bottom();
        if (xdiff < ydiff) {
            //push left
            x = other.left() - width - COLLISION_OFFSET;
            ///velocity_x = 0;
            rightContact = true;
            leftContact = false;
        }
        else{
            //push up
            y = other.top() + COLLISION_OFFSET;
            velocity_y = 0;
            bottomContact = true;
        }
    }
    else if ((top() > other.bottom() && top() < other.top()) && (left() < other.right() && left() > other.left())) {
        //bottom right coner
        float xdiff = other.right()-left();
        float ydiff = top()-other.bottom();
        if (xdiff < ydiff) {
            //push right
            x = other.right() + COLLISION_OFFSET;
            velocity_x = 0;
            leftContact = true;
            rightContact = false;
        }
        else{
            //push down
            y = other.bottom() - height - COLLISION_OFFSET;
            velocity_y = 0;
            velocity_x = 0;
        }
    }
    else if ((top() > other.bottom() && top() < other.top()) && (right() > other.left() && right() < other.right())) {
        //bottom left coner
        float xdiff = right()-other.left();
        float ydiff = top()-other.bottom();
        if (xdiff < ydiff) {
            //push left
            x = other.left() - width - COLLISION_OFFSET;
            velocity_x = 0;
            rightContact = true;
            leftContact = false;
        }
        else{
            //push down
            y = other.bottom() - height - COLLISION_OFFSET;
            velocity_y = 0;
            velocity_x = 0;
        }
    }
}
//*/


void Entity::Draw(ShaderProgram* program){
    float vertices[] = {x, y, x+width, y+height, x, y+height, x+width, y+height, x, y, x+width, y};
    sprite.Draw(program, vertices);
}