#ifndef PLAYER_H
#define PLAYER_H

#include <math.h>

#include "include/raylib.h"

void draw_player2D();
void handle_player_input(float delta_time);
void cast_rays3D();

#endif
