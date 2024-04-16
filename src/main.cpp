#include <imgui.h>
#include <time.h>
#include <cstdlib>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT
#include "boids/boids.hpp"
#include "camera.hpp"
#include "character/character.hpp"
#include "context/ContextManager.hpp"
#include "doctest/doctest.h"
#include "glimac/common.hpp"
#include "meshs/mesh.hpp"

#define GLFW_INCLUDE_NONE

int main()
{
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    auto ctx = p6::Context{{.title = "Boids"}};
    ctx.maximize_window();
    Camera    camera;
    Character character;
    ContextManager::setup(ctx, camera);
    Boids boids;

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
    img::Image decorText = p6::load_image_buffer("assets/textures/decor_bake.png");

    Mesh boid;
    boid.loadModel("bee.obj");
    img::Image beeText = p6::load_image_buffer("assets/textures/bee.png");

    GLuint beeBake;
    glGenTextures(1, &beeBake);
    glBindTexture(GL_TEXTURE_2D, beeBake);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, beeText.width(), beeText.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, beeText.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint decorBake;
    glGenTextures(1, &decorBake);
    glBindTexture(GL_TEXTURE_2D, decorBake);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, decorText.width(), decorText.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, decorText.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    decor.setVbo();
    decor.setVao();

    boid.setVbo();
    boid.setVao();

    shaderTexture.use();

    ctx.update = [&]() {
        ctx.background({0.06f, 0.08f, 0.0f});
        ContextManager::check_keys(ctx, camera, character);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);

        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 viewMatrix = camera.getViewMatrix();

        glm::vec4 lightDir    = {-2.0f, 1.0f, -2.0f, 0.0f};
        glm::vec3 lightDir_vs = glm::vec3(viewMatrix * lightDir);

        glUniform3f(uKdLocation, 1.0, 0.9f, 0.65f);                                  // Couleur diffuse
        glUniform3f(uKsLocation, 1.0f, 1.0f, 1.0f);                                  // Couleur spéculaire
        glUniform1f(uShininessLocation, 4.0f);                                       // Brilliance
        glUniform3f(uLightDirLocation, lightDir_vs.x, lightDir_vs.y, lightDir_vs.z); // Direction de la lumière
        glUniform3f(uLightIntensityLocation, 2.0f, 2.0f, 2.0f);                      // Intensité de la lumière

        decor.draw(glm::vec3(0., -1.5f, 0.), glm::vec3{1.}, ProjMatrix, viewMatrix, uMVPMatrixLocation, uMVMatrixLocation, uNormalMatrixLocation, decorBake, 0.0f);

        shaderTexture.use();

        boids.draw(uMVPMatrixLocation, uMVMatrixLocation, uNormalMatrixLocation, ProjMatrix, viewMatrix, boid, beeBake);
        boids.update(ctx.delta_time());

        character.draw(uMVPMatrixLocation, uMVMatrixLocation, uNormalMatrixLocation, ProjMatrix, viewMatrix, boid, beeBake);
    };
    ctx.start();

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    boid.~Mesh();
    decor.~Mesh();
    character.~Character();
}