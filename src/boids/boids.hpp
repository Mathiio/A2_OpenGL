#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "boids/boid.hpp"
#include "glimac/common.hpp"
#include "glm/fwd.hpp"
#include "obstacles/obstacles.hpp"

class Boids {
public:
    Boids(int nbBoids = 12);

    void addBoid(int number = 1);
    void removeBoid(int number = 1);
    void update(float delta_time, const Obstacles& obstacles);
    void draw(GLuint uMVPMatrixLocation, GLuint uMVMatrixLocation, GLuint uNormalMatrixLocation, glm::mat4 ProjMatrix, glm::mat4 viewMatrix, Mesh mesh, GLuint textName) const;
    void helper();

private:
    std::vector<Boid> boids;
    float             turnFactor{0.04f};
    float             visualRange{.15f};
    float             protectedRange{.065f};
    float             alignementFactor{.001f};
    float             separationFactor{.9f};
    float             cohesionFactor{.02f};
    float             maxSpeed{.8f};
    float             minSpeed{.65f};
    int               numBoids;
};