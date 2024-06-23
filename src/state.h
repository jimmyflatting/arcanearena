#pragma once

#include <raylib.h>
#include <raymath.h>

typedef enum GameScreen
{
    LOGO = 0,
    TITLE,
    GAMEPLAY,
    ENDING
} GameScreen;

typedef struct Map
{
    int width;
    int height;
    int *tiles;
} Map;

typedef struct State
{
    GameScreen currentScreen;
    int framesCounter;
    Map map;
} State;

#include "gfx/scene.h"