#include "obstacles/obstacles.hpp"
#include <iostream>

void Obstacles::addObstacle(const Obstacle& obstacle)
{
    m_obstacles.push_back(obstacle);
}

glm::vec3 Obstacles::updateCollision(const glm::vec3& pos, const glm::vec3& velocityChange, float turnFactor) const
{
    glm::vec3 adjustedVelocityChange = velocityChange;

    for (const auto& obstacle : m_obstacles)
    {
        const float      delta       = 0.06f;
        const float      xLength     = obstacle.getXLength();
        const float      yLength     = obstacle.getYLength();
        const float      zLength     = obstacle.getZLength();
        const glm::vec3& obstaclePos = obstacle.getPosition();
        glm::vec3        minBounds   = obstaclePos - glm::vec3(xLength / 2.0f, yLength / 2.0f, zLength / 2.0f);
        glm::vec3        maxBounds   = obstaclePos + glm::vec3(xLength / 2.0f, yLength / 2.0f, zLength / 2.0f);

        if (minBounds.x - delta < pos.x && pos.x < maxBounds.x + delta &&
            minBounds.y - delta < pos.y && pos.y < maxBounds.y + delta &&
            minBounds.z - delta < pos.z && pos.z < maxBounds.z + delta
            )
        {
            if (minBounds.x - delta < pos.x && pos.x < minBounds.x + delta){
                adjustedVelocityChange.x += (obstaclePos.x < minBounds.x) ? turnFactor : -turnFactor;
            }
            if (maxBounds.x - delta < pos.x && pos.x < maxBounds.x + delta){
                adjustedVelocityChange.x += (obstaclePos.x < maxBounds.x) ? turnFactor : -turnFactor;
            }

            if (minBounds.y - delta < pos.y && pos.y < minBounds.y + delta){
                adjustedVelocityChange.y += (obstaclePos.y < minBounds.y) ? turnFactor : -turnFactor;
            }
            if (maxBounds.y - delta < pos.y && pos.y < maxBounds.y + delta){
                adjustedVelocityChange.y += (obstaclePos.y < maxBounds.y) ? turnFactor : -turnFactor;
            }

            if (minBounds.z - delta < pos.z && pos.z < minBounds.z + delta){
                adjustedVelocityChange.z += (obstaclePos.z < minBounds.z) ? turnFactor : -turnFactor;
            }
            if (maxBounds.z - delta < pos.z && pos.z < maxBounds.z + delta){
                adjustedVelocityChange.z += (obstaclePos.z < maxBounds.z) ? turnFactor : -turnFactor;
            }
        }
    }

    return adjustedVelocityChange;
}
