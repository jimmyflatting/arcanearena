// game.c

#include "game.h"

#define MAX_BULLETS 10

struct Player player = {10, 10, 32, 32, RED, 0, 3};
struct Bullet bullets[MAX_BULLETS];

extern State state;

void InitGame(State *state)
{
    // Load the tilemap
    LoadTilemap("assets/maps/dummy/dummy.tmx");

    // Initialize bullets
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i].active = 0;
        bullets[i].width = 10;
        bullets[i].height = 5;
        bullets[i].speed = 10;
        bullets[i].color = BLACK;
    }
}

void UpdateBullets()
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].active)
        {
            switch (bullets[i].direction)
            {
            case 0:
                bullets[i].y -= bullets[i].speed;
                break; // Up
            case 1:
                bullets[i].y += bullets[i].speed;
                break; // Down
            case 2:
                bullets[i].x -= bullets[i].speed;
                break; // Left
            case 3:
                bullets[i].x += bullets[i].speed;
                break; // Right
            }

            // Deactivate the bullet if it goes off screen
            if (bullets[i].x > GetScreenWidth() || bullets[i].x < 0 ||
                bullets[i].y > GetScreenHeight() || bullets[i].y < 0)
            {
                bullets[i].active = 0;
            }
        }
    }
}

void DrawBullets()
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].active)
        {
            DrawRectangle(bullets[i].x, bullets[i].y, bullets[i].width, bullets[i].height, bullets[i].color);
        }
    }
}

void ShootBullet()
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!bullets[i].active)
        {
            bullets[i].direction = player.direction;
            bullets[i].active = 1;

            switch (player.direction)
            {
            case 0: // Up
                bullets[i].x = player.x + player.width / 2 - bullets[i].width / 2;
                bullets[i].y = player.y;
                break;
            case 1: // Down
                bullets[i].x = player.x + player.width / 2 - bullets[i].width / 2;
                bullets[i].y = player.y + player.height;
                break;
            case 2: // Left
                bullets[i].x = player.x;
                bullets[i].y = player.y + player.height / 2 - bullets[i].height / 2;
                break;
            case 3: // Right
                bullets[i].x = player.x + player.width;
                bullets[i].y = player.y + player.height / 2 - bullets[i].height / 2;
                break;
            }
            break;
        }
    }
}

void PlayerCollisionWall()
{
    if (((player.x + player.width) >= GetScreenWidth()) || (player.x <= 0))
    {
        player.x = (player.x + player.width) >= GetScreenWidth() ? GetScreenWidth() - player.width : 0;
    }

    if (((player.y + player.height) >= GetScreenHeight()) || (player.y <= 0))
    {
        player.y = (player.y + player.height) >= GetScreenHeight() ? GetScreenHeight() - player.height : 0;
    }
}

void GameUpdate()
{
    // Move player box
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
    {
        // Check collision with foreground tiles
        if (!check_collision(player.x, player.y - 5, player.width, player.height))
        {
            player.y -= 5;
        }
        player.direction = 0;
    }
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    {
        // Check collision with foreground tiles
        if (!check_collision(player.x, player.y + 5, player.width, player.height))
        {
            player.y += 5;
        }
        player.direction = 1;
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    {
        // Check collision with foreground tiles
        if (!check_collision(player.x - 5, player.y, player.width, player.height))
        {
            player.x -= 5;
        }
        player.direction = 2;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    {
        // Check collision with foreground tiles
        if (!check_collision(player.x + 5, player.y, player.width, player.height))
        {
            player.x += 5;
        }
        player.direction = 3;
    }
    if (IsKeyPressed(KEY_SPACE))
    {
        ShootBullet();
        // play sound (implementation not shown)
    }

    // Player collision with wall
    PlayerCollisionWall();

    // Update bullets
    UpdateBullets();

    // Check for collision with bullets and enemies (implementation not shown)
}

int check_collision(int x, int y, int width, int height)
{
    Rectangle playerRect = {x, y, width, height};
    for (int i = 0; i < get_num_collision_boxes(); i++)
    {
        if (CheckCollisionRecs(playerRect, get_collision_boxes()[i]))
        {
            return 1;
        }
    }
    return 0;
}

void GridDraw()
{
    for (int i = 0; i < GetScreenWidth(); i += 32)
    {
        DrawLine(i, 0, i, GetScreenHeight(), LIGHTGRAY);
    }

    for (int i = 0; i < GetScreenHeight(); i += 32)
    {
        DrawLine(0, i, GetScreenWidth(), i, LIGHTGRAY);
    }
}

void GameDraw()
{
    // Draw map
    RenderTilemap();

    // Draw player box
    DrawRectangle(player.x, player.y, player.width, player.height, player.color);

    // Draw grid
    GridDraw();

    // Draw bullets
    DrawBullets();

    // Draw enemies (implementation not shown)
    // Draw score (implementation not shown)
    // Draw health (implementation not shown)
    // Draw game over (implementation not shown)
}

void UnloadGame()
{
    // Unload the tilemap
    UnloadTilemap();
}
