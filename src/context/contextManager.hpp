#pragma once

#include "camera.hpp"
#include "character/character.hpp"
#include "p6/p6.h"

class ContextManager {
public:
    static void setup(p6::Context& ctx, Camera& camera);
    static void check_keys(p6::Context& ctx, Camera& camera, Character& character);
};