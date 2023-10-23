#ifndef MAP_H
#define MAP_H

#include "include/raylib.h"

extern const int mapX;
extern const int mapY;
extern const int mapS;

extern int map[];

Color generate_texture_at_point(int x, int y);
bool is_wall(int x, int y);
void draw_map2D();

#endif
