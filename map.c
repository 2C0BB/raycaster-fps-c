#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/raylib.h"

#include "brickwall.h"
#include "hedge.h"
#include "map.h"


int test_texture[4096][3];


// MAP VARS
const int mapX = 20;
const int mapY = 12;
const int mapS = 64;

int map[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

void load_test_texture(char *filepath) {
    load_texture_from_file(filepath, test_texture);
}

void load_texture_from_file(char *filepath, int out_arr[4096][3]) {
    
    char buffer[100];

    FILE *fp;
    errno_t errors = fopen_s(&fp, filepath, "r");

    if (errors != 0) {
        printf("failed to read file\n");
        exit(1);
    }
    
    int i = 0;
    while (fgets(buffer, 100, fp) && i < 4096) {
        char *pos;

        if ((pos = strchr(buffer, '\n')) != NULL) {
            *pos = '\0';
        } else {
            printf("line %d in file is too long\n", i+1);
            exit(1);
        }

        char *token = strtok(buffer, " ");

        int rgb_idx = 0;
        while (token != NULL) {
            
            out_arr[i][rgb_idx] = atoi(token);

            token = strtok(NULL, " ");
            rgb_idx++;
        }
        
        i++;
    }
}

Color generate_texture_at_point(int x, int y, float color_mult) {
    Color out;


    int pix_off = (y * 64 + x);

    out.r = test_texture[pix_off][0] * color_mult;
    out.g = test_texture[pix_off][1] * color_mult;
    out.b = test_texture[pix_off][2] * color_mult;


    // out.r = hedge_text[pix_off + 0];
    // out.g = hedge_text[pix_off + 1];
    // out.b = hedge_text[pix_off + 2];

    // out.r = bricks[y * 64 + x][0] * color_mult;
    // out.g = bricks[y * 64 + x][1] * color_mult;
    // out.b = bricks[y * 64 + x][2] * color_mult;
    // out.a = 255;

    // if ( x < 32) {
    //     out.r = 237;
    //     out.g = 28;
    //     out.b = 36;
    // } else {
    //     out.r = 0;
    //     out.g = 0;
    //     out.b = 0;
    // }


    out.a = 255;
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
