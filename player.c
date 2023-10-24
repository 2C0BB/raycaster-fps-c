#include "player.h"
#include "angle_maths.h"
#include "include/raylib.h"
#include "map.h"

#include "consts.h"


// PLAYER VARS
const int max_dof = 20;
const int player_vel = 300;
const int player_turning_vel = 50;

float player_x = 300;
float player_y = 300;
float player_dx = 0;
float player_dy = 0;
float player_angle = 1;


void draw_player2D() {
    // DrawCircle(player_x, player_y, 8, YELLOW);
    DrawRectangle(player_x, player_y, 8, 8, RED);
    DrawLine(player_x, player_y, player_x + player_dx*5, player_y + player_dy*5, BLUE);
}

void handle_player_input(float delta_time) {
    if (IsKeyDown(KEY_A)) {
        player_angle -= 0.1 * player_turning_vel * delta_time;
    }
    if (IsKeyDown(KEY_D)) {
        player_angle += 0.1 * player_turning_vel * delta_time;
    }
    player_angle = clamp_angle(player_angle);


    bool w_down = IsKeyDown(KEY_W);
    bool s_down = IsKeyDown(KEY_S);
    if (w_down || s_down) {
       
        player_dx = cos(player_angle) * player_vel * delta_time;
        player_dy = sin(player_angle) * player_vel * delta_time;

        float new_x = player_x;
        float new_y = player_y;

        if (w_down) {
            new_x += player_dx;
            new_y += player_dy;
        }

        if (s_down) {
            new_x -= player_dx;
            new_y -= player_dy;
        }


        if (!is_wall(new_x/64, player_y/64)) {
            player_x = new_x;
        }
        if (!is_wall(player_x/64, new_y/64)) {
            player_y = new_y;
        }
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

    float ray_angle = player_angle - degs_to_rads(fov/2.0);

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

            if (map_pos < mapX * mapY && map_pos > 0 && map[map_pos] > 0) { // hit a wall

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

            if (map_pos < mapX * mapY && map_pos > 0 && map[map_pos] > 0) { // hit a wall
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


        int texture_col_offset;
        float color_mult;

        if (dist_vert < dist_horiz) { // vertical collision first
            ray_x = vx;
            ray_y = vy;
            final_distance = dist_vert;
            texture_col_offset = ((int)ray_y%64);
            color_mult = 0.7;
        } else { // horizontal collision first
            ray_x = hx;
            ray_y = hy;
            final_distance = dist_horiz;
            texture_col_offset = ((int)ray_x%64);
            color_mult = 1;
        }
        
        // 2D RAYS
        // DrawLine(player_x, player_y, ray_x, ray_y, BLUE);

        // 3D RAYS
        float ca = clamp_angle(player_angle - ray_angle);


        final_distance *= cos(ca); // using cosine to remove fisheye effect. This uses flat distance instead of direct.

        int line_height = mapS * SCREEN_HEIGHT / final_distance;
        
        float text_y_step = ((float) mapS) / (float)line_height;
        float text_y_offset = 0;

        if (line_height > SCREEN_HEIGHT) {
            text_y_offset = (line_height - SCREEN_HEIGHT)/2.0;
            line_height = SCREEN_HEIGHT;
        }
        float line_offset = (SCREEN_HEIGHT/2.0) - line_height/2.0;
        int line_width = SCREEN_WIDTH / fov; 


        DrawRectangle(r * line_width, 0, line_width, line_offset, SKYBLUE);

        float text_y = text_y_offset * text_y_step;
        for (int y = 0; y < line_height; y++) {
            Color pixel_color = generate_texture_at_point(texture_col_offset, (int)(text_y), color_mult);
            DrawRectangle(r * line_width, line_offset + y, line_width, 1, pixel_color); 
            text_y += text_y_step;
        }

        DrawRectangle(r * line_width, line_height + line_offset, line_width, line_offset, DARKGRAY);

        // REMOVE DEBUG
        DrawText(TextFormat("Player Rotation: %f", player_angle), 10, 10, 20, BLACK);
        // -----------------------------------------

        // DrawRectangle(r*line_width, line_offset, line_width, line_height, wall_color);

        ray_angle += degs_to_rads(1);
    }
}
