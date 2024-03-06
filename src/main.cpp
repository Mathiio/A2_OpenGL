#include <imgui.h>
#include <time.h>
#include <cstdlib>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT
#include "boids.hpp"
#include "doctest/doctest.h"
#include "draw.hpp"
#include "p6/p6.h"

int main()
{
    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    int               number            = 5;
    float             radius            = 0.02f;
    float             separation_factor = 1.0f;
    float             separation_radius = 0.1f;
    float             alignment_factor  = 1.0f;
    float             alignment_radius  = 0.1f;
    std::vector<Boid> boids(number);

    // Actual application code
    auto ctx = p6::Context{{.title = "Boids"}};
    ctx.maximize_window();

    ctx.imgui = [&]() {
        // Show a simple window
        ImGui::Begin("Controls");
        ImGui::SliderInt("Number", &number, 0, 200);
        ImGui::SliderFloat("Radius", &radius, 0.02f, 0.05f);
        ImGui::SliderFloat("Separation Factor", &separation_factor, 0.0f, 1.0f);
        ImGui::SliderFloat("Separation Radius", &separation_radius, 0.0f, 0.5f);
        ImGui::SliderFloat("Alignment Factor", &alignment_factor, 0.0f, 1.0f);
        ImGui::SliderFloat("Alignment Radius", &alignment_radius, 0.0f, 0.5f);
        ImGui::End();
    };

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Black);
        draw_square(ctx);

        boids.resize(number);

        for (size_t i = 0; i < boids.size(); ++i)
        {
            boids[i].set_radius(radius);
            boids[i].separation(boids, separation_factor, separation_radius);
            boids[i].alignment(boids, alignment_factor, alignment_radius);
            boids[i].update_position();
            draw_boid(ctx, boids[i]);
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}