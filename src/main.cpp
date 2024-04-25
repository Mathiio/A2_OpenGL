#include <imgui.h>
#include <time.h>
#include <cstdlib>

#define DOCTEST_CONFIG_IMPLEMENT
#include <iostream>
#include "boids/boids.hpp"
#include "camera.hpp"
#include "character/character.hpp"
#include "context/contextManager.hpp"
#include "doctest/doctest.h"
#include "light.hpp"
#include "meshs/mesh.hpp"
#include "meshs/meshs.hpp"
#include "obstacles/obstacles.hpp"
#include "random/random.hpp"

#define GLFW_INCLUDE_NONE

int main()
{
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    auto ctx = p6::Context{{.title = "Bees"}};
    srand(static_cast<unsigned int>(time(nullptr)));

    Camera         camera;
    Character      character;
    Boids          boids;
    Obstacles      obstacles;
    ContextManager context;
    context.setup(ctx);

    obstacles.addObstacle({{0.727f, -0.30f, 0.672f}, 0.544f, 1.30f, 0.65f});      // Grand Sapin
    obstacles.addObstacle({{-0.690f, -0.750f, 0.549f}, 0.62f, 0.66f, 0.5f});      // Ruche à abeilles
    obstacles.addObstacle({{-0.401f, -0.620f, -0.862f}, 0.31f, 0.74f, 0.27f});    // Petit sapin
    obstacles.addObstacle({{-0.644f, -0.890f, -0.076f}, 0.524f, 0.266f, 0.466f}); // Arbre coupé
    obstacles.addObstacle({{-0.819f, -0.856f, -0.841f}, 0.370f, 0.340f, 0.328f}); // Tronc d'arbre

    const p6::Shader shader = p6::load_shader(
        "shaders/3D.vs.glsl",
        "shaders/pointLight.fs.glsl"
    );

    GLuint uMVPMatrixLocation    = glGetUniformLocation(shader.id(), "uMVPMatrix");
    GLuint uMVMatrixLocation     = glGetUniformLocation(shader.id(), "uMVMatrix");
    GLuint uNormalMatrixLocation = glGetUniformLocation(shader.id(), "uNormalMatrix");

    glEnable(GL_DEPTH_TEST);

    Mesh  decor("decor.obj", "decor.png");
    Mesh  boid("bee.obj", "bee.png");
    Meshs clouds("cloud.obj", "cloud.png", 10);
    clouds.randomPos();
    clouds.randomScale();
    boids.randomRotation();

    Light lightCharacter(1, shader);
    Light lightFixed(2, shader);

    shader.use();

    ctx.update = [&]() {
        if (context.getIsNight())
        {
            ctx.background({0.06f, 0.08f, 0.0f});
        }
        else
        {
            ctx.background({0.53f, 0.65f, 0.83f});
        }
        context.check_keys(ctx, camera, character);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 viewMatrix = camera.getViewMatrix();

        lightCharacter.update(viewMatrix, character);
        lightFixed.update(viewMatrix, character);

        character.draw(uMVPMatrixLocation, uMVMatrixLocation, uNormalMatrixLocation, ProjMatrix, viewMatrix, boid);
        clouds.draw(ProjMatrix, viewMatrix, uMVPMatrixLocation, uMVMatrixLocation, uNormalMatrixLocation, 0.0f);
        decor.draw(glm::vec3(0.), glm::vec3{1.}, ProjMatrix, viewMatrix, uMVPMatrixLocation, uMVMatrixLocation, uNormalMatrixLocation, 0.0f);
        boids.draw(uMVPMatrixLocation, uMVMatrixLocation, uNormalMatrixLocation, ProjMatrix, viewMatrix, boid);
        boids.update(ctx.delta_time(), obstacles, context);
    };
    ctx.start();
    boid.~Mesh();
    decor.~Mesh();
    character.~Character();
}