#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "boids/boid.hpp"
#include "glimac/common.hpp"
#include "glm/fwd.hpp"
#include "obstacles/obstacles.hpp"
#include "random/random.hpp"

class Boids {
public:
    // Boids(int nbBoids = 10);
    Boids(int nbBoids = randExponential(1, 25));

    void addBoid(int number = 1);
    void removeBoid(int number = 1);
    void update(float delta_time, const Obstacles& obstacles);
    void draw(GLuint uMVPMatrixLocation, GLuint uMVMatrixLocation, GLuint uNormalMatrixLocation, glm::mat4 ProjMatrix, glm::mat4 viewMatrix, Mesh mesh) const;
    void randomRotation();
    void helper();

    float getMovementSpeed() { return movementSpeed; };
    float getRotationSpeed() { return rotationSpeed; };

private:
    std::vector<Boid> boids;
    float             turnFactor{0.04f};
    float             visualRange{.15f};
    float             protectedRange{.065f};
    float             alignementFactor{.001f};
    float             separationFactor{.9f};
    float             cohesionFactor{.02f};
    float             minSpeed{randBeta(0.01f, 0.6f, 0.0f)};
    float             maxSpeed{randBeta(0.01f, 0.6f, minSpeed)};
    // float             maxSpeed{.6f};
    // float             minSpeed{.4f};
    float movementSpeed{0.1f};
    float rotationSpeed{5.0f};
    int   numBoids;
};