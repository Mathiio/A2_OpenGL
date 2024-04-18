#include "context/contextManager.hpp"
#include "GLFW/glfw3.h"
#include "camera.hpp"
#include "character/character.hpp"

void ContextManager::setup(p6::Context& ctx, Boids& boids)
{
    ctx.imgui = [&]() {
        boids.helper();
    };

    glEnable(GL_DEPTH_TEST);
}

void ContextManager::check_keys(p6::Context& ctx, Camera& camera, Character& character)
{
    if (ctx.key_is_pressed(GLFW_KEY_ESCAPE))
    {
        ctx.stop();
    }
    if (ctx.key_is_pressed(GLFW_KEY_LEFT) || ctx.key_is_pressed(GLFW_KEY_A))
    {
        character.rotate(camera, 5.0f);
    }
    if (ctx.key_is_pressed(GLFW_KEY_RIGHT) || ctx.key_is_pressed(GLFW_KEY_D))
    {
        character.rotate(camera, -5.0f);
    }
    if (ctx.key_is_pressed(GLFW_KEY_UP) || ctx.key_is_pressed(GLFW_KEY_W))
    {
        character.move(camera, -0.1f);
    }
    if (ctx.key_is_pressed(GLFW_KEY_DOWN) || ctx.key_is_pressed(GLFW_KEY_S))
    {
        character.move(camera, 0.1f);
    }
}