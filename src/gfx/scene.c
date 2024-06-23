#include "scene.h"
#include "state.h"

extern struct Player player; // Declare external variables
extern struct Bullet bullets[];

void UpdateScene(State *state)
{
    switch (state->currentScreen)
    {
    case LOGO:
        // Update logo screen (implementation not shown)
        if (state->framesCounter > 180)
        {
            state->currentScreen = GAMEPLAY;
        }
        break;
    case GAMEPLAY:
        GameUpdate();
        break;
    case ENDING:
        // Update ending screen (implementation not shown)
        break;
    default:
        break;
    }
}

void DrawScene(State *state, int screenWidth, int screenHeight)
{
    switch (state->currentScreen)
    {
    case LOGO:
        // Draw logo screen (implementation not shown)
        DrawText("LOGO SCREEN", screenWidth / 2 - MeasureText("LOGO SCREEN", 20) / 2, screenHeight / 2 - 10, 20, LIGHTGRAY);
        break;
    case GAMEPLAY:
        GameDraw();
        break;
    case ENDING:
        // Draw ending screen (implementation not shown)
        DrawText("ENDING SCREEN", screenWidth / 2 - MeasureText("ENDING SCREEN", 20) / 2, screenHeight / 2 - 10, 20, LIGHTGRAY);
        break;
    default:
        break;
    }
}
