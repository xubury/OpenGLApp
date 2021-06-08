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

    float getRestitution() const;

    bool isKinematic() const;

    glm::vec3 getCenterOfMass() const;

    glm::vec3 getCenterOfMassInWorld() const;

    const glm::vec3 &getVelocity() const;

    const glm::vec3 &getAngularVelocity() const;

   private:
    glm::vec3 m_centerOfMass;

    glm::vec3 m_force;

    glm::vec3 m_velocity;
    glm::vec3 m_angularVelocity;

    float m_mass;

    float m_restitution;

    bool m_isKinematic;
};

}  // namespace te

#endif /* RIGIDBODY_HPP */
