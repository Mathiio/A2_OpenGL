#include "collision/collision.hpp"
#include <iostream>



void CollisionObjects::addCollisionObject(const CollisionObject& object) {
    m_collisionObjects.push_back(object);
}

bool CollisionObjects::isInCollisionRange(const glm::vec3& point) const {
    for (const auto& object : m_collisionObjects) {
        std::cout << "inside" << std::endl;
        if (object.isInCollisionRange(point)) {
            return true;
        }
    }
    return false;
}

bool CollisionObject::isInCollisionRange(const glm::vec3& point) const {
    glm::vec3 minBounds = m_position - glm::vec3(m_xLength / 2.0f, m_yLength / 2.0f, m_zLength / 2.0f);
    glm::vec3 maxBounds = m_position + glm::vec3(m_xLength / 2.0f, m_yLength / 2.0f, m_zLength / 2.0f);

    return point.x >= minBounds.x && point.x <= maxBounds.x &&
            point.y >= minBounds.y && point.y <= maxBounds.y &&
            point.z >= minBounds.z && point.z <= maxBounds.z;
}