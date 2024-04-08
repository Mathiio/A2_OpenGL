#pragma once

#include "p6/p6.h"
#include "camera.hpp"

class ContextManager {
public:
    static void setup(p6::Context& ctx, Camera& camera);
    static void check_keys(p6::Context& ctx);
};