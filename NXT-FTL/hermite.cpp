﻿#include "hermite.h"

position calculate_hermite(float t, position P1, position P2, position R1, position R2)
{
    // Bezier
    // Based on https://stackoverflow.com/a/7962115/5154345
    float t2 = pow(t, 2);
    float t3 = pow(t, 3);

    float f1 = 2 * t3 - 3 * t2 + 1;
    float f2 = -2 * t3 + 3 * t2;
    float f3 = t3 - 2 * t2 + t;
    float f4 = t3 - t2;

    float Px = f1 * P1.x + f2 * P2.x + f3 * R1.x + f4 * R2.x;
    float Py = f1 * P1.y + f2 * P2.y + f3 * R1.y + f4 * R2.y;

    return position(Px , Py);
}

void hermite::get_points_between(int nb_points, position P1, position P2)
{
    position R1 = position(cos(P1.direction_in_rad), sin(P1.direction_in_rad));
    position R2 = position(cos(P2.direction_in_rad), sin(P2.direction_in_rad));
    float inc = 1.f / nb_points;
    
    for(float t = 0; t < 1; t += inc)
    {
        auto res = calculate_hermite(t, P1, P2, R1, R2);
        positions.push_back(res);
    }
}