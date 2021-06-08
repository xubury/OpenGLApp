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

}  // namespace te

#endif /* COLLISION_OBJECT_HPP */
