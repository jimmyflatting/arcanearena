#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include <stdio.h>
#include "state.h"
#include "world/tilemap.h"

typedef struct Player
{
    int x;
    int y;
    int width;
    int height;
    Color color;
    int isShooting;
    int direction; // 0: Up, 1: Down, 2: Left, 3: Right
} Player;

typedef struct Bullet
{
    int x;
    int y;
    int width;
    int height;
    int speed;
    int active;
    int direction; // 0: Up, 1: Down, 2: Left, 3: Right
    Color color;
} Bullet;

void InitGame(State *state);
void GameUpdate(void);
void GameDraw(void);
int check_collision(int x, int y, int width, int height);

#endif // GAME_H
