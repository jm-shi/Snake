//
//  main.h
//  Snake
//
//  Created on 9/6/16.
//
//  Thanks to VertoStudio3D for providing the C tutorials and a basic SDL template. His videos can be found here: https://www.youtube.com/playlist?list=PLT6WFYYZE6uLMcPGS3qfpYm7T_gViYMMt

#ifndef main_h
#define main_h

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SNAKE_SIZE 20
#define FRUIT_SIZE 20

#define STOP 0
#define GO_LEFT 1
#define GO_UP 2
#define GO_RIGHT 3
#define GO_DOWN 4

#include <SDL2_image/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>

// Left = 0, up = 1, down = 2, right = 3
typedef enum {
    STATIONARY = STOP,
    LEFT = GO_LEFT,
    UP = GO_UP,
    RIGHT = GO_RIGHT,
    DOWN = GO_DOWN
} Direction;

typedef struct Snake {
    int x, y;
    int collisionX, collisionY;
    int isDead, direction;
    int length;
    struct Snake *head;
    struct Snake *previous;
    struct Snake *next;
    struct Snake *tail;
} Snake;

typedef struct {
    int x, y;
} Fruit;

typedef struct {
    // Snake
    Snake snake;
    
    // Fruit
    Fruit fruit;
    
    // Font
    TTF_Font *font;
    
    // Renderer
    SDL_Renderer *renderer;
    
    // Pause
    int paused;
    
    // Score
    int score;
    
    // Snake can go through walls
    int noWalls;
    
} GameState;

void render(SDL_Renderer *renderer, GameState *game);

#endif /* main_h */
