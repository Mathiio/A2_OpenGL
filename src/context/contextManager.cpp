#include "context/contextManager.hpp"
#include "GLFW/glfw3.h"
#include "camera.hpp"
#include "character/character.hpp"

void ContextManager::setup(p6::Context& ctx)
{
    ctx.imgui = [&]() {
        ImGui::Begin("Controls");
        // ImGui::Text("Number of boids %zu", &numBoids);
        ImGui::SliderFloat("Turn factor", &turnFactor, .001f, .5f);
        ImGui::SliderFloat("Visual range", &visualRange, .001f, .5f);
        ImGui::SliderFloat("Protected range", &protectedRange, .001f, .5f);
        ImGui::SliderFloat("Cohesion factor", &cohesionFactor, .001f, .5f);
        ImGui::SliderFloat("Separation factor", &separationFactor, .001f, 1.f);
        ImGui::SliderFloat("Alignement factor", &alignementFactor, .001f, 1.f);
        ImGui::SliderFloat("Max speed", &maxSpeed, .001f, 1.f);
        ImGui::SliderFloat("Min speed", &minSpeed, .001f, 1.f);
        ImGui::SliderFloat("Movement speed", &movementSpeed, 0.01f, 0.1f);
        ImGui::SliderFloat("Rotation speed", &rotationSpeed, 1.0f, 5.0f);
        // ImGui::InputInt("Number of boids wanted", &numBoids);
        // if (ImGui::Button("Apply"))
        // {
        //     if (numBoids != static_cast<int>(boids.size()))
        //     {
        //         if (numBoids > 25)
        //         {
        //             numBoids = 25;
        //         }

        //         if (numBoids > static_cast<int>(boids.size()))
        //         {
        //             addBoid(numBoids - boids.size());
        //         }
        //         else
        //         {
        //             removeBoid(boids.size() - numBoids);
        //         }
        //     }
        // }
        ImGui::End();
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
        character.rotate(camera, rotationSpeed);
    }
    if (ctx.key_is_pressed(GLFW_KEY_RIGHT) || ctx.key_is_pressed(GLFW_KEY_D))
    {
        character.rotate(camera, -rotationSpeed);
    }
    if (ctx.key_is_pressed(GLFW_KEY_UP) || ctx.key_is_pressed(GLFW_KEY_W))
    {
        character.move(camera, -movementSpeed);
    }
    if (ctx.key_is_pressed(GLFW_KEY_DOWN) || ctx.key_is_pressed(GLFW_KEY_S))
    {
        character.move(camera, movementSpeed);
    }
}