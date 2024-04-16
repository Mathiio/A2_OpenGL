#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include <functional>

class Obstacle {
public:
    Obstacle(const glm::vec3& position, float xLength, float yLength, float zLength)
        : m_position(position), m_xLength(xLength), m_yLength(yLength), m_zLength(zLength) {}

    const glm::vec3& getPosition() const { return m_position; }
    float getXLength() const { return m_xLength; }
    float getYLength() const { return m_yLength; }
    float getZLength() const { return m_zLength; }

private:
    glm::vec3 m_position;
    float m_xLength;
    float m_yLength;
    float m_zLength;
};

class Obstacles {
public:
    void addObstacle(const Obstacle& object);
    glm::vec3 updateCollision(const glm::vec3& pos, const glm::vec3& velocityChange, float turnFactor) const;

    auto begin() const { return m_obstacles.begin(); }
    auto end() const { return m_obstacles.end(); }

private:
    std::vector<Obstacle> m_obstacles;
};