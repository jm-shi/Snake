//
//  menu.h
//  Snake
//
//  Created on 9/17/16.
//

#ifndef menu_h
#define menu_h

#include "main.h"

void drawScore(SDL_Renderer *renderer, GameState *game);

void setAlpha(SDL_Texture *texture, Uint8 alpha);
void noWallsActivated(SDL_Renderer *renderer, GameState *game, int canGoThrough);

void pauseGame(SDL_Renderer *renderer, GameState *game);

void playAgainButtons(SDL_Renderer *renderer, GameState *game, int mouseOverYes, int mouseOverNo);

#endif /* menu_h */
