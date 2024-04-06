#include <imgui.h>
#include <time.h>
#include <cstdlib>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT
#include "TrackballCamera.hpp"
#include "boids.hpp"
#include "doctest/doctest.h"
#include "draw.hpp"
#include "glimac/common.hpp"
#include "glimac/default_shader.hpp"
#include "glimac/sphere_vertices.hpp"
#include "p6/p6.h"

#define GLFW_INCLUDE_NONE

int main()
{
    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    int               number            = 100;
    float             radius            = 0.02f;
    float             separation_factor = 1.0f;
    float             separation_radius = 0.1f;
    float             alignment_factor  = 1.0f;
    float             alignment_radius  = 0.1f;
    float             cohesion_factor   = 1.0f;
    float             cohesion_radius   = 0.1f;
    std::vector<Boid> boids(number);
    TrackballCamera   camera;

    // Actual application code
    auto ctx = p6::Context{{.title = "Boids"}};
    ctx.maximize_window();

    const p6::Shader shader = p6::load_shader(
        "shaders/3D.vs.glsl",
        "shaders/normals.fs.glsl"
    );

    glEnable(GL_DEPTH_TEST);

    ctx.imgui = [&]() {
        // Show a simple window
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

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    const std::vector<glimac::ShapeVertex> vertices_sphere = glimac::sphere_vertices(1.f, 32, 16);
    glBufferData(GL_ARRAY_BUFFER, vertices_sphere.size() * sizeof(glimac::ShapeVertex), vertices_sphere.data(), GL_STATIC_DRAW);

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

    // Unbind the VAO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint uMVPMatrixLocation    = glGetUniformLocation(shader.id(), "uMVPMatrix");
    GLuint uMVMatrixLocation     = glGetUniformLocation(shader.id(), "uMVMatrix");
    GLuint uNormalMatrixLocation = glGetUniformLocation(shader.id(), "uNormalMatrix");

    glEnable(GL_DEPTH_TEST);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Black);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao);

        shader.use();

        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 viewMatrix = camera.getViewMatrix();

        boids.resize(number);

        for (Boid& boid : boids)
        {
            boid.draw(ctx, uMVPMatrixLocation, uMVMatrixLocation, uNormalMatrixLocation, ProjMatrix, viewMatrix, vertices_sphere);
            // boid.set_radius(radius);
            boid.separation(boids, separation_factor, separation_radius);
            boid.alignment(boids, alignment_factor, alignment_radius);
            boid.cohesion(boids, cohesion_factor, cohesion_radius);
            boid.update_position();
            // draw_boid(ctx, boid);
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}