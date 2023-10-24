#include "angle_maths.h"
#include "include/raylib.h"
#include <math.h>

float degs_to_rads(float degrees) {
    return degrees * PI / 180.0;
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
