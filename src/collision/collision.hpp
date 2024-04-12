#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include <functional>

class CollisionObject {
public:
    CollisionObject(const glm::vec3& position, float xLength, float yLength, float zLength, float avoidanceFactor)
        : m_position(position), m_xLength(xLength), m_yLength(yLength), m_zLength(zLength), m_avoidanceFactor(avoidanceFactor) {}

    bool isInCollisionRange(const glm::vec3& point) const;

    const glm::vec3& getPosition() const { return m_position; }
    float getAvoidanceFactor() const { return m_avoidanceFactor; }

private:
    glm::vec3 m_position;
    float m_xLength;
    float m_yLength;
    float m_zLength;
    float m_avoidanceFactor;
};

class CollisionObjects {
public:
    void addCollisionObject(const CollisionObject& object);
    bool isInCollisionRange(const glm::vec3& point) const;

    auto begin() const { return m_collisionObjects.begin(); }
    auto end() const { return m_collisionObjects.end(); }

private:
    std::vector<CollisionObject> m_collisionObjects;
};