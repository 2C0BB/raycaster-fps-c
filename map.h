#ifndef MAP_H
#define MAP_H

extern const int mapX;
extern const int mapY;
extern const int mapS;

extern int map[];

bool is_wall(int x, int y);
void draw_map2D();

#endif
