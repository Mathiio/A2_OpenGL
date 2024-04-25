#pragma once

#include <glm/glm.hpp>
#include "glimac/common.hpp"
#include "glm/fwd.hpp"
#include "meshs/mesh.hpp"
#include "p6/p6.h"

class Boid {
public:
    Boid(glm::vec3 position, glm::vec3 velocity, glm::vec3 scale, float rotation);
    Boid();

    void      update(float delta_time);
    void      drawMesh(GLuint uMVPMatrixLocation, GLuint uMVMatrixLocation, GLuint uNormalMatrixLocation, glm::mat4 ProjMatrix, glm::mat4 viewMatrix, Mesh mesh) const;
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getVelocity() const { return velocity; }
    glm::vec3 getScale() const { return scale; }
    float     getRotation() const { return rotation; }
    void      setVelocity(glm::vec3 newVelocity) { this->velocity = newVelocity; };
    void      setPosition(glm::vec3 newPosition) { this->position = newPosition; };
    void      setScale(glm::vec3 newScale) { this->scale = newScale; };
    void      setRotation(float newRotation) { this->rotation = newRotation; };

private:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 scale;
    float     rotation;
};
