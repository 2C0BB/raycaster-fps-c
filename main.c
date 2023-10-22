#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "player.h"
#include "consts.h"

#include "include/raylib.h"





void display () {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // draw_map2D();
    // draw_player2D();
    cast_rays3D();

    DrawFPS(20, 20);

    EndDrawing();
}

int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raycaster");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    float delta_time;
    // Main game loop
    while (!WindowShouldClose()) {    // Detect window close button or ESC key

        delta_time = GetFrameTime();

        // Update
        //----------------------------------------------------------------------------------
        handle_player_input(delta_time);
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
