#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "character/character.hpp"

class Camera {
private:
    glm::vec3 m_Position;
    float     m_Phi;
    float     m_Theta;

    glm::vec3 m_FrontVector;
    glm::vec3 m_LeftVector;
    glm::vec3 m_UpVector;

    // Private method to compute direction vectors
    void computeDirectionVectors()
    {
        m_FrontVector.x = std::sin(m_Theta) * std::cos(m_Phi);
        m_FrontVector.y = std::sin(m_Phi);
        m_FrontVector.z = std::cos(m_Theta) * std::cos(m_Phi);
        m_LeftVector    = glm::normalize(glm::cross(m_FrontVector, glm::vec3(0.0f, 1.0f, 0.0f)));
        m_UpVector      = glm::normalize(glm::cross(m_LeftVector, m_FrontVector));
    }

public:
    // Constructor
    Camera()
        : m_Position(0.0f, 0.0f, -2.0f), m_Phi(glm::pi<float>()), m_Theta(glm::radians(180.0f)), m_FrontVector(0.0f, 0.0f, -1.0f), m_LeftVector(-1.0f, 0.0f, 0.0f), m_UpVector(0.0f, 1.0f, 0.0f)
    {
        computeDirectionVectors();
    }

    // Move along left vector
    void moveLeft(float t) { m_Position -= t * m_LeftVector; }

    // Move along front vector
    void moveFront(float t) { m_Position -= t * m_FrontVector; }

    // Rotate left
    void rotateLeft(float degrees)
    {
        float radians = glm::radians(degrees);
        m_Theta += radians;
        computeDirectionVectors();
    }

    // Rotate up
    void rotateUp(float degrees)
    {
        float radians = glm::radians(degrees);
        m_Phi += radians;
        computeDirectionVectors();
    }

    // Get view matrix
    glm::mat4 getViewMatrix() const { return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector); }
    glm::vec3 getPosition() const { return m_Position; }

    float getTheta() const { return m_Theta; }
    float getPhi() const { return m_Phi; }

    void moveCharacter(Character& character, float direction)
    {
        glm::vec3 newPosition = character.getPosition() + direction * m_FrontVector;
        character.setPosition(newPosition);
    }

    void rotateCharacter(Character& character, float orientation)
    {
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f * orientation), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 cameraToCharacter   = glm::translate(glm::mat4(1.0f), -character.getPosition());
        glm::mat4 cameraFromCharacter = glm::translate(glm::mat4(1.0f), character.getPosition());
        glm::mat4 cameraTransform     = cameraFromCharacter * rotationMatrix * cameraToCharacter;

        glm::vec4 cameraPoint  = cameraTransform * glm::vec4(m_Position, 1.0f);
        glm::vec3 cameraCoords = glm::vec3(cameraPoint);
        m_Position             = cameraCoords;

        character.setRotation(glm::degrees(m_Theta));
    }
};