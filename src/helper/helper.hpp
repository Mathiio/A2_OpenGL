#pragma once

#include "p6/p6.h"

namespace Helper {
    void setupImGui(p6::Context& ctx, int& number, float& radius, float& separation_factor, float& separation_radius, float& alignment_factor, float& alignment_radius, float& cohesion_factor, float& cohesion_radius);
}