#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include "Physics/CollisionObject.hpp"
#include "Component/Transform.hpp"

namespace te {

struct Rigidbody : public CollisionObject {
   public:
    Rigidbody(float mass, bool isKinematic);

    void step(const Time &deltaTime) override;

    void addForce(const glm::vec3 &force, const glm::vec3 &pos);

    void addImpulse(const glm::vec3 &impulse);

    float getMass() const;

    bool isKinematic() const;

   private:
    glm::vec3 m_gravity;
    glm::vec3 m_force;
    glm::vec3 m_velocity;

    float m_mass;
    bool m_isKinematic;
};

}  // namespace te

#endif /* RIGIDBODY_HPP */
