#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

struct CameraAxes {
    glm::vec3 right;
    glm::vec3 down;
    glm::vec3 front;
};

class Camera {
private:
    float m_distance;
    float m_angle_x;
    float m_angle_y;

    float     r_;
    float     phi_;
    float     theta_;
    glm::vec3 center_;

    static const glm::vec3 GRAVITY_UP;

public:
    Camera()
        : m_distance(5.0f), m_angle_x(0.0f), m_angle_y(0.0f) {}

    void move_front(float delta)
    {
        m_distance += delta;
    }

    void rotate_left(float degrees)
    {
        m_angle_y += degrees;
    }

    void rotateUp(float degrees)
    {
        m_angle_x += degrees;
    }

    glm::mat4 getViewMatrix() const
    {
        float angle_x_rad = glm::radians(m_angle_x);
        float angle_y_rad = glm::radians(m_angle_y);

        glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -m_distance));
        glm::mat4 rotation_x  = glm::rotate(glm::mat4(1.0f), angle_x_rad, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rotation_y  = glm::rotate(glm::mat4(1.0f), angle_y_rad, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 view_matrix = translation * rotation_x * rotation_y;

        return view_matrix;
    }

    CameraAxes compute_axes() const
    {
        const glm::vec3 front = glm::normalize(glm::vec3(-cos(phi_) * sin(theta_), sin(phi_), -cos(phi_) * cos(theta_)));

        // const glm::vec3 GRAVITY_UP(0.f, -1.f, 0.f);
        const glm::vec3 right = glm::normalize(glm::cross(front, GRAVITY_UP));
        const glm::vec3 down  = glm::normalize(glm::cross(front, right));
        return {right, down, front};
    };

    void lookAt()
    {
        CameraAxes axes = compute_axes();
        // const glm::vec3 GRAVITY_UP(0.f, -1.f, 0.f);

        const glm::vec3 eye  = center_ - r_ * axes.front;
        glm::mat4       view = glm::lookAt(eye, center_, GRAVITY_UP);

        // glMatrixMode(GL_MODELVIEW);
        // glLoadIdentity();
        // glLoadMatrixf(glm::value_ptr(view));
    }

    void zoom(int zoom_input)
    {
        r_ *= std::pow(1.1f, zoom_input);
    }
};