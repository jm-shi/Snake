//
//  main.c
//  Snake
//
//  Created on 9/6/16.
//
//  Thanks to VertoStudio3D for providing the C tutorials and a basic SDL template. His videos can be found here: https://www.youtube.com/playlist?list=PLT6WFYYZE6uLMcPGS3qfpYm7T_gViYMMt

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include "main.h"
#include "snakeStatus.h"
#include "menu.h"

void loadGame(GameState *game) {
    srand((int)time(NULL));

    // Snake starts at center, is alive, and moves in random direction
    game->snake.x = SCREEN_WIDTH/2;
    game->snake.y = SCREEN_HEIGHT/2;
    game->snake.collisionX = -1;
    game->snake.collisionY = -1;
    game->snake.isDead = 0;
    game->snake.direction = rand()%4;
    game->snake.length = 1;
    game->snake.head = &game->snake;
    game->snake.previous = NULL;
    game->snake.next = NULL;
    game->snake.tail = &game->snake;

    game->fruit.x = rand() % (SCREEN_WIDTH-20) / 10 * 10;
    game->fruit.y = rand() % (SCREEN_HEIGHT-20) / 10 * 10;
    // The fruit's coordinate must divide evenly with 20
    if (game->fruit.x % 20 != 0) {
        game->fruit.x += 10;
    }
    if (game->fruit.y % 20 != 0) {
        game->fruit.y += 10;
    }

    game->font = TTF_OpenFont("RobotoBlack.ttf", 50);
    game->paused = 0;
    game->score = 0;
    game->noWalls = 1; // By default, snake can go through walls
}

void process(SDL_Renderer *renderer, GameState *game) {
    if(!game->snake.isDead) {
        Snake *snake = &game->snake;

        ateFruit(renderer, game);

        // If snake goes beyond the right end of screen, go to left end of screen
        if (snake->head->x >= SCREEN_WIDTH-SNAKE_SIZE && snake->head->direction == GO_RIGHT) {
            if (game->noWalls) {
                snake->head->x = -SNAKE_SIZE;
            }
            else {
                snake->isDead = 1;
                snake->collisionX = snake->head->x;
                snake->collisionY = snake->head->y;
            }
        }
        // If snake goes beyond left end of screen, go to right end of screen
        else if (snake->head->x <= 0 && snake->head->direction == GO_LEFT) {
            if (game->noWalls) {
                snake->head->x = SCREEN_WIDTH;
            }
            else {
                snake->isDead = 1;
                snake->collisionX = snake->head->x;
                snake->collisionY = snake->head->y;
            }
        }
        // If snake goes beyond top of screen, go to bottom of screen
        else if (snake->head->y <= 0 && snake->head->direction == GO_UP) {
            if (game->noWalls) {
                snake->head->y = SCREEN_HEIGHT;
            }
            else {
                snake->isDead = 1;
                snake->collisionX = snake->head->x;
                snake->collisionY = snake->head->y;
            }
        }
        // If snake goes beyond bottom of screen, go to top of screen
        else if (snake->head->y + SNAKE_SIZE >= SCREEN_HEIGHT && snake->head->direction == GO_DOWN) {
            if (game->noWalls) {
                snake->head->y = -SNAKE_SIZE;
            }
            else {
                snake->isDead = 1;
                snake->collisionX = snake->head->x;
                snake->collisionY = snake->head->y;
            }
        }

        moveSnake(snake);
    }
}

int playAgain(SDL_Renderer *renderer, SDL_Window *window, GameState *game) {
    SDL_Event event;
    int running = 1;

    while (SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_WINDOWEVENT_CLOSE: {
                if(window) {
                    SDL_DestroyWindow(window);
                    window = NULL;
                    running = 0;
                }
            break;
            }
            case SDL_MOUSEMOTION: {
                int x = event.motion.x;
                int y = event.motion.y;
                if (game->snake.isDead) {
                    // Hovering over "yes" text
                    if (x >= 340 && x <= 435 && y >= 280 && y <= 340) {
                        render(renderer, game);
                        playAgainButtons(renderer, game, 1, 0);
                        SDL_RenderPresent(renderer);
                    }
                    // Hovering over "no" text
                    else if (x >= 480 && x <= 560 && y >= 280 && y <= 340) {
                        render(renderer, game);
                        playAgainButtons(renderer, game, 0, 1);
                        SDL_RenderPresent(renderer);
                    }
                    else {
                        render(renderer, game);
                        playAgainButtons(renderer, game, 0, 0);
                        SDL_RenderPresent(renderer);
                    }
                }
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                int x = event.button.x;
                int y = event.button.y;
                // Clicking "yes" text
                if (x >= 340 && x <= 435 && y >= 280 && y <= 340) {
                    TTF_CloseFont(game->font);
                    freeSnake(game);
                    loadGame(game);
                    running = 0;
                }
                // Clicking "no" text
                else if (x >= 480 && x <= 560 && y >= 280 && y <= 340) {
                    running = 0;
                }
                break;
            }
            case SDL_QUIT:
                running = 0;
                break;
        }
    }

    return running;
}

