#include "boids/boids.hpp"
#include <cmath>
#include "glm/fwd.hpp"
#include "random/random.hpp"

void Boids::initNumberBoid()
{
    for (int i = 0; i < 25; i++)
    {
        if (randBernoulli(randUniform(0.0f, 1.0f)))
        {
            boids.emplace_back();
        }
    }
}

void Boids::update(float delta_time, const Obstacles& obstacles, ContextManager context)
{
    for (auto& boid : boids)
    {
        // Reset moyennes
        glm::vec3 posAvg        = {0, 0, 0};
        glm::vec3 velAvg        = {0, 0, 0};
        glm::vec3 closeD        = {0, 0, 0};
        int       boids_in_area = 0;

        for (auto const& otherBoid : boids)
        {
            if (&boid != &otherBoid)
            {
                // Distance entre deux boids
                const glm::vec3 boids_delta = boid.getPosition() - otherBoid.getPosition();

                if (std::fabs(boids_delta.x) < context.getVisualRange() && std::fabs(boids_delta.y) < context.getVisualRange() && std::fabs(boids_delta.z) < context.getVisualRange())
                {
                    const float boids_distance = glm::length(boids_delta);

                    if (boids_distance < context.getProtectedRange())
                    {
                        closeD += boids_delta / boids_distance * (context.getProtectedRange() - boids_distance);
                    }
                    // Si l'autre boid est dans la plage visuelle
                    else if (boids_distance < context.getVisualRange())
                    {
                        // Ajouter la position et la vitesse à la moyenne
                        posAvg += otherBoid.getPosition();
                        velAvg += otherBoid.getVelocity();

                        boids_in_area++;
                    }
                }
            }
        }

        glm::vec3 alignementForce = {0, 0, 0};
        glm::vec3 cohesionForce   = {0, 0, 0};
        glm::vec3 separationForce = {0, 0, 0};

        if (boids_in_area > 0)
        {
            // Mettre à jour moyennes
            posAvg /= boids_in_area;
            velAvg /= boids_in_area;

            // Différence entre pos moyenne des boids dans l'area et pos boid
            alignementForce = (posAvg - boid.getPosition()) * context.getAlignementFactor();

            // Différence entre vitesse moyenne des boids dans l'area et vitesse boid
            cohesionForce = (velAvg - boid.getVelocity()) * context.getCohesionFactor();
        }
        // Ajouter force d'évitement à vitesse boid
        separationForce = closeD * context.getSeparationFactor();

        boid.setVelocity(boid.getVelocity() + separationForce + alignementForce + cohesionForce);

        const glm::vec3& pos = boid.getPosition();

        // Nouvelle vélocité si boids trop proche des bords sur x,y,z
        auto velocityChange = glm::vec3(0.0f);

        // Update nouvelle vélocité si boids trop proche des bords sur x,y,z et changer vélocité
        velocityChange.x += (pos.x < -0.8f) ? context.getTurnFactor() : (pos.x > 0.8f) ? -context.getTurnFactor()
                                                                                       : 0.0f;
        velocityChange.y += (pos.y < -0.8f) ? context.getTurnFactor() : (pos.y > 0.8f) ? -context.getTurnFactor()
                                                                                       : 0.0f;
        velocityChange.z += (pos.z < -0.8f) ? context.getTurnFactor() : (pos.z > 0.8f) ? -context.getTurnFactor()
                                                                                       : 0.0f;

        velocityChange = obstacles.updateCollision(pos, velocityChange, context.getTurnFactor());

        boid.setVelocity(boid.getVelocity() + velocityChange);

        const float speed = glm::length(boid.getVelocity());

        boid.setVelocity(boid.getVelocity() * ((speed > context.getMaxSpeed()) ? context.getMaxSpeed() / speed : (speed < context.getMinSpeed()) ? context.getMinSpeed() / speed
                                                                                                                                                 : 1.0f));
        boid.update(delta_time);
    }
}

void Boids::draw(GLuint uMVPMatrixLocation, GLuint uMVMatrixLocation, GLuint uNormalMatrixLocation, glm::mat4 ProjMatrix, glm::mat4 viewMatrix, Mesh& mesh) const
{
    for (auto const& boid : boids)
    {
        boid.drawMesh(uMVPMatrixLocation, uMVMatrixLocation, uNormalMatrixLocation, ProjMatrix, viewMatrix, mesh);
    }
}

void Boids::randomRotation()
{
    glm::vec3 const transition(0.1f, 0.5f, 0.4f);

    for (auto& boid : boids)
    {
        float const newRotation = randMarkov(transition, boid.getRotation());
        boid.setRotation(newRotation * 360);
    }
}

void Boids::addBoid(int number)
{
    for (int i = 0; i < number; ++i)
    {
        boids.emplace_back();
    }
}

void Boids::removeBoid(int number)
{
    if (static_cast<int>(boids.size()) >= number)
    {
        for (int i = 0; i < number; ++i)
        {
            boids.pop_back();
        }
    }
}