#include "state.h"
#include <stdio.h>

// Function prototypes
void TilemapEditorUI(void);

State state;

int main(void)
{
    // Initialization
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Arcane Arena");

    state.currentScreen = GAMEPLAY;
    state.framesCounter = 0;

    InitGame(&state);

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // check for fullscreen
        int display = GetCurrentMonitor();
        if (IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_ENTER))
        {
            printf("Alt + Enter\n");
            ToggleFullscreen();
        }
        // Update
        //----------------------------------------------------------------------------------
        UpdateScene(&state);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawScene(&state, screenWidth, screenHeight);

        EndDrawing();
        //----------------------------------------------------------------------------------

        state.framesCounter++;
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    CloseWindow();

    return 0;
}