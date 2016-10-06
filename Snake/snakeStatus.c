//
//  snakeStatus.c
//  Snake
//
//  Created on 9/17/16.
//

#include "snakeStatus.h"

// Randomize the fruit's x and y position
void randomize(Fruit *fruit) {
    fruit->x = rand() % (SCREEN_WIDTH-20) / 10 * 10;
    fruit->y = rand() % (SCREEN_HEIGHT-20) / 10 * 10;

    if (fruit->x % 20 != 0) {
        fruit->x += 10;
    }
    if (fruit->y % 20 != 0) {
        fruit->y += 10;
    }
}

// Make each trailing snake block move based on its direction
void moveSnakeBlocks(Snake *curNode) {
    while (curNode->previous != NULL) {
        curNode = curNode->previous;
        if(curNode->direction == LEFT) {
            if (curNode->next->x == SCREEN_WIDTH-SNAKE_SIZE) {
                curNode->x = 0;
                curNode->y = curNode->next->y;
            }
            else {
                curNode->x = curNode->next->x + 40;
                curNode->y = curNode->next->y;
                curNode->x -= 20; // Move left
            }
        }
        if(curNode->direction == UP) {
            if (curNode->next->y == SCREEN_HEIGHT-SNAKE_SIZE) {
                curNode->x = curNode->next->x;
                curNode->y = 0;
            }
            else {
                curNode->x = curNode->next->x;
                curNode->y = curNode->next->y;
                curNode->y += 20; // Move up
            }
        }
        if(curNode->direction == RIGHT) {
            if (curNode->next->x == 0) {
                curNode->x = SCREEN_WIDTH-SNAKE_SIZE;
                curNode->y = curNode->next->y;
            }
            else {
                curNode->x = curNode->next->x - 40;
                curNode->y = curNode->next->y;
                curNode->x += 20; // Move right
            }
        }
        if(curNode->direction == DOWN) {
            if (curNode->next->y == 0) {
                curNode->x = curNode->next->x;
                curNode->y = SCREEN_HEIGHT-SNAKE_SIZE;
            }
            else {
                curNode->x = curNode->next->x;
                curNode->y = curNode->next->y - 40;
                curNode->y += 20; // Move down
            }
        }
    }
}

// Add an additional body to the snake's tail
void elongateSnake(Snake *snake) {
    // Start at snake head
    Snake *curNode = snake->head;
    
    Snake *snakeTail = (Snake*)malloc(sizeof(Snake));
    snakeTail->head = snake->head;
    snakeTail->previous = NULL;
    snakeTail->isDead = 0;
    snakeTail->length = snake->head->length;

    for (int counter = 0; counter < snake->length-2; counter++) {
        curNode = curNode->previous;
    }

    // Add snakeTail to end of snake (make it the new last node)
    snakeTail->next = curNode;
    curNode->previous = snakeTail;
    snake->head->tail = snakeTail;
    snakeTail->direction = snakeTail->next->direction;

    curNode = snakeTail->next;
    // Update only the snake tail's x and y positions
    moveSnakeBlocks(curNode);
}

void ateFruit(SDL_Renderer *renderer, GameState *game) {
    Snake *snake = &game->snake;
    Fruit *fruit = &game->fruit;

    // If snake eats fruit
    if (snake->x == fruit->x && snake->y == fruit->y) {
        snake->head->length+=1;
        elongateSnake(snake);
        game->score+=1;

        randomize(fruit);

        // Fruit cannot spawn on spaces occupied by the snake
        Snake *curNode = snake;
        int uniqueSpace = 0;
        while (!uniqueSpace) {
            for (int index = 0; index < snake->length; index++) {
                if (curNode->x == fruit->x && curNode->y == fruit->y) {
                    randomize(fruit);
                    continue;
                }
                curNode = curNode->previous;
            }
            uniqueSpace = 1;
        }
    }
}

void checkIfDead(Snake *snake) {
    Snake *curNode = snake->head;
    for (int index = 0; index < snake->length-1; index++) {
        curNode = curNode->previous;
        if (snake->head->x == curNode->x && snake->head->y == curNode->y) {
            snake->collisionX = curNode->x;
            snake->collisionY = curNode->y;
            snake->isDead = 1;
            break;
        }
    }
}

void moveSnake(Snake *snake) {
    Snake *curNode = snake->head;

    if (curNode->isDead) {
        return;
    }

    // First move the head
    if (curNode->direction == LEFT) {
        curNode->x -= 20;
    }
    else if (curNode->direction == UP) {
        curNode->y -= 20;
    }
    else if (curNode->direction == RIGHT) {
        curNode->x += 20;
    }
    else if (curNode->direction == DOWN) {
        curNode->y += 20;
    }

    Snake *snakeTail = snake->tail;
    if (snake->length > 1) {
        Snake *curNode = snake->head;
        // Loop to back end of the snake
        for (int counter = 0; counter < snake->length-2; counter++) {
            curNode = curNode->previous;
        }
        // Make each snake block copy the direction directly in front of it, until you reach the head
        snakeTail->direction = snakeTail->next->direction;
        curNode = snakeTail->next;
        while (curNode->next != NULL) {
            curNode->direction = curNode->next->direction;
            curNode = curNode->next;
        }
    }
    moveSnakeBlocks(curNode);
}

void freeSnake(GameState *game) {
    Snake *curNode = game->snake.previous;
    while (curNode != NULL) {
        free(curNode);
        curNode = curNode->previous;
    }
}
