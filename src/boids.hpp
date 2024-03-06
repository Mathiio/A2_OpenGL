#pragma once
#include <vector>
#include "p6/p6.h"

class Boid {
private:
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec2 acceleration;
    float     radius;

public:
    Boid()
        : position({p6::random::number(-0.75f + 0.02f, 0.75f - 0.02f), p6::random::number(-0.75f + 0.02f, 0.75f - 0.02f)})
        , velocity({p6::random::number(-1.0f, 1.0f), p6::random::number(-1.0f, 1.0f)})
        , acceleration({p6::random::number(-1.0f, 1.0f), p6::random::number(-1.0f, 1.0f)})
        , radius(0.02f) {}

    Boid(const glm::vec2& init_position, const glm::vec2& init_velocity)
        : position(init_position), velocity(init_velocity), acceleration(1.0f, 1.0f), radius(0.02f) {}

    glm::vec2 get_position() const { return position; }
    float     get_radius() const { return radius; }
    void      set_radius(float new_radius) { radius = new_radius; }
    // void               set_position(const std::vector<float>& new_pos) { position = new_pos; }

    //  glm::vec2  get_velocity() const { return velocity; }
    // void               set_velocity(const std::vector<float>& new_vel) { velocity = new_vel; }

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

        //! Pas terrible
        float maxSpeed = 1.0f;
        float speed    = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        if (speed > maxSpeed)
        {
            velocity.x = velocity.x * maxSpeed / speed;
            velocity.y = velocity.y * maxSpeed / speed;
        }
        position += velocity / 100.f;
    }

    std::vector<Boid> boids_in_area(std::vector<Boid>& boids, float radius)
    {
        std::vector<Boid> boids_in_area;
        for (const Boid& boid : boids)
        {
            glm::vec2 delta    = boid.position - position;
            float     distance = glm::length(delta);
            if (distance < radius)
            {
                boids_in_area.push_back(boid);
            }
        }
        return boids_in_area;
    }

    void separation(Boid& other, float separation_factor, float separation_radius)
    {
        glm::vec2 separation_force = {0, 0};
        glm::vec2 delta            = other.position - position;
        float     distance         = glm::length(delta);

        if (distance < separation_radius)
        {
            separation_force = -delta * separation_factor / distance * distance;
        }
        velocity += separation_force;
    }

    void alignment(std::vector<Boid>& boids, float alignment_factor, float alignment_radius)
    {
        std::vector<Boid> other_boids       = boids_in_area(boids, alignment_radius);
        glm::vec2         average_direction = {0, 0};

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
        std::vector<Boid> other_boids       = boids_in_area(boids, cohesion_radius);
        glm::vec2         average_position  = {0, 0};

        for (const Boid& other_boid : other_boids)
        {
            average_position += other_boid.position;
        }

        if (!other_boids.empty())
        {
            average_position /= other_boids.size();
        }

        glm::vec2 cohesion_force = (average_position - position) * cohesion_factor;
        velocity += cohesion_force;
    }
};