#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include <iostream>

#include "Ball.hpp"
#include "Paddle.hpp"

#include "ShaderProgram.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

#define ORTHOTOP 1.0f
#define ORTHOBOT -1.0f
#define ORTHOLEFT -1.33f
#define ORTHORIGHT 1.33f

using std::array;
Ball* ball;
Paddle* paddle1;
Paddle* paddle2;

Matrix projectionMatrix;
Matrix modelMatrix;
Matrix viewMatrix;


float lastFrameTicks;

bool newGame;
GLuint ballTextureID;
GLuint paddleTextureID;

//LBRT order
bool collided(const std::array<float, 4> &rect1, const std::array<float, 4> &rect2){
    if(rect2[0] > (rect1[0]+rect1[2]) || rect2[1] > (rect1[1]+rect1[3]) || (rect2[0]+rect2[2]) < rect1[0] || (rect2[0]+rect2[3]) < rect1[1])
        return false;
    return true;
}

void HandleCollisions(){
    const array<float, 4> boxBall = {ball->x, ball->y, ball->width, ball->height};
    const array<float, 4> boxPaddle1 = {paddle1->x, paddle1->y, paddle1->width, paddle1->height};
    const array<float, 4> boxPaddle2 = {paddle2->x, paddle2->y, paddle2->width, paddle2->height};
    
    //Top
    if (boxBall[1] + boxBall[3] / 2 >= ORTHOTOP)
        ball->hitTop();
    if (boxPaddle1[1] + boxPaddle1[3] / 2 > ORTHOTOP)
        paddle1->Stop();
    if (boxPaddle2[1] + boxPaddle2[3] / 2 > ORTHOTOP)
        paddle2->Stop();
    
    //Bottom
    if (boxBall[1] - boxBall[3] / 2 <= ORTHOBOT)
        ball->hitBottom();
    if (boxPaddle1[1] - boxPaddle1[3] / 2 < ORTHOBOT)
        paddle1->Stop();
    if (boxPaddle2[1] - boxPaddle2[3] / 2 < ORTHOBOT)
        paddle2->Stop();
    
    //Right
    if (boxBall[0] - boxBall[2] / 2 >= ORTHORIGHT){
        ball->reset();
        newGame = true;
    }
    
    //Left
    if (boxBall[0] + boxBall[2] / 2 <= ORTHOLEFT){
        ball->reset();
        newGame = true;
    }
    //Paddle 1
    if (collided(boxBall,boxPaddle1))
        ball->hitLeftPaddle(boxPaddle1[3], PADDLEHEIGHT);
    //Paddle 2
    if (collided(boxBall, boxPaddle2))
        ball->hitRightPaddle(boxPaddle2[3],PADDLEHEIGHT);
}

GLuint LoadTexture(const char *image_path) {
    SDL_Surface *surface = IMG_Load(image_path);
    GLuint textureID;
    glGenTextures(0, &textureID);
    //    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(surface);
    return textureID;
}

ShaderProgram Setup(){//Ball* ball, Paddle* paddle,Paddle* paddle2
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif

    ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    
    //set up the matrix stuff
    projectionMatrix.setOrthoProjection(-1.7777f, 1.77777f, -1.0f, 1.0f, -1.0f, 1.0f);
    
    glViewport(0, 0, 800, 600);
    
    //background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //black
    
    //Setup my entities here!
    ball = new Ball(0.0f, 0.0f, 0.1f, 0.1f);
    paddle1 = new Paddle(-1.0f, 0.0f, 0.1f, 0.30f);
    paddle2 = new Paddle(1.0f, 0.0f, 0.1f, 0.30f);

    
    program.setModelMatrix(modelMatrix);
    program.setProjectionMatrix(projectionMatrix);
    program.setViewMatrix(viewMatrix);

    glUseProgram(program.programID);
    
    lastFrameTicks = 0.0f;
    
    newGame = true;

    return program;
}

void ProcessEvents(bool& done){
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        //Exit
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        
        //Keyboard 2 players ONLY
        else if (event.type == SDL_KEYDOWN){
            //Player 1
            if (event.key.keysym.scancode == SDL_SCANCODE_W)
                paddle1->Up();
            else if (event.key.keysym.scancode == SDL_SCANCODE_S)
                paddle1->Down();
            //Player 2
            if (event.key.keysym.scancode == SDL_SCANCODE_UP)
                paddle2->Up();
            else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
                paddle2->Down();
        }
        
        else if (event.type == SDL_KEYUP){
            //Player 1
            if (event.key.keysym.scancode == SDL_SCANCODE_W)
                paddle1->Stop();
            else if (event.key.keysym.scancode == SDL_SCANCODE_S)
                paddle1->Stop();
            
            //Player 2
            if (event.key.keysym.scancode == SDL_SCANCODE_UP)
                paddle2->Stop();
            else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
                paddle2->Stop();
        }
    }
}

void Update(){
    //Physics
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    
    ball->Update(elapsed);
    paddle1->Update(elapsed);
    paddle2->Update(elapsed);
    
    if (newGame){
        ball->launch();
        newGame = false;
    }
    //Collisions
    HandleCollisions();
}

void Render(ShaderProgram program){
    //Clears the screen to the set color
    glClear(GL_COLOR_BUFFER_BIT);
    
    //Setup Transforms Draw 
    ball->Draw(program, ballTextureID);
    paddle1->Draw(program, paddleTextureID);
    paddle2->Draw(program, paddleTextureID);
    SDL_GL_SwapWindow(displayWindow);
}

int main(int argc, char *argv[]){
    ShaderProgram program = Setup();
    ballTextureID = LoadTexture(RESOURCE_FOLDER"ballBlue.png");
    paddleTextureID = LoadTexture(RESOURCE_FOLDER"brickWall.png");
    bool done = false;
    while (!done) {
        
        ProcessEvents(done);
        Update();
        Render(program);
        
        glUseProgram(program.programID);
    }
    SDL_Quit();
    return 0;
}