#pragma once
#include <algorithm>
#include <functional>
#include <glm/glm.hpp>
#include <vector>

class Obstacle {
public:
    Obstacle(const glm::vec3& position, float xLength, float yLength, float zLength)
        : m_position(position), m_xLength(xLength), m_yLength(yLength), m_zLength(zLength) {}

    bool             isCollision(const glm::vec3& pos, double delta) const;
    const glm::vec3& getPosition() const { return m_position; }
    float            getXLength() const { return m_xLength; }
    float            getYLength() const { return m_yLength; }
    float            getZLength() const { return m_zLength; }

private:
    glm::vec3 m_position;
    float     m_xLength;
    float     m_yLength;
    float     m_zLength;
};

class Obstacles {
public:
    void      addObstacle(const Obstacle& object);
    bool      isCollision(const glm::vec3& pos, double delta);
    glm::vec3 updateCollision(const glm::vec3& pos, const glm::vec3& velocityChange, float turnFactor) const;

    auto begin() const { return m_obstacles.begin(); }
    auto end() const { return m_obstacles.end(); }

private:
    std::vector<Obstacle> m_obstacles;
};