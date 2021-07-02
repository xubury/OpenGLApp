#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include "Physics/CollisionObject.hpp"

namespace te {

struct Rigidbody : public CollisionObject {
   public:
    Rigidbody(float mass, bool isKinematic);

    void step(const Time &deltaTime) override;

    void addForce(const glm::vec3 &force, const glm::vec3 &pos);

    void addTorque(const glm::vec3 &torque);

    void addImpulse(const glm::vec3 &impulse);

    void addAngularImpulse(const glm::vec3 &impulse);

    float getMass() const;

    float getInvMass() const;

    void setMass(float mass);

    glm::mat3 getLocalInertia() const;

    glm::mat3 getLocalInvInertia() const;

    glm::mat3 getGlobalInvInertia() const;

    float getRestitution() const;

    void setRestitution(float restitution);

    bool isKinematic() const;

    void setKinematic(bool kinematic);

    glm::vec3 getCenterOfMass() const;

    void setCenterOfMass(const glm::vec3 &center);

    glm::vec3 getCenterOfMassWorld() const;

    glm::vec3 getVelocity() const;

    void setVelocity(const glm::vec3 &velocity);

    glm::vec3 getAngularVelocity() const;

    void setAngularVelocity(const glm::vec3 &angularVelocity);

    float getStaticFriction() const;

    void setStaticFriction(float friction);

    float getDynamicFriction() const;

    void setDynamicFriction(float friction);

   private:
    glm::vec3 m_localCenterOfMass;

    glm::vec3 m_force;
    glm::vec3 m_velocity;

    glm::vec3 m_torque;
    glm::vec3 m_angularVelocity;

    float m_mass;
    float m_invMass;

    glm::mat3 m_localInertia;
    glm::mat3 m_localInvInertia;

    float m_restitution;

    float m_staticFriction;

    float m_dynamicFriction;

    bool m_isKinematic;
};

}  // namespace te

#endif /* RIGIDBODY_HPP */