int processEvents(SDL_Renderer *renderer, SDL_Window *window, GameState *game) {

    if (game->snake.isDead)
        return 0;

    SDL_Event event;
    int running = 1;

    while (SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_WINDOWEVENT_CLOSE:
                if(window) {
                    SDL_DestroyWindow(window);
                    window = NULL;
                    running = 0;
                }
            break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        running = 0;
                        break;
                    case SDLK_SPACE:
                        if (game->paused)
                            game->paused = 0;
                        else
                            game->paused = 1;
                        break;
                    case SDLK_w:
                        if (game->noWalls) {
                            game->noWalls = 0;
                            noWallsActivated(renderer, game, 0);
                        }
                        else {
                            game->noWalls = 1;
                            noWallsActivated(renderer, game, 1);
                        }
                        break;
                }
            break;
            case SDL_QUIT:
                running = 0;
                break;
        }
    }

    if (!game->paused) {
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        if(state[SDL_SCANCODE_LEFT]) {
            if (game->snake.head->direction != GO_RIGHT) {
                game->snake.head->direction = GO_LEFT;
            }
            else if (game->snake.length == 1) {
                game->snake.head->direction = GO_LEFT;
            }
        }
        else if(state[SDL_SCANCODE_UP]) {
            if (game->snake.head->direction != GO_DOWN) {
                game->snake.head->direction = GO_UP;
            }
            else if (game->snake.length == 1) {
                game->snake.head->direction = GO_UP;
            }
        }
        else if(state[SDL_SCANCODE_RIGHT]) {
            if (game->snake.head->direction != GO_LEFT) {
                game->snake.head->direction = GO_RIGHT;
            }
            else if (game->snake.length == 1) {
                game->snake.head->direction = GO_RIGHT;
            }
        }
        else if(state[SDL_SCANCODE_DOWN]) {
            if (game->snake.head->direction != GO_UP) {
                game->snake.head->direction = GO_DOWN;
            }
            else if (game->snake.length == 1) {
                game->snake.head->direction = GO_DOWN;
            }
        }
    }
    else {
        render(renderer, game);
        pauseGame(renderer, game);
        SDL_RenderPresent(renderer);
    }

    return running;
}

void render(SDL_Renderer *renderer, GameState *game)
{
    // Set the drawing color to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderClear(renderer);

    // Set the drawing color to green
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    // Draw a rectangle at snake's head position
    SDL_Rect rect = {
        game->snake.x, // x-coordinate
        game->snake.y, // y-coordinate
        SNAKE_SIZE, // Width
        SNAKE_SIZE  // Height
    };
    SDL_RenderFillRect(renderer, &rect);

    // Set the drawing color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Give snake a white outline
    SDL_RenderDrawRect(renderer, &rect);

    int snakeNum = 1;
    Snake *curNode = game->snake.head;
    while (snakeNum++ < game->snake.length) {
        // Set the drawing color to green
        SDL_SetRenderDrawColor(renderer, 25, 156, 11, 255);

        // Draw a rectangle at snake's position
        SDL_Rect rect2 = {
            curNode->previous->x, // x-coordinate
            curNode->previous->y, // y-coordinate
            SNAKE_SIZE,           // Width
            SNAKE_SIZE            // Height
        };
        SDL_RenderFillRect(renderer, &rect2);

        // Set the drawing color to white
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        // Give snake a white outline
        SDL_RenderDrawRect(renderer, &rect2);

        curNode = curNode->previous;
    }

    // Set the drawing color to red
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    // Draw a rectangle at fruit's position
    SDL_Rect fruitRect = {
        game->fruit.x,
        game->fruit.y,
        FRUIT_SIZE,
        FRUIT_SIZE
    };
    SDL_RenderFillRect(renderer, &fruitRect);

    // Set the drawing color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // Give fruit a white outline
    SDL_RenderDrawRect(renderer, &fruitRect);

    checkIfDead(&game->snake);

    // Highlight the square that the snake head collided with
    if(game->snake.isDead) {
        SDL_SetRenderDrawColor(renderer, 144, 195, 212, 255);
        SDL_Rect collideRect = {
            game->snake.collisionX,
            game->snake.collisionY,
            SNAKE_SIZE,
            SNAKE_SIZE
        };
        SDL_RenderFillRect(renderer, &collideRect);

        playAgainButtons(renderer, game, 0, 0);
    }

    drawScore(renderer, game);
}

int main(int argc, char * argv[]) {

    GameState gameState;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    srand((int)time(NULL));

    window = SDL_CreateWindow("Snake",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH,
                              SCREEN_HEIGHT,
                              SDL_WINDOW_OPENGL);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    gameState.renderer = renderer;
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    loadGame(&gameState);

    int running = 1;
    while (running) {
        running = processEvents(renderer, window, &gameState);

        if (!gameState.paused) {
            process(renderer, &gameState);

            render(renderer, &gameState);
            SDL_RenderPresent(renderer);

            SDL_Delay(50);

            if (gameState.snake.isDead) {
                gameState.snake.direction = STATIONARY;
                while (playAgain(renderer, window, &gameState));
            }
        }
    }

    TTF_CloseFont(gameState.font);
    freeSnake(&gameState);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
