#include <stdbool.h>
#include "include/raylib.h"

#include "brickwall.h"

// MAP VARS
const int mapX = 20;
const int mapY = 12;
const int mapS = 64;

int map[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

Color generate_texture_at_point(int x, int y) {
    Color out;

    out.r = bricks[y * 64 + x][0];
    out.g = bricks[y * 64 + x][1];
    out.b = bricks[y * 64 + x][2];
    out.a = 255;

    // if ( x < 32) {
    //     out.r = 237;
    //     out.g = 28;
    //     out.b = 36;
    // } else {
    //     out.r = 0;
    //     out.g = 0;
    //     out.b = 0;
    // }


    return out;
} 

bool is_wall(int x, int y) {
    return map[y * mapX + x] == 1;
}

void draw_map2D() {
    for (int y = 0; y < mapY; y++) {
        for (int x = 0; x < mapX; x++) {

            if (is_wall(x, y)) {

                DrawRectangle(x * mapS, y * mapS, mapS, mapS, BLACK);   

            }
        }
    }
}
