#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "boids/boid.hpp"
#include "glimac/common.hpp"
#include "glm/fwd.hpp"

class Boids {
public:
    Boids(int nbBoids = 200);

    void addBoid(int number = 1);
    void removeBoid(int number = 1);
    void update(float delta_time);
    void draw(GLuint uMVPMatrixLocation, GLuint uMVMatrixLocation, GLuint uNormalMatrixLocation, glm::mat4 ProjMatrix, glm::mat4 viewMatrix, std::vector<glimac::ShapeVertex> vertices_sphere) const;
    void helper();

private:
    std::vector<Boid> boids;
    float turnFactor{0.03f};
    float visualRange{.15f};
    float protectedRange{.065f};
    float alignementFactor{.001f};
    float separationFactor{.9f};
    float cohesionFactor{.02f};
    float maxSpeed{.8f};
    float minSpeed{.5f};
    int numBoids;
};