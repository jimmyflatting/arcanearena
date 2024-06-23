// tilemap.c

#include "tilemap.h"
#include <stdio.h>
#include <stdlib.h>

// Static variables to hold the tilemap and its associated texture loader
static tmx_map *map = NULL;

void *raylib_tex_loader(const char *path)
{
    Texture2D *texture = malloc(sizeof(Texture2D));
    if (!texture)
    {
        printf("Failed to allocate memory for texture\n");
        return NULL;
    }
    *texture = LoadTexture(path);
    if (texture->id == 0)
    {
        printf("Failed to load texture: %s\n", path);
        free(texture);
        return NULL;
    }
    return texture;
}

void raylib_free_tex(void *ptr)
{
    if (ptr)
    {
        UnloadTexture(*(Texture2D *)ptr);
        free(ptr);
    }
}

Color int_to_color(int color)
{
    tmx_col_bytes res = tmx_col_to_bytes(color);
    return *((Color *)&res);
}

void LoadTilemap(const char *filepath)
{
    tmx_img_load_func = raylib_tex_loader;
    tmx_img_free_func = raylib_free_tex;
    map = tmx_load(filepath);
    if (!map)
    {
        fprintf(stderr, "Error loading map: %s\n", tmx_strerr());
        EXIT_FAILURE; // Or handle the error appropriately
    }
}

void UnloadTilemap()
{
    if (map)
    {
        tmx_map_free(map);
        map = NULL;
    }
}

void RenderTilemap()
{
    if (!map)
    {
        return;
    }

    ClearBackground(int_to_color(map->backgroundcolor));
    draw_all_layers(map, map->ly_head);
}

void draw_all_layers(tmx_map *map, tmx_layer *layers)
{
    if (!layers)
    {
        return;
    }

    while (layers)
    {
        if (layers->visible)
        {
            if (layers->type == L_GROUP)
            {
                draw_all_layers(map, layers->content.group_head);
            }
            else if (layers->type == L_OBJGR)
            {
                draw_objects(layers->content.objgr);
            }
            else if (layers->type == L_IMAGE)
            {
                draw_image_layer(layers->content.image);
            }
            else if (layers->type == L_LAYER)
            {
                draw_layer(map, layers);
            }
        }
        layers = layers->next;
    }
}

void draw_image_layer(tmx_image *image)
{
    if (!image)
    {
        return;
    }

    Texture2D *texture = (Texture2D *)image->resource_image;
    if (texture)
    {
        DrawTexture(*texture, 0, 0, WHITE);
    }
}

#define MAX_COLLISION_BOXES 100

Rectangle collisionBoxes[MAX_COLLISION_BOXES];
int numCollisionBoxes = 0;

void add_collision_box(Rectangle box)
{
    if (numCollisionBoxes < MAX_COLLISION_BOXES)
    {
        collisionBoxes[numCollisionBoxes++] = box;
    }
    else
    {
        fprintf(stderr, "Error: Exceeded maximum collision boxes.\n");
    }
}

Rectangle *get_collision_boxes()
{
    return collisionBoxes;
}

int get_num_collision_boxes()
{
    return numCollisionBoxes;
}

void draw_layer(tmx_map *map, tmx_layer *layer)
{
    unsigned long i, j;
    unsigned int gid, x, y, w, h, flags;
    float op;
    tmx_tileset *ts;
    tmx_image *im;
    void *image;
    op = layer->opacity;

    for (i = 0; i < map->height; i++)
    {
        for (j = 0; j < map->width; j++)
        {
            gid = (layer->content.gids[(i * map->width) + j]) & TMX_FLIP_BITS_REMOVAL;
            if (map->tiles[gid] != NULL)
            {
                ts = map->tiles[gid]->tileset;
                im = map->tiles[gid]->image;
                x = map->tiles[gid]->ul_x;
                y = map->tiles[gid]->ul_y;
                w = ts->tile_width;
                h = ts->tile_height;

                if (im)
                {
                    image = im->resource_image;
                }
                else
                {
                    image = ts->image->resource_image;
                }

                flags = (layer->content.gids[(i * map->width) + j]) & ~TMX_FLIP_BITS_REMOVAL;
                if (strcmp(layer->name, "Foreground") == 0 && map->tiles[gid]->id != 0)
                {
                    // Draw tile
                    draw_tile(image, x, y, w, h, j * ts->tile_width, i * ts->tile_height, op, flags);

                    // Add collision box
                    Rectangle collisionBox = {j * ts->tile_width, i * ts->tile_height, w, h};
                    add_collision_box(collisionBox); // Function to add collision box
                }
                else
                {
                    // Draw tile without collision
                    draw_tile(image, x, y, w, h, j * ts->tile_width, i * ts->tile_height, op, flags);
                }
            }
        }
    }
}

void draw_tile(void *image, unsigned int sx, unsigned int sy, unsigned int sw, unsigned int sh,
               unsigned int dx, unsigned int dy, float opacity, unsigned int flags)
{
    if (!image)
    {
        return;
    }

    DrawTextureRec(*(Texture2D *)image, (Rectangle){sx, sy, sw, sh}, (Vector2){dx, dy}, (Color){255, 255, 255, (unsigned char)(opacity * 255)});
}

void draw_polyline(double offset_x, double offset_y, double **points, int points_count, Color color)
{
    for (int i = 1; i < points_count; i++)
    {
        DrawLineEx((Vector2){offset_x + points[i - 1][0], offset_y + points[i - 1][1]},
                   (Vector2){offset_x + points[i][0], offset_y + points[i][1]},
                   1.0f, color);
    }
}

void draw_polygon(double offset_x, double offset_y, double **points, int points_count, Color color)
{
    draw_polyline(offset_x, offset_y, points, points_count, color);
    if (points_count > 2)
    {
        DrawLineEx((Vector2){offset_x + points[0][0], offset_y + points[0][1]},
                   (Vector2){offset_x + points[points_count - 1][0], offset_y + points[points_count - 1][1]},
                   1.0f, color);
    }
}

void draw_objects(tmx_object_group *objgr)
{
    tmx_object *head = objgr->head;
    Color color = int_to_color(objgr->color);

    while (head)
    {
        if (head->visible)
        {
            if (head->obj_type == OT_SQUARE)
            {
                DrawRectangleLinesEx((Rectangle){head->x, head->y, head->width, head->height}, 1.0f, color);
            }
            else if (head->obj_type == OT_POLYGON)
            {
                draw_polygon(head->x, head->y, head->content.shape->points, head->content.shape->points_len, color);
            }
            else if (head->obj_type == OT_POLYLINE)
            {
                draw_polyline(head->x, head->y, head->content.shape->points, head->content.shape->points_len, color);
            }
            else if (head->obj_type == OT_ELLIPSE)
            {
                DrawEllipseLines(head->x + head->width / 2.0, head->y + head->height / 2.0, head->width / 2.0, head->height / 2.0, color);
            }
        }
        head = head->next;
    }
}
