#pragma once

#include <glm/glm.hpp>
#include "glimac/common.hpp"
#include "glm/fwd.hpp"
#include "meshs/mesh.hpp"
#include "p6/p6.h"

class Character {
public:
    Character(glm::vec3 position, glm::vec3 velocity, float rotation);
    Character();

    // void update(Camera& camera, float delta_time);
    void draw(GLuint uMVPMatrixLocation, GLuint uMVMatrixLocation, GLuint uNormalMatrixLocation, glm::mat4 ProjMatrix, glm::mat4 viewMatrix, Mesh mesh, GLuint textName) const;

    glm::vec3 getPosition() const;
    glm::vec3 getVelocity() const;
    float     getRotation() const;
    void      setVelocity(glm::vec3 velocity) { this->velocity = velocity; }
    void      setPosition(glm::vec3 position) { this->position = position; }
    void      setRotation(float rotation) { this->rotation = rotation; }

private:
    glm::vec3 position;
    glm::vec3 velocity;
    float     rotation;
};
