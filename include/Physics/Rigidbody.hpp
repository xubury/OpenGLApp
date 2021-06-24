#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include "Physics/CollisionObject.hpp"

namespace te {

struct Rigidbody : public CollisionObject {
   public:
    Rigidbody(float mass, bool isKinematic);

    void step(const Time &deltaTime) override;

    void addForce(const glm::vec3 &force, const glm::vec3 &pos);

    void addImpulse(const glm::vec3 &impulse);

    float getMass() const;

    void setMass(float mass);

    float getRestitution() const;

    void setRestitution(float restitution);

    bool isKinematic() const;

    void setKinematic(bool kinematic);

    glm::vec3 getCenterOfMass() const;

    glm::vec3 getCenterOfMassInWorld() const;

    glm::vec3 getVelocity() const;

    void setVelocity(const glm::vec3 &velocity);

    glm::vec3 getAngularVelocity() const;

    void setAngularVelocity(const glm::vec3 &angularVelocity);

    float getStaticFriction() const;

    void setStaticFriction(float friction);

    float getDynamicFriction() const;

    void setDynamicFriction(float friction);

   private:
    glm::vec3 m_centerOfMass;

    glm::vec3 m_force;

    glm::vec3 m_velocity;
    glm::vec3 m_angularVelocity;

    float m_mass;

    float m_restitution;

    float m_staticFriction;

    float m_dynamicFriction;

    bool m_isKinematic;
};

}  // namespace te

#endif /* RIGIDBODY_HPP */
