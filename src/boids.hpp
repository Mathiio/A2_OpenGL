#pragma once
#include <vector>
#include "glimac/common.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "p6/p6.h"

class Boid {
private:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float     radius;

public:
    Boid()
        : position({p6::random::number(-0.3f + 0.02f, 0.3f - 0.02f), p6::random::number(-0.3f + 0.02f, 0.3f - 0.02f), p6::random::number(-0.3f + 0.02f, 0.3f - 0.02f)})
        , velocity({p6::random::number(-1.0f, 1.0f), p6::random::number(-1.0f, 1.0f), p6::random::number(-1.0f, 1.0f)})
        , acceleration({p6::random::number(-1.0f, 1.0f), p6::random::number(-1.0f, 1.0f), p6::random::number(-1.0f, 1.0f)})
        , radius(0.02f) {}

    Boid(const glm::vec3& init_position, const glm::vec3& init_velocity)
        : position(init_position), velocity(init_velocity), acceleration(1.0f, 1.0f, 1.0f), radius(0.02f) {}

    glm::vec3 get_position() const { return position; }
    float     get_radius() const { return radius; }
    void      set_radius(float new_radius) { radius = new_radius; }

    void draw(p6::Context& ctx, GLuint uMVPMatrixLocation, GLuint uMVMatrixLocation, GLuint uNormalMatrixLocation, glm::mat4 ProjMatrix, glm::mat4 viewMatrix, std::vector<glimac::ShapeVertex> vertices_sphere) const
    {
        // Position
        glm::vec3 spherePosition = glm::vec3(position.x, position.y, position.z);

        glm::vec3 direction(velocity.x, velocity.y, velocity.z);
        direction = glm::normalize(direction);
        glm::vec3 directionOrthogonale(-direction.y, direction.x, 0.0f);
        directionOrthogonale      = glm::normalize(directionOrthogonale);
        glm::vec3 directionFinale = glm::cross(direction, directionOrthogonale);
        directionFinale           = glm::normalize(directionFinale);

        glm::mat4 rotationMatrix(
            glm::vec4(directionFinale, 0.0f),
            glm::vec4(direction, 0.0f),
            glm::vec4(directionOrthogonale, 0.0f),
            glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
        );

        glm::mat4 MVMatrix = viewMatrix * glm::translate(glm::mat4{1.f}, spherePosition) * glm::scale(glm::mat4{1.f}, glm::vec3(0.015f)) * rotationMatrix;

        // NormalMatrix
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        glDrawArrays(GL_TRIANGLES, 0, vertices_sphere.size());
    }

    void update_position()
    {
        if (position.x < -0.75f + radius || position.x > 0.75f - radius)
        {
            velocity.x *= -1;
        }
        if (position.y < -0.75f + radius || position.y > 0.75f - radius)
        {
            velocity.y *= -1;
        }
        if (position.z < -0.75f + radius || position.z > 0.75f - radius)
        {
            velocity.z *= -1;
        }

        //! ???
        float maxSpeed = 1.0f;
        float speed    = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
        if (speed > maxSpeed)
        {
            velocity.x = velocity.x * maxSpeed / speed;
            velocity.y = velocity.y * maxSpeed / speed;
            velocity.z = velocity.z * maxSpeed / speed;
        }
        position += velocity / 100.f;
    }

    std::vector<Boid> boids_in_area(std::vector<Boid>& boids, float radius)
    {
        std::vector<Boid> boids_in_area;
        for (const Boid& boid : boids)
        {
            glm::vec3 delta    = boid.position - position;
            float     distance = glm::length(delta);
            if (distance < radius)
            {
                boids_in_area.push_back(boid);
            }
        }
        return boids_in_area;
    }

    void separation(std::vector<Boid>& boids, float separation_factor, float separation_radius)
    {
        std::vector<Boid> other_boids = boids_in_area(boids, separation_radius);

        for (const Boid& other_boid : other_boids)
        {
            if (other_boid.position != position)
            {
                glm::vec3 separation_force = {0, 0, 0};
                glm::vec3 delta            = other_boid.position - position;
                float     distance         = glm::length(delta);

                separation_force = -delta * separation_factor / distance * distance;
                velocity += separation_force;
            }
        }
    }

    void alignment(std::vector<Boid>& boids, float alignment_factor, float alignment_radius)
    {
        std::vector<Boid> other_boids       = boids_in_area(boids, alignment_radius);
        glm::vec3         average_direction = {0, 0, 0};

        for (const Boid& other_boid : other_boids)
        {
            average_direction += glm::normalize(velocity) + glm::normalize(other_boid.velocity);
        }

        if (!other_boids.empty())
        {
            average_direction /= other_boids.size();
            average_direction = glm::normalize(average_direction);
        }

        velocity += average_direction * alignment_factor;
    }

    void cohesion(std::vector<Boid>& boids, float cohesion_factor, float cohesion_radius)
    {
        std::vector<Boid> other_boids      = boids_in_area(boids, cohesion_radius);
        glm::vec3         average_position = {0, 0, 0};

        for (const Boid& other_boid : other_boids)
        {
            average_position += other_boid.position;
        }

        if (!other_boids.empty())
        {
            average_position /= other_boids.size();
        }

        glm::vec3 cohesion_force = (average_position - position) * cohesion_factor;
        velocity += cohesion_force;
    }
};