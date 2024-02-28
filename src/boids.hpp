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
        float maxSpeed = 5.0;
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

    void repulsion(Boid& other)
    {
        float dx       = other.position.x - position.x;
        float dy       = other.position.y - position.y;
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance < 0.2f)
        { // Distance de répulsion arbitraire
            // Appliquer une force de répulsion inversement proportionnelle à la distance
            velocity.x -= dx / (distance * distance);
            velocity.y -= dy / (distance * distance);
        }
        // float distance     = glm::distance(position, other.get_position());
        // float min_distance = 0.1f;
        // float max_force    = 0.1f; // Limite de la force de répulsion

        // if (distance < min_distance)
        // {
        //     glm::vec2 direction       = glm::normalize(position - other.get_position());
        //     glm::vec2 repulsion_force = direction * max_force; // Limiter la force de répulsion
        //     velocity += repulsion_force;                       // Ajouter la force de répulsion à la vitesse
        //     other.velocity += repulsion_force;                 // Ajouter la force de répulsion à la vitesse
        // }

        // glm::vec2 repulsion;
        // float     perceptionRadius = 0.5f;

        // float d = glm::distance(position, other.position);

        // if (d < perceptionRadius && d > 0)
        // {
        //     glm::vec2 diff = position - other.position;
        //     diff /= (d * d);
        //     repulsion += diff;
        // }

        // acceleration += repulsion;
    }
};