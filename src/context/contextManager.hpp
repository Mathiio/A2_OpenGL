#pragma once

#include "p6/p6.h"
#include "camera.hpp"
#include "boids/boids.hpp"

class ContextManager {
public:
    static void setup(p6::Context& ctx, Camera& camera, Boids& boids);
    static void check_keys(p6::Context& ctx);
};