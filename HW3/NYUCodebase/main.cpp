#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "vector"
#include "Entity.hpp"
#include "SheetSprite.hpp"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

enum {STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER};
int state;

// 60 FPS (1.0f/60.0f)
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

#define LWALL -1.7
#define RWALL 1.5

float timeLeftOver = 0.0f;
float lastFrameTicks = 0.0f;

std::vector<Entity> bullets;

//40 is player
std::vector<Entity> entities;

std::vector<Entity> text;


//TODO: 3 states
//TODO: display text
//TODO: enemy bullets
//TODO: bullet limit

SDL_Window* displayWindow;

GLuint LoadTexture(const char *image_path) {
    SDL_Surface *surface = IMG_Load(image_path);
    GLuint textureID;
    glGenTextures(0, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(surface);
    return textureID;
}

void initEntityArray(){
    GLuint spriteSheetTexture = LoadTexture(RESOURCE_FOLDER"sheet.png");
    for (float i = 0; i < 4; i ++){
        for(float j = 0.0f; j < 10; j++){
            Entity myEntity = *new Entity(-1.35+((3*j)/10.0f),0.8-(i/8.0f),0.1,0.1) ;
            myEntity.sprite = SheetSprite(spriteSheetTexture, 425.0f/1024.0f, 468.0f/1024.0f, 93.0f/1024.0f, 84.0f/1024.0f, 0.2);
            myEntity.type = ALIEN;
            myEntity.step = 0.1f;
            entities.push_back(myEntity);
        }
    }
    //now the player
    Entity player = *new Entity(0,-0.8,0.2,0.2) ;
    player.sprite = SheetSprite(spriteSheetTexture, 247.0f/1024.0f, 84.0f/1024.0f, 99.0f/1024.0f, 75.0f/1024.0f, 0.2);
    player.type = PLAYER;
    entities.push_back(player);
}


//this does not yet work
void initMenuScreen(){
    GLuint sSTex = LoadTexture(RESOURCE_FOLDER"font2.png");
    Entity letter = *new Entity(0,0,0.01,0.1);
    letter.sprite = SheetSprite(sSTex, 0, 0, 1.0f, 1.0f, 0.2);
    text.push_back(letter);
}

void shootBullet() {
    GLuint sSTex = LoadTexture(RESOURCE_FOLDER"sheet.png");
    Entity newBullet = *new Entity(entities[40].x+entities[40].width/2.0f,entities[40].y+0.2,0.01,0.1);
    newBullet.sprite = SheetSprite(sSTex, 843.0f/1024.0f, 116.0f/1024.0f, 13.0f/1024.0f, 57.0f/1024.0f, 0.2);
    newBullet.type = BULLET;
    newBullet.velocity_y = 2.0;
    bullets.push_back(newBullet);
}

ShaderProgram Setup(){
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    
    ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    
    Matrix projectionMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;
    
    projectionMatrix.setOrthoProjection(-1.7777f, 1.77777f, -1.0f, 1.0f, -1.0f, 1.0f);
    
    glViewport(0, 0, 800, 600);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    initMenuScreen();
    initEntityArray();

    
    program.setModelMatrix(modelMatrix);
    program.setProjectionMatrix(projectionMatrix);
    program.setViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    return program;
}

void PlayerInput(bool& done){
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        
        else if (event.type == SDL_KEYDOWN){
            if (event.key.keysym.scancode == SDL_SCANCODE_LEFT){
                entities[40].velocity_x = -0.5;
            }
            
            else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT){
                entities[40].velocity_x = 0.5;
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_SPACE){
                if(state == STATE_GAME_LEVEL){
                        shootBullet();
                }
                else if (state == STATE_MAIN_MENU){
                    state = STATE_GAME_LEVEL;
                }
                else if (state == STATE_GAME_OVER){
                    state = STATE_GAME_LEVEL;
                    entities.clear();
                    initEntityArray();
                }

            }
        }
        else if (event.type == SDL_KEYUP){
            if (event.key.keysym.scancode == SDL_SCANCODE_LEFT){
                entities[40].velocity_x = 0;
            }
            
            else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT){
                entities[40].velocity_x = 0;
            }
            
        }
    }
}

//return true if the aliens should turn right
GLboolean rightTurn(){
    for(int i = 0; i < 10; i++){
        for(int j = i; j < 40; j+=10){
            if(entities[j].isAlive && entities[j].x < LWALL)//leftmost left of left wall
                return true;
        }
    }
    return false;
}

GLboolean leftTurn(){
    for(int i = 9; i > -1; i--){
        for(int j = i; j < 40; j+=10){
            if(entities[j].isAlive && entities[j].x > RWALL)//rightmost right of right wall
                return true;
        }
    }
    return false;
}

void Update(float elapsed){
    if (state == STATE_GAME_LEVEL){
        int sign = entities[0].step/0.1f;
        if (rightTurn())
            sign = 1;
        if (leftTurn())
            sign = -1;
        float newstep;
        for(int i=0; i < entities.size(); i++) {//loop through entities and update all
            newstep = sign * fabs(entities[i].step);
            if (newstep != entities[i].step){
                entities[i].step = newstep;
                entities[i].y = entities[i].y-0.1;
            }
            entities[i].Update(elapsed);
        }
        for(int i=0; i < bullets.size(); i++) {
            if (bullets[i].isAlive) {
                bullets[i].Update(elapsed);
                for (int j = 0; j < entities.size(); j++) {
                    if(entities[j].isAlive && bullets[i].collidesWith(entities[j])){//do collision stuffs
                        entities[j].isAlive = false;
                        bullets[i].isAlive = false;
                    }
                }
            }
        }
    }
    int alive = 0;
    for (int i = 0; i < entities.size(); i++) {
        if (entities[i].isAlive) {
            alive++;
        }
    }
    if (alive == 1) {
        state = STATE_GAME_OVER;
        puts("game over. press space to play again");
    }
}

void fixedTSUpdate(){
    //take care of ticks here
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    
    float fixedElapsed = elapsed + timeLeftOver;
    if(fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS){
        fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
    }
    while (fixedElapsed >= FIXED_TIMESTEP){
        fixedElapsed -= FIXED_TIMESTEP;
        Update(FIXED_TIMESTEP);
    }
    timeLeftOver = fixedElapsed;
}

void Render(ShaderProgram* program){
    glClear(GL_COLOR_BUFFER_BIT);
    if (state == STATE_GAME_LEVEL){
        for(int i=0; i < entities.size(); i++){
            entities[i].Render(program);
        }
        for(int i=0; i < bullets.size(); i++){
            if(bullets[i].isAlive){
                bullets[i].Render(program);
            }
        }
    }
    else if (state == STATE_MAIN_MENU){
        puts("Main menu. Press space to start");
        for(int i=0; i < bullets.size(); i++){
            text[i].Render(program);
        }
    }
    SDL_GL_SwapWindow(displayWindow);
}

int main(int argc, char *argv[]){
    ShaderProgram program = Setup();
    bool done = false;
    state = STATE_MAIN_MENU;//STATE_GAME_LEVEL;//TODO: switch back
    //initMenuScreen(&program);
    while (!done) {
        PlayerInput(done);
        fixedTSUpdate();
        Render(&program);
        glUseProgram(program.programID);
    }
    for (int i = 0; i < entities.size(); i++) {
        entities[i].~Entity();
    }
    SDL_Quit();
    return 0;
}