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

#define GLFW_INCLUDE_NONE

int main()
{
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    auto ctx = p6::Context{{.title = "Boids"}};
    ctx.maximize_window();
    Camera camera;
    ContextManager::setup(ctx, camera);
    Boids boids;

    ctx.imgui = [&]() {
        boids.helper();
    };

    const p6::Shader shaderTexture = p6::load_shader(
        "shaders/3D.vs.glsl",
        "shaders/texture.fs.glsl"
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
    // GLuint uText                 = glGetUniformLocation(shaderTexture.id(), "uText");

    glEnable(GL_DEPTH_TEST);

    Mesh decor;
    decor.loadModel("decor.obj");
    img::Image decorText = p6::load_image_buffer("assets/textures/test.jpg");

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
        ctx.background({0.07f, 0.09f, 0.0f});
        // ctx.background(p6::NamedColor::Black);
        ContextManager::check_keys(ctx);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);

        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 viewMatrix = camera.getViewMatrix();

        decor.draw(glm::vec3(0., -1., 0.), glm::vec3{1.}, ProjMatrix, viewMatrix, uMVPMatrixLocation, uMVMatrixLocation, uNormalMatrixLocation, decorBake);

        shaderTexture.use();
        boids.draw(uMVPMatrixLocation, uMVMatrixLocation, uNormalMatrixLocation, ProjMatrix, viewMatrix, boid, beeBake);
        boids.update(ctx.delta_time());
    };
    ctx.start();

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    // boid.~Mesh();
}