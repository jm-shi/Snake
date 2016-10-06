//
//  menu.c
//  Snake
//
//  Created on 9/17/16.
//

#include "menu.h"

void drawScore(SDL_Renderer *renderer, GameState *game) {
    SDL_Surface *scoreSurface;
    SDL_Color white = {255,255,255};
    
    SDL_Rect rect;
    rect.x = 10;
    rect.y = 0;
    rect.w = SCREEN_WIDTH/8;
    rect.h = SCREEN_HEIGHT/8;
    
    char text[20];
    sprintf(text, "%03d", game->score);
    
    scoreSurface = TTF_RenderText_Solid(game->font, text, white);
    SDL_Texture *scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    SDL_RenderCopy(renderer, scoreTexture, NULL, &rect);
    SDL_DestroyTexture(scoreTexture);
    SDL_FreeSurface(scoreSurface);
}

void setAlpha(SDL_Texture *texture, Uint8 alpha) {
    SDL_SetTextureAlphaMod(texture, alpha);
}

void noWallsActivated(SDL_Renderer *renderer, GameState *game, int canGoThrough) {
    SDL_Rect rect;
    rect.x = 10;
    rect.y = SCREEN_HEIGHT/4+20;
    rect.w = SCREEN_WIDTH-20;
    rect.h = SCREEN_HEIGHT/3;
    
    SDL_Color white = {255, 255, 255};
    SDL_Surface *sWall;
    SDL_Surface *sNoWall;
    
    sWall = TTF_RenderText_Solid(game->font, "Walls activated", white);
    sNoWall = TTF_RenderText_Solid(game->font, "No walls", white);
    
    SDL_Texture *walls = SDL_CreateTextureFromSurface(renderer, sWall);
    SDL_Texture *noWalls = SDL_CreateTextureFromSurface(renderer, sNoWall);
    
    int a = 255;
    
    if (canGoThrough) {
        while (a > 0) {
            render(renderer, game);
            SDL_RenderCopy(renderer, noWalls, NULL, &rect);
            setAlpha(noWalls, a);
            a -= 10;
            SDL_RenderPresent(renderer);
        }
    }
    else {
        while (a > 0) {
            render(renderer, game);
            SDL_RenderCopy(renderer, walls, NULL, &rect);
            setAlpha(walls, a);
            a -= 10;
            SDL_RenderPresent(renderer);
        }
    }
    
    SDL_DestroyTexture(walls);
    SDL_DestroyTexture(noWalls);
    SDL_FreeSurface(sWall);
    SDL_FreeSurface(sNoWall);
}

void pauseGame(SDL_Renderer *renderer, GameState *game) {
    SDL_Rect rect;
    rect.x = 10;
    rect.y = SCREEN_HEIGHT/4+20;
    rect.w = SCREEN_WIDTH-20;
    rect.h = SCREEN_HEIGHT/3;
    
    SDL_Color white = {255, 255, 255};
    SDL_Surface *sPaused;
    
    sPaused = TTF_RenderText_Solid(game->font, "Paused. Press space to continue", white);
    
    SDL_Texture *paused = SDL_CreateTextureFromSurface(renderer, sPaused);
    SDL_RenderCopy(renderer, paused, NULL, &rect);
    SDL_DestroyTexture(paused);
    SDL_FreeSurface(sPaused);
}


void playAgainButtons(SDL_Renderer *renderer, GameState *game, int mouseOverYes, int mouseOverNo) {
    
    SDL_Rect gameOverRect;
    gameOverRect.x = SCREEN_WIDTH/4;
    gameOverRect.y = SCREEN_HEIGHT/4;
    gameOverRect.w = SCREEN_WIDTH/2;
    gameOverRect.h = SCREEN_HEIGHT/3;
    
    SDL_Rect playAgainRect;
    playAgainRect.x = SCREEN_WIDTH/9;
    playAgainRect.y = SCREEN_HEIGHT/2 + 15;
    playAgainRect.w = SCREEN_WIDTH/3;
    playAgainRect.h = SCREEN_HEIGHT/5;
    
    SDL_Rect yesRect;
    yesRect.x = SCREEN_WIDTH/9 + 275;
    yesRect.y = SCREEN_HEIGHT/2 + 15;
    yesRect.w = SCREEN_WIDTH/7;
    yesRect.h = SCREEN_HEIGHT/5;
    
    SDL_Rect noRect;
    noRect.x = SCREEN_WIDTH/9 + 400;
    noRect.y = SCREEN_HEIGHT/2 + 15;
    noRect.w = SCREEN_WIDTH/7;
    noRect.h = SCREEN_HEIGHT/5;
    
    SDL_Color cyan = {0, 255, 255};
    SDL_Color orange = {255, 128, 0};
    SDL_Color blue = {16, 104, 155};
    
    SDL_Surface *sGameOver;
    SDL_Surface *sPlayAgain;
    SDL_Surface *sYes;
    SDL_Surface *sNo;
    
    if (mouseOverYes) {
        sGameOver = TTF_RenderText_Solid(game->font, "GAME OVER", orange);
        sPlayAgain = TTF_RenderText_Solid(game->font, "Play again?", blue);
        sYes = TTF_RenderText_Solid(game->font, "Yes", cyan);
        sNo = TTF_RenderText_Solid(game->font, "No", blue);
    }
    else if (mouseOverNo) {
        sGameOver = TTF_RenderText_Solid(game->font, "GAME OVER", orange);
        sPlayAgain = TTF_RenderText_Solid(game->font, "Play again?", blue);
        sYes = TTF_RenderText_Solid(game->font, "Yes", blue);
        sNo = TTF_RenderText_Solid(game->font, "No", cyan);
    }
    else {
        sGameOver = TTF_RenderText_Solid(game->font, "GAME OVER", orange);
        sPlayAgain = TTF_RenderText_Solid(game->font, "Play again?", blue);
        sYes = TTF_RenderText_Solid(game->font, "Yes", blue);
        sNo = TTF_RenderText_Solid(game->font, "No", blue);
    }
    
    SDL_Texture *gameOver = SDL_CreateTextureFromSurface(renderer, sGameOver);
    SDL_Texture *playAgain = SDL_CreateTextureFromSurface(renderer, sPlayAgain);
    SDL_Texture *yes = SDL_CreateTextureFromSurface(renderer, sYes);
    SDL_Texture *no = SDL_CreateTextureFromSurface(renderer, sNo);
    
    SDL_RenderCopy(renderer, gameOver, NULL, &gameOverRect);
    SDL_RenderCopy(renderer, playAgain, NULL, &playAgainRect);
    SDL_RenderCopy(renderer, yes, NULL, &yesRect);
    SDL_RenderCopy(renderer, no, NULL, &noRect);
    
    SDL_DestroyTexture(gameOver);
    SDL_DestroyTexture(playAgain);
    SDL_DestroyTexture(yes);
    SDL_DestroyTexture(no);
    
    SDL_FreeSurface(sGameOver);
    SDL_FreeSurface(sPlayAgain);
    SDL_FreeSurface(sYes);
    SDL_FreeSurface(sNo);
}
