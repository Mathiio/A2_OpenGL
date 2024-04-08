#include "helper/helper.hpp"
#include <imgui.h>

namespace Helper {
    void setupImGui(p6::Context& ctx, int& number, float& radius, float& separation_factor, float& separation_radius, float& alignment_factor, float& alignment_radius, float& cohesion_factor, float& cohesion_radius) {
        ctx.imgui = [&]() {
            ImGui::Begin("Controls");
            ImGui::SliderInt("Number", &number, 0, 200);
            ImGui::SliderFloat("Radius", &radius, 0.02f, 0.05f);
            ImGui::SliderFloat("Separation Factor", &separation_factor, 0.0f, 1.0f);
            ImGui::SliderFloat("Separation Radius", &separation_radius, 0.0f, 0.5f);
            ImGui::SliderFloat("Alignment Factor", &alignment_factor, 0.0f, 1.0f);
            ImGui::SliderFloat("Alignment Radius", &alignment_radius, 0.0f, 0.5f);
            ImGui::SliderFloat("Cohesion Factor", &cohesion_factor, 0.0f, 1.0f);
            ImGui::SliderFloat("Cohesion Radius", &cohesion_radius, 0.0f, 0.5f);
            ImGui::End();
        };
    }
}