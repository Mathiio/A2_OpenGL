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

        // velocity += acceleration / 100.f;
        position += velocity / 100.f;
    }

    //! Ne fonctionne pas
    void drag()
    {
        // Calculer l'amplitude de la vitesse
        float amplitude = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        // Calculer la direction de la vitesse
        float directionX = (amplitude == 0) ? 0 : velocity.x / amplitude;
        float directionY = (amplitude == 0) ? 0 : velocity.y / amplitude;
        // Appliquer la décélération en fonction de la densité et de l'amplitude de la vitesse
        float decelerationFactor = amplitude * amplitude; // * density;
        velocity.x -= directionX * decelerationFactor;
        velocity.y -= directionY * decelerationFactor;
    }

    void separation(Boid& other, float separation_factor, float separation_radius)
    {
        glm::vec2 separation_force = {0, 0};
        glm::vec2 delta            = other.position - position;
        float     distance         = glm::length(delta);

        if (distance < separation_radius)
        {
            separation_force = -delta * separation_factor / distance * distance;
            // separation_force = -delta / distance * (distance / separation_factor);
        }
        velocity += separation_force;
    }
};