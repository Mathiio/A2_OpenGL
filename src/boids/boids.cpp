#include "boids/boids.hpp"
#include <cmath>
#include <ostream>
#include "glm/fwd.hpp"


Boids::Boids(int nbBoids)
    : boids(nbBoids), numBoids(nbBoids)
{
}

void Boids::update(float delta_time, const Obstacles& obstacles)
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

                if (std::fabs(boids_delta.x) < visualRange && std::fabs(boids_delta.y) < visualRange && std::fabs(boids_delta.z) < visualRange)
                {
                    const float boids_distance = glm::length(boids_delta);

                    if (boids_distance < protectedRange)
                    {
                        closeD += boids_delta / boids_distance * (protectedRange - boids_distance);
                    }
                    // Si l'autre boid est dans la plage visuelle
                    else if (boids_distance < visualRange)
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
            alignementForce = (posAvg - boid.getPosition()) * cohesionFactor;

            // Différence entre vitesse moyenne des boids dans l'area et vitesse boid
            cohesionForce = (velAvg - boid.getVelocity()) * alignementFactor;
        }
        // Ajouter force d'évitement à vitesse boid
        separationForce = closeD * separationFactor;

        boid.setVelocity(boid.getVelocity() + separationForce + alignementForce + cohesionForce);

        const glm::vec3& pos = boid.getPosition();

        // Nouvelle vélocité si boids trop proche des bords sur x,y,z
        glm::vec3 velocityChange = glm::vec3(0.0f);

        // Update nouvelle vélocité si boids trop proche des bords sur x,y,z et changer vélocité
        velocityChange.x += (pos.x < -0.8f) ? turnFactor : (pos.x > 0.8f) ? -turnFactor : 0.0f;
        velocityChange.y += (pos.y < -0.8f) ? turnFactor : (pos.y > 0.8f) ? -turnFactor : 0.0f;
        velocityChange.z += (pos.z < -0.8f) ? turnFactor : (pos.z > 0.8f) ? -turnFactor : 0.0f;

        velocityChange = obstacles.updateCollision(pos, velocityChange, turnFactor);

        boid.setVelocity(boid.getVelocity() + velocityChange);

        const float speed = glm::length(boid.getVelocity());

        boid.setVelocity(boid.getVelocity() * ((speed > maxSpeed) ? maxSpeed / speed : (speed < minSpeed) ? minSpeed / speed : 1.0f));
        boid.update(delta_time);
    }
}

void Boids::draw(GLuint uMVPMatrixLocation, GLuint uMVMatrixLocation, GLuint uNormalMatrixLocation, glm::mat4 ProjMatrix, glm::mat4 viewMatrix, Mesh mesh) const
{
    for (auto const& boid : boids)
    {
        boid.drawMesh(uMVPMatrixLocation, uMVMatrixLocation, uNormalMatrixLocation, ProjMatrix, viewMatrix, mesh);
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

void Boids::helper()
{
    ImGui::Begin("Controls");
    ImGui::Text("Number of boids %zu", boids.size());
    ImGui::SliderFloat("Turn factor", &turnFactor, .001f, .5f);
    ImGui::SliderFloat("Visual range", &visualRange, .001f, .5f);
    ImGui::SliderFloat("Protected range", &protectedRange, .001f, .5f);
    ImGui::SliderFloat("Cohesion factor", &cohesionFactor, .001f, .5f);
    ImGui::SliderFloat("Separation factor", &separationFactor, .001f, 1.f);
    ImGui::SliderFloat("Alignement factor", &alignementFactor, .001f, 1.f);
    ImGui::SliderFloat("Max speed", &maxSpeed, .001f, 1.f);
    ImGui::SliderFloat("Min speed", &minSpeed, .001f, 1.f);
    ImGui::InputInt("Number of boids wanted", &numBoids);
    if (ImGui::Button("Apply"))
    {
        if (numBoids != static_cast<int>(boids.size()))
        {
            if (numBoids > 25)
            {
                numBoids = 25;
            }

            if (numBoids > static_cast<int>(boids.size()))
            {
                addBoid(numBoids - boids.size());
            }
            else
            {
                removeBoid(boids.size() - numBoids);
            }
        }
    }
    ImGui::End();
}