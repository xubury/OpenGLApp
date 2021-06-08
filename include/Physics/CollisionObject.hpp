#ifndef COLLISION_OBJECT_HPP
#define COLLISION_OBJECT_HPP

#include <ECS/ECS.hpp>

namespace te {

struct CollisionObject : Component<CollisionObject, EntityBase> {
   public:
    CollisionObject(bool isDynamic)
        : m_isDynamic(isDynamic), m_isTrigger(false) {}

    bool isDynamic() const { return m_isDynamic; }

    bool isTrigger() const { return m_isTrigger; }

    virtual void step(const Time &deltaTime) = 0;

   protected:
    bool m_isDynamic;
    bool m_isTrigger;
};

struct ContactPoint {
    glm::vec3 position;
    float depth;
    ContactPoint() : position(0), depth(0) {}
};

struct ContactManifold {
    CollisionObject *objA;
    CollisionObject *objB;
    uint8_t pointCount;
    ContactPoint points[4];
    glm::vec3 normal;

    ContactManifold()
        : objA(nullptr), objB(nullptr), pointCount(0), points(), normal(0) {}

    bool hasCollision() const { return pointCount > 0; }
};

}  // namespace te

#endif /* COLLISION_OBJECT_HPP */
