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

    int               number = 5;
    float             radius = 0.02f;
    std::vector<Boid> boids(number);

    // Actual application code
    auto ctx = p6::Context{{.title = "Boids"}};
    ctx.maximize_window();

    ctx.imgui = [&]() {
        // Show a simple window
        ImGui::Begin("Controls");
        ImGui::SliderInt("Number", &number, 0, 100);
        ImGui::SliderFloat("Radius", &radius, 0.02f, 0.05f);
        ImGui::End();
    };

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Black);
        draw_square(ctx);

        boids.resize(number);

        // for (auto& boid : boids)
        // {
        //     boid.set_radius(radius);
        //     boid.update_position();
        //     draw_boid(ctx, boid);
        // }
        for (size_t i = 0; i < boids.size(); ++i)
        {
            boids[i].set_radius(radius);
            // Assume here you want each boid to avoid collision with every other boid
            for (size_t j = 0; j < boids.size(); ++j)
            {
                if (i != j)
                { // Don't compare a boid with itself
                    boids[i].repulsion(boids[j]);
                }
            }
            // boids[i].drag();
            boids[i].update_position();
            draw_boid(ctx, boids[i]);
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}