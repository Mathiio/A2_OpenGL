#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "boids/boid.hpp"
#include "context/contextManager.hpp"
#include "glm/fwd.hpp"
#include "obstacles/obstacles.hpp"
#include "random/random.hpp"

class Boids {
public:
    Boids()
        : numBoids(0)
    {
        initNumberBoid();
    }

    void initNumberBoid();
    void addBoid(int number = 1);
    void removeBoid(int number = 1);
    void update(float delta_time, const Obstacles& obstacles, ContextManager context);
    void draw(GLuint uMVPMatrixLocation, GLuint uMVMatrixLocation, GLuint uNormalMatrixLocation, glm::mat4 ProjMatrix, glm::mat4 viewMatrix, Mesh& mesh) const;
    void randomRotation();

private:
    std::vector<Boid> boids;
    int               numBoids;
};