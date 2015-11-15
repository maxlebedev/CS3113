#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "vector"
#include "Entity.hpp"
#include "SheetSprite.hpp"

#include <SDL_mixer.h>

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

//enum {STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER};
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


SDL_Window* displayWindow;

Matrix projectionMatrix;
Matrix modelMatrix;
Matrix viewMatrix;

Mix_Chunk *pewpew;
Mix_Music *music;

#define LEVEL_HEIGHT 16
#define LEVEL_WIDTH 22
#define TILE_SIZE 0.2f

unsigned int levelData[LEVEL_HEIGHT][LEVEL_WIDTH] =
{
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0},
    {0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0},
    {0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0},
    {0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0},
    {0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0},
    {0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0},
    {0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0},
    {0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0},
    {0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0},
    {0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0},
    {0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0},
    {0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0},
    {0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};


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

void procedurallyGenerate(){
    for(int y = LEVEL_HEIGHT-2; y > 2; y--){
        for(int x = 2; x < LEVEL_WIDTH-2; x++){
            //if the tile 2 below you and 1 over is 1, 66% chance to be 1
            if (levelData[y+2][x+1] || levelData[y+2][x-1]) {
                if (rand() % 3) {
                    levelData[y][x] = 1;
                }
            }
        }
    }
    for (int i = 0; i < LEVEL_HEIGHT; ++i){
        for (int j = 0; j < LEVEL_WIDTH; ++j){
            std::cout << levelData[i][j] << ' ';
        }
        std::cout << std::endl;
    }

}

void initEntityArray(){
    GLuint spriteSheetTexture = LoadTexture(RESOURCE_FOLDER"tiles_spritesheet.png");
    
    for (float i = 0; i < 4; i ++){
        for(float j = 0.0f; j < 10; j++){
            Entity myEntity = *new Entity(-1.35+((3*j)/10.0f),0.8-(i/8.0f),0.1,0.1) ;
            myEntity.sprite = SheetSprite(spriteSheetTexture, 843.0f/1024.0f, 116.0f/1024.0f, 13.0f/1024.0f, 57.0f/1024.0f, 0.2);
            myEntity.type = PLATFORM;
            myEntity.isStatic = true;
            entities.push_back(myEntity);
        }
    }
    
    Entity player = *new Entity((LEVEL_WIDTH*TILE_SIZE)/2,-0.5,0.23,0.23);
    player.sprite = SheetSprite(spriteSheetTexture, 792.0f/914.0f, 828.0f/936.0f, 48.0f/914.0f, 106.0f/936.0f, 0.2);
    player.type = PLAYER;
    entities.push_back(player);

    for(int y=LEVEL_HEIGHT; y > 0 ; y--) {
        for(int x=0; x < LEVEL_WIDTH; x++) {
            if(levelData[y][x]) {
                //(LEVEL_WIDTH*TILE_SIZE)
                Entity myEntity = *new Entity((x)/(LEVEL_WIDTH*TILE_SIZE),-(y)/(LEVEL_HEIGHT*TILE_SIZE),TILE_SIZE,TILE_SIZE) ;
                myEntity.sprite = SheetSprite(spriteSheetTexture, 504.0f/914.0f, 288.0f/936.0f, 70.0f/914.0f, 70.0f/936.0f, 0.2);
                myEntity.type = PLATFORM;
                myEntity.isStatic = true;
                entities.push_back(myEntity);
            }
        }
    }
}

void shootBullet() {
    GLuint sSTex = LoadTexture(RESOURCE_FOLDER"tiles_spritesheet.png");//this is global
    Entity newBullet = *new Entity(entities[40].x+entities[40].width/2.0f,entities[40].y+0.2,0.01,0.1);
    newBullet.sprite = SheetSprite(sSTex, 843.0f/1024.0f, 116.0f/1024.0f, 13.0f/1024.0f, 57.0f/1024.0f, 0.2);
    newBullet.type = BULLET;
    newBullet.velocity_y = 2.0;
    bullets.push_back(newBullet);
}

ShaderProgram Setup(){
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Platforms", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    
    ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    
    projectionMatrix.setOrthoProjection(-1.7777f, 1.77777f, -1.0f, 1.0f, -1.0f, 1.0f);
    
    glViewport(0, 0, 800, 600);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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
                entities[40].acceleration_x = -1.0;
            }
            
            else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT){
                entities[40].acceleration_x = 1.0;
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_SPACE){
                shootBullet();
                Mix_PlayChannel( -1, pewpew, 0);
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_UP){
                entities[40].velocity_y += 3.0f;
            }
        }
        else if (event.type == SDL_KEYUP){
            if (event.key.keysym.scancode == SDL_SCANCODE_LEFT){
                entities[40].acceleration_x = 0;
            }
            
            else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT){
                entities[40].acceleration_x = 0;
            }
            
        }

    }
}

void Update(float elapsed, ShaderProgram* program){
        for(int i=0; i < entities.size(); i++) {//loop through entities and update all
            entities[i].Update(elapsed);
        }
    
        for(int i=0; i < entities.size(); i++) {
            if (entities[i].isAlive && !entities[i].isStatic) {
                for(int j=0; j < entities.size(); j++){
                    if(i!=j && entities[j].isAlive && entities[i].collidesWith(entities[j])){
                        entities[i].uncollide(entities[j]);
                    }
                }
            }
        }
        //bullet stuff
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
    viewMatrix.identity();
    viewMatrix.Translate(-(entities[40].x), -(entities[40].y), 0);//(entities[40].y)
    program->setViewMatrix(viewMatrix);
}

void fixedTSUpdate(ShaderProgram* program){
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
        Update(FIXED_TIMESTEP, program);
    }
    timeLeftOver = fixedElapsed;
}

void Render(ShaderProgram* program){
    glClear(GL_COLOR_BUFFER_BIT);

        for(int i=0; i < entities.size(); i++){
            entities[i].Render(program);
        }
        for(int i=0; i < bullets.size(); i++){
            if(bullets[i].isAlive){
                bullets[i].Render(program);
            }
        }
    SDL_GL_SwapWindow(displayWindow);
}

void setupSound(){
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 );
    pewpew = Mix_LoadWAV(RESOURCE_FOLDER"Laser.wav");
    
    music = Mix_LoadMUS( "Clockwork_Grey_Interlude.mp3");
    Mix_PlayMusic(music, -1);
}

int main(int argc, char *argv[]){
    puts("start");
    procedurallyGenerate();
    ShaderProgram program = Setup();
    setupSound();
    
    bool done = false;
    while (!done) {
        PlayerInput(done);
        fixedTSUpdate(&program);
        Render(&program);
        glUseProgram(program.programID);
    }
    for (int i = 0; i < entities.size(); i++) {
        entities[i].~Entity();
    }
    //Mix_FreeChunk(someSound);
    //Mix_FreeMusic(music);
    SDL_Quit();
    return 0;
}