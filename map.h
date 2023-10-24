#ifndef MAP_H
#define MAP_H

#include "include/raylib.h"

extern const int mapX;
extern const int mapY;
extern const int mapS;

extern int map[];

void load_test_texture(char *filepath);
void load_texture_from_file(char *filepath, int out_arr[4096][3]);
Color generate_texture_at_point(int x, int y, float color_mult);
bool is_wall(int x, int y);
void draw_map2D();

#endif
