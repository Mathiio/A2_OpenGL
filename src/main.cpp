#include <imgui.h>
#include <time.h>
#include <cstdlib>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT
#include "boids/boids.hpp"
#include "camera.hpp"
#include "context/ContextManager.hpp"
#include "doctest/doctest.h"
#include "glimac/common.hpp"
#include "meshs/mesh.hpp"
#include "obstacles/obstacles.hpp"
#include "textures/texture.hpp"

#define GLFW_INCLUDE_NONE

int main()
{
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    auto ctx = p6::Context{{.title = "Boids"}};

    Camera camera;
    ContextManager::setup(ctx, camera);
    Boids boids;
    Obstacles obstacles;


    obstacles.addObstacle({{0.727f, -0.672f, -0.386f}, 0.544f, 0.65f, 1.30f});
    obstacles.addObstacle({{-0.690f, -0.549f, -0.850f}, 0.62f, 0.5f, 0.66f});
    obstacles.addObstacle({{-0.401f, -0.862f, -0.72f}, 0.31f, 0.27f, 0.74f});


    ctx.imgui = [&]() {
        boids.helper();
    };

    const p6::Shader shaderTexture = p6::load_shader(
        "shaders/3D.vs.glsl",
        "shaders/directionalLight.fs.glsl"
    );

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    static constexpr GLuint vertex_attr_position = 0;
    glEnableVertexAttribArray(vertex_attr_position);
    glVertexAttribPointer(vertex_attr_position, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, position)));

    static constexpr GLuint vertex_attr_normal = 1;
    glEnableVertexAttribArray(vertex_attr_normal);
    glVertexAttribPointer(vertex_attr_normal, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal)));

    static constexpr GLuint vertex_attr_texCoords = 2;
    glEnableVertexAttribArray(vertex_attr_texCoords);
    glVertexAttribPointer(vertex_attr_texCoords, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint uMVPMatrixLocation    = glGetUniformLocation(shaderTexture.id(), "uMVPMatrix");
    GLuint uMVMatrixLocation     = glGetUniformLocation(shaderTexture.id(), "uMVMatrix");
    GLuint uNormalMatrixLocation = glGetUniformLocation(shaderTexture.id(), "uNormalMatrix");
    GLuint uText                 = glGetUniformLocation(shaderTexture.id(), "uText");

    GLuint uKdLocation             = glGetUniformLocation(shaderTexture.id(), "uKd");
    GLuint uKsLocation             = glGetUniformLocation(shaderTexture.id(), "uKs");
    GLuint uShininessLocation      = glGetUniformLocation(shaderTexture.id(), "uShininess");
    GLuint uLightDirLocation       = glGetUniformLocation(shaderTexture.id(), "uLightDir_vs");
    GLuint uLightIntensityLocation = glGetUniformLocation(shaderTexture.id(), "uLightIntensity");

    glEnable(GL_DEPTH_TEST);

    Mesh decor;
    decor.loadModel("decor.obj");
    GLuint decorBake = Texture::instance().loadTexture("assets/textures/decor.png");

    Mesh boid;
    boid.loadModel("bee.obj");
    GLuint beeBake = Texture::instance().loadTexture("assets/textures/bee.png");

    Mesh cloud1;
    cloud1.loadModel("cloud1.obj");
    GLuint cloud1Bake = Texture::instance().loadTexture("assets/textures/cloud1.png");

    Mesh cloud2;
    cloud2.loadModel("cloud2.obj");
    GLuint cloud2Bake = Texture::instance().loadTexture("assets/textures/cloud2.png");


    decor.setVbo();
    decor.setVao();

    boid.setVbo();
    boid.setVao();

    cloud1.setVbo();
    cloud1.setVao();

    cloud2.setVbo();
    cloud2.setVao();

    shaderTexture.use();

    ctx.update = [&]() {
        ctx.background({0.07f, 0.09f, 0.0f});
        ContextManager::check_keys(ctx);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);

        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 viewMatrix = camera.getViewMatrix();

        glUniform3f(uKdLocation, 1.0, 0.9f, 0.65f); // Couleur diffuse (gris clair)
        glUniform3f(uKsLocation, 1.0f, 1.0f, 1.0f); // Couleur spéculaire (blanc)
        glUniform1f(uShininessLocation, 4.0f);      // Brilliance

        glm::vec4 lightDir    = {-2.0f, 1.0f, -2.0f, 0.0f};
        glm::vec3 lightDir_vs = glm::vec3(viewMatrix * lightDir);

        glUniform3f(uLightDirLocation, lightDir_vs.x, lightDir_vs.y, lightDir_vs.z); // Direction de la lumière (vers le haut)
        glUniform3f(uLightIntensityLocation, 2.0f, 2.0f, 2.0f);

        decor.draw(glm::vec3(0., 0., 0.), glm::vec3{1.}, ProjMatrix, viewMatrix, uMVPMatrixLocation, uMVMatrixLocation, uNormalMatrixLocation, decorBake);

        shaderTexture.use();

        boids.draw(uMVPMatrixLocation, uMVMatrixLocation, uNormalMatrixLocation, ProjMatrix, viewMatrix, boid, beeBake);
        boids.update(ctx.delta_time(), obstacles);
    };
    ctx.start();

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}