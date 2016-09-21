//
//  snakeStatus.h
//  Snake
//
//  Created on 9/17/16.
//

#ifndef snakeStatus_h
#define snakeStatus_h

#include "main.h"

void ateFruit(SDL_Renderer *renderer, GameState *game);
void randomize(Fruit *fruit);

void checkIfDead(Snake *snake);
void elongateSnake(Snake *snake);
void moveSnake(Snake *snake);

void freeSnake(GameState *game);

#endif /* snakeStatus_h */
