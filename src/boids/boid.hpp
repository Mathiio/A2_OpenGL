#pragma once

#include <glm/glm.hpp>
#include "glimac/common.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Boid {
public:
    Boid(glm::vec3 position, glm::vec3 velocity, float radius);
    Boid();

    void      update(float delta_time);
    void      draw(GLuint uMVPMatrixLocation, GLuint uMVMatrixLocation, GLuint uNormalMatrixLocation, glm::mat4 ProjMatrix, glm::mat4 viewMatrix, std::vector<glimac::ShapeVertex> vertices_sphere) const;

    glm::vec3 getPosition() const;
    glm::vec3 getVelocity() const;
    void      setVelocity(glm::vec3 velocity) { this->velocity = velocity; }
    void      set_position(glm::vec3 position) { this->position = position; }

private:
    glm::vec3 position;
    glm::vec3 velocity;
    float     radius;
};
