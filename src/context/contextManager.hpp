#pragma once

#include "boids/boid.hpp"
#include "camera.hpp"
#include "character/character.hpp"
#include "p6/p6.h"
#include "random/random.hpp"

class ContextManager {
private:
    // std::vector<Boid> boids;
    float turnFactor{0.04f};
    float visualRange{.15f};
    float protectedRange{.065f};
    float alignementFactor{.001f};
    float separationFactor{.9f};
    float cohesionFactor{.02f};
    float minSpeed{randBeta(0.01f, 0.6f, 0.0f)};
    float maxSpeed{randBeta(0.01f, 0.6f, minSpeed)};
    float movementSpeed{0.1f};
    float rotationSpeed{5.0f};
    int   numBoids;
    bool  isNight{randPoisson(0.1f)};
    bool  isLowPoly{false};

public:
    void setup(p6::Context& ctx);
    void check_keys(p6::Context& ctx, Camera& camera, Character& character);
    // std::vector<Boid>& getBoids() { return boids; }
    float getTurnFactor() const { return turnFactor; }
    float getVisualRange() const { return visualRange; }
    float getProtectedRange() const { return protectedRange; }
    float getAlignementFactor() const { return alignementFactor; }
    float getSeparationFactor() const { return separationFactor; }
    float getCohesionFactor() const { return cohesionFactor; }
    float getMinSpeed() const { return minSpeed; }
    float getMaxSpeed() const { return maxSpeed; }
    int   getNumBoids() const { return numBoids; }
    bool  getIsNight() const { return isNight; }
    bool  getIsLowPoly() const { return isLowPoly; }
};