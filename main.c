#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "include/raylib.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 768

#define P2 PI/2
#define P3 3*PI/2

// PLAYER VARS
const int max_dof = 20;
const int player_vel = 5; 

float player_x = 300;
float player_y = 300;
float player_dx;
float player_dy;
float player_angle = 1;

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

bool is_wall(int x, int y) {
    return map[y * mapX + x] == 1;
}

float dist(float ax, float ay, float bx, float by, float ang) {
    return ( sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

float clamp_angle(float angle) {
    if (angle < 0) {
        angle += 2*PI;
    }

    if (angle >= 2*PI) {
        angle -= 2*PI;
    }

    return angle;
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

void draw_player2D() {
    // DrawCircle(player_x, player_y, 8, YELLOW);
    DrawRectangle(player_x, player_y, 8, 8, RED);
    DrawLine(player_x, player_y, player_x + player_dx*5, player_y + player_dy*5, BLUE);
}

void handle_input() {
    if (IsKeyDown(KEY_A)) {
        player_angle -= 0.1;
    }
    if (IsKeyDown(KEY_D)) {
        player_angle += 0.1;
    }

    player_angle = clamp_angle(player_angle);

    player_dx = cos(player_angle) * player_vel;
    player_dy = sin(player_angle) * player_vel;

    if (IsKeyDown(KEY_W)) {
        player_x += player_dx;
        player_y += player_dy;
    }

    if (IsKeyDown(KEY_S)) {
        player_x -= player_dx;
        player_y -= player_dy;
    }
}

void cast_rays3D() {
    int dof, mx, my, map_pos;
    float ray_x = player_x; 
    float ray_y = player_y; 
    float x_offset = 0; 
    float y_offset = 0;
    float final_distance;

    int fov = 80;

    float ray_angle = player_angle - ((fov/2.0) * (PI/180));

    for (int r = 0; r < fov; r++) {

        ray_angle = clamp_angle(ray_angle);

        
        // HORIZONTAL CHECKS
        dof = 0;
        float dist_horiz = 1000000;
        float hx = player_x;
        float hy = player_y;

        float aTan = -1 / tan(ray_angle);

        if ( ray_angle > PI ) { // looking up
            ray_y = ((int)(player_y / 64))*64 - 0.001; // .0001 for accuracy
            ray_x = (player_y - ray_y) * aTan + player_x;

            y_offset = -64;
            x_offset = -y_offset * aTan;
        }

        if ( ray_angle < PI ) { // looking down
            ray_y = ((int)(player_y / 64))*64 + 64;
            ray_x = (player_y - ray_y) * aTan + player_x;

            y_offset = 64;
            x_offset = -y_offset * aTan;
        }

        if (ray_angle == 0 || ray_angle == PI || ray_angle == 2*PI) { // looking directly left or right
            ray_x = player_x;
            ray_y = player_y;
            dof = max_dof;
        }

        while (dof < max_dof) {
            mx = ray_x / 64;
            my = ray_y / 64;
            map_pos = my * mapX + mx;

            if (map_pos < mapX * mapY && map_pos > 0 && map[map_pos] == 1) { // hit a wall

                hx = ray_x;
                hy = ray_y;
                dist_horiz = dist(player_x, player_y, hx, hy, ray_angle);
                dof = max_dof;
            } else {
                ray_x += x_offset;
                ray_y += y_offset;
                dof++;
            }
        }
        // ----------------------------------------------------------------------------


        // VERTICAL CHECKS
        dof = 0;
        float dist_vert = 1000000;
        float vx = player_x;
        float vy = player_y;
        float nTan = -tan(ray_angle);

        if ( ray_angle > P2 && ray_angle < P3 ) { // looking up
            ray_x = ((int)(player_x / 64))*64 - 0.001; // .0001 for accuracy
            ray_y = (player_x - ray_x) * nTan + player_y;

            x_offset = -64;
            y_offset = -x_offset * nTan;
        }

        if ( ray_angle < P2 || ray_angle > P3 ) { // looking left
            ray_x = ((int)(player_x / 64))*64 + 64;
            ray_y = (player_x - ray_x) * nTan + player_y;

            x_offset = 64;
            y_offset = -x_offset * nTan;
        }

        if (ray_angle == 0 || ray_angle == PI || ray_angle == 2*PI) { // looking directly left or right
            ray_x = player_x;
            ray_y = player_y;
            dof = max_dof;
        }

        while (dof < max_dof) {
            mx = ray_x / 64;
            my = ray_y / 64;
            map_pos = my * mapX + mx;

            if (map_pos < mapX * mapY && map_pos > 0 && map[map_pos] == 1) { // hit a wall
                vx = ray_x;
                vy = ray_y;
                dist_vert = dist(player_x, player_y, vx, vy, ray_angle);
                dof = max_dof;
            } else {
                ray_x += x_offset;
                ray_y += y_offset;
                dof++;
            }
        }

        Color wall_color;

        if (dist_vert < dist_horiz) {
            ray_x = vx;
            ray_y = vy;
            final_distance = dist_vert;
            wall_color = RED;
        } else {
            ray_x = hx;
            ray_y = hy;
            final_distance = dist_horiz;
            wall_color = ORANGE;
        }
        
        // 2D RAYS
        // DrawLine(player_x, player_y, ray_x, ray_y, BLUE);

        // 3D RAYS
        float ca = clamp_angle(player_angle - ray_angle);


        final_distance *= cos(ca); // using cosine to remove fisheye effect. This uses flat distance instead of direct.

        float line_height = mapS * SCREEN_HEIGHT / final_distance;
        if (line_height > SCREEN_HEIGHT) {
            line_height = SCREEN_HEIGHT;
        }
        float line_offset = (SCREEN_HEIGHT/2.0) - line_height/2.0;
        int line_width = SCREEN_WIDTH / fov; 

        DrawRectangle(r*line_width, line_offset, line_width, line_height, wall_color);

        ray_angle += (PI / 180);
    }
}

void display () {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // draw_map2D();
    // draw_player2D();
    cast_rays3D();

    EndDrawing();
}

int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raycaster");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) {    // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------
        handle_input();
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        display();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
