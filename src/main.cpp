#include <imgui.h>
#include <time.h>
#include <cstdlib>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT
#include <iostream>
#include "boids/boids.hpp"
#include "camera.hpp"
#include "character/character.hpp"
#include "context/ContextManager.hpp"
#include "doctest/doctest.h"
#include "glimac/common.hpp"
#include "meshs/mesh.hpp"
#include "obstacles/obstacles.hpp"
#include "random/random.hpp"
#include "textures/texture.hpp"

#define GLFW_INCLUDE_NONE

int main()
{
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    auto ctx = p6::Context{{.title = "Bees"}};

    Camera    camera;
    Character character;
    Boids     boids;
    Obstacles obstacles;
    ContextManager::setup(ctx, boids);

    obstacles.addObstacle({{0.727f, -0.30f, 0.672f}, 0.544f, 1.30f, 0.65f}); // Grand Sapin
    obstacles.addObstacle({{-0.690f, -0.750f, 0.549f}, 0.62f, 0.66f, 0.5f}); // Ruche à abeilles
    obstacles.addObstacle({{-0.401f, -0.620f, -0.862f}, 0.31f, 0.74f, 0.27f}); // Petit sapin
    obstacles.addObstacle({{-0.644f, -0.890f, -0.076f}, 0.524f, 0.266f, 0.466f}); // Arbre coupé
    obstacles.addObstacle({{-0.819f, -0.856f, -0.841f}, 0.370f, 0.340f, 0.328f}); // Tronc d'arbre

    const p6::Shader shaderTexture = p6::load_shader(
        "shaders/3D.vs.glsl",
        "shaders/directionalLight.fs.glsl"
    );

    GLuint uMVPMatrixLocation      = glGetUniformLocation(shaderTexture.id(), "uMVPMatrix");
    GLuint uMVMatrixLocation       = glGetUniformLocation(shaderTexture.id(), "uMVMatrix");
    GLuint uNormalMatrixLocation   = glGetUniformLocation(shaderTexture.id(), "uNormalMatrix");
    GLuint uKdLocation             = glGetUniformLocation(shaderTexture.id(), "uKd");
    GLuint uKsLocation             = glGetUniformLocation(shaderTexture.id(), "uKs");
    GLuint uShininessLocation      = glGetUniformLocation(shaderTexture.id(), "uShininess");
    GLuint uLightDirLocation       = glGetUniformLocation(shaderTexture.id(), "uLightDir_vs");
    GLuint uLightIntensityLocation = glGetUniformLocation(shaderTexture.id(), "uLightIntensity");
    glEnable(GL_DEPTH_TEST);

    Mesh   decor("decor.obj");
    GLuint decorBake = Texture::instance().loadTexture("assets/textures/decor.png");
    decor.setBuffers();

    Mesh   boid("bee.obj");
    GLuint beeBake = Texture::instance().loadTexture("assets/textures/bee.png");
    boid.setBuffers();

    Mesh   cloud1("cloud1.obj");
    GLuint cloud1Bake = Texture::instance().loadTexture("assets/textures/cloud1.png");
    cloud1.setBuffers();

    Mesh      cloud2("cloud2.obj");
    glm::vec3 newPosCloud2 = randomPos();
    GLuint cloud2Bake = Texture::instance().loadTexture("assets/textures/cloud2.png");
    cloud2.setBuffers();

    shaderTexture.use();

    ctx.update = [&]() {
        ctx.background({0.06f, 0.08f, 0.0f});
        ContextManager::check_keys(ctx, camera, character);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 viewMatrix = camera.getViewMatrix();

        glm::vec4 lightDir    = {-2.0f, 1.0f, -2.0f, 0.0f};
        glm::vec3 lightDir_vs = glm::vec3(viewMatrix * lightDir);

        glUniform3f(uKdLocation, 1.0, 0.9f, 0.65f);                                  // Couleur diffuse
        glUniform3f(uKsLocation, 1.0f, 1.0f, 1.0f);                                  // Couleur spéculaire
        glUniform1f(uShininessLocation, 4.0f);                                       // Brilliance
        glUniform3f(uLightDirLocation, lightDir_vs.x, lightDir_vs.y, lightDir_vs.z); // Direction de la lumière
        glUniform3f(uLightIntensityLocation, 2.0f, 2.0f, 2.0f);                      // Intensité de la lumière

        character.draw(uMVPMatrixLocation, uMVMatrixLocation, uNormalMatrixLocation, ProjMatrix, viewMatrix, boid, beeBake);
        cloud2.draw(newPosCloud2, glm::vec3{1.}, ProjMatrix, viewMatrix, uMVPMatrixLocation, uMVMatrixLocation, uNormalMatrixLocation, cloud2Bake, 0.0f);
        decor.draw(glm::vec3(0., 0., 0.), glm::vec3{1.}, ProjMatrix, viewMatrix, uMVPMatrixLocation, uMVMatrixLocation, uNormalMatrixLocation, decorBake, 0.0f);
        boids.draw(uMVPMatrixLocation, uMVMatrixLocation, uNormalMatrixLocation, ProjMatrix, viewMatrix, boid, beeBake);
        boids.update(ctx.delta_time(), obstacles);
    };
    ctx.start();
    boid.~Mesh();
    decor.~Mesh();
    character.~Character();
}