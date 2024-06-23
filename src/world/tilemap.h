// tilemap.h

#ifndef TILEMAP_H
#define TILEMAP_H

#include <raylib.h>
#include <tmx.h>
#include <string.h>

// Function declarations
int get_num_collision_boxes();
Rectangle *get_collision_boxes();

// Function declarations
void LoadTilemap(const char *filepath);
void UnloadTilemap();
void RenderTilemap();
void draw_all_layers(tmx_map *map, tmx_layer *layers);
void draw_objects(tmx_object_group *objgr);
void draw_image_layer(tmx_image *image);
void draw_layer(tmx_map *map, tmx_layer *layer);
void draw_tile(void *image, unsigned int sx, unsigned int sy, unsigned int sw, unsigned int sh,
               unsigned int dx, unsigned int dy, float opacity, unsigned int flags);
void add_collision_box(Rectangle box);

#endif // TILEMAP_H
