#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Camera {
private:
    float m_Distance;
    float m_AngleX;
    float m_AngleY;

public:
    Camera()
        : m_Distance(2.0f), m_AngleX(0.0f), m_AngleY(180.0f) {}

    void moveFront(float delta)
    {
        m_Distance += delta;
    }

    void rotateLeft(float degrees)
    {
        m_AngleY += degrees;
    }

    void rotateUp(float degrees)
    {
        m_AngleX += degrees;
    }

    glm::mat4 getViewMatrix() const
    {
        float angleX_rad = glm::radians(m_AngleX);
        float angleY_rad = glm::radians(m_AngleY);

        glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -m_Distance));
        glm::mat4 rotationX   = glm::rotate(glm::mat4(1.0f), angleX_rad, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rotationY   = glm::rotate(glm::mat4(1.0f), angleY_rad, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 viewMatrix  = translation * rotationX * rotationY;

        return viewMatrix;
    }
};