#pragma once
#include "boids.hpp"
#include "p6/p6.h"

void draw_boid(p6::Context& ctx, const Boid& boid)
{
    ctx.stroke = {1, 1, 1, 0};
    ctx.fill   = {1, 1, 1, 1};
    ctx.circle(
        p6::Center{boid.get_position()},
        p6::Radius{boid.get_radius()}
    );
}

void draw_square(p6::Context& ctx)
{
    ctx.stroke = {1, 1, 1, 1};
    ctx.fill   = {1, 1, 1, 0};
    ctx.square(
        p6::Center{0, 0},
        p6::Radius{0.75f}
    );
}