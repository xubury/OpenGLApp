#include "Physics/Rigidbody.hpp"
#include <glm/gtx/quaternion.hpp>

namespace te {

Rigidbody::Rigidbody(float mass, bool isKinematic)
    : CollisionObject(true),
      m_localCenterOfMass(0.f),
      m_force(0.f),
      m_velocity(0.f),
      m_torque(0.f),
      m_angularVelocity(0.f),
      m_mass(mass),
      m_invMass(1.f / mass),
      m_localInertia(20.f),
      m_localInvInertia(1 / 20.f),
      m_restitution(0.5f),
      m_staticFriction(0.5f),
      m_dynamicFriction(0.5f),
      m_isKinematic(isKinematic) {
    TE_ASSERT(mass > 0, "Rigidbody mass can't be less than zero!")
}

void Rigidbody::step(const Time &deltaTime) {
    if (isKinematic()) {
        float dt = deltaTime.count();
        m_velocity += m_force * m_invMass * dt;
        glm::mat3 rotation = glm::toMat3(owner()->getRotation());
        m_angularVelocity += rotation * m_localInvInertia *
                             glm::transpose(rotation) * m_torque * dt;

        owner()->translateWorld(m_velocity * dt);
        if (glm::length(m_angularVelocity) > 0) {
            glm::quat rot = glm::angleAxis(glm::length(m_angularVelocity) * dt,
                                           glm::normalize(m_angularVelocity));
            owner()->rotateWorld(rot);
        }
        m_force = glm::vec3(0);
        m_torque = glm::vec3(0);
    }
}

void Rigidbody::addForce(const glm::vec3 &force, const glm::vec3 &pos) {
    m_force += force;
    addTorque(
        glm::cross(owner()->toWorldVector(pos - getCenterOfMass()), force));
}

void Rigidbody::addTorque(const glm::vec3 &torque) { m_torque += torque; }

void Rigidbody::addImpulse(const glm::vec3 &impulse) {
    m_velocity += impulse * m_invMass;
}

void Rigidbody::addAngularImpulse(const glm::vec3 &impulse) {
    m_angularVelocity += getGlobalInvInertia() * impulse;
}

float Rigidbody::getMass() const { return m_mass; }

float Rigidbody::getInvMass() const { return m_invMass; }

void Rigidbody::setMass(float mass) {
    m_mass = mass;
    m_invMass = 1.f / mass;
}

glm::mat3 Rigidbody::getLocalInertia() const { return m_localInertia; }

glm::mat3 Rigidbody::getLocalInvInertia() const { return m_localInvInertia; }

glm::mat3 Rigidbody::getGlobalInvInertia() const {
    glm::mat3 rotation = glm::toMat3(owner()->getRotation());
    return rotation * m_localInvInertia * glm::transpose(rotation);
}

float Rigidbody::getRestitution() const { return m_restitution; }

void Rigidbody::setRestitution(float restitution) {
    m_restitution = restitution;
}

float Rigidbody::getStaticFriction() const { return m_staticFriction; }

void Rigidbody::setStaticFriction(float friction) {
    m_staticFriction = friction;
}

float Rigidbody::getDynamicFriction() const { return m_dynamicFriction; }

void Rigidbody::setDynamicFriction(float friction) {
    m_dynamicFriction = friction;
}

bool Rigidbody::isKinematic() const { return m_isKinematic; }

void Rigidbody::setKinematic(bool kinematic) { m_isKinematic = kinematic; }

glm::vec3 Rigidbody::getCenterOfMass() const { return m_localCenterOfMass; }

void Rigidbody::setCenterOfMass(const glm::vec3 &center) {
    m_localCenterOfMass = center;
}

glm::vec3 Rigidbody::getCenterOfMassWorld() const {
    return owner()->toWorldSpace(m_localCenterOfMass);
}

glm::vec3 Rigidbody::getVelocity() const { return m_velocity; }

void Rigidbody::setVelocity(const glm::vec3 &velocity) {
    m_velocity = velocity;
}

glm::vec3 Rigidbody::getAngularVelocity() const { return m_angularVelocity; }

void Rigidbody::setAngularVelocity(const glm::vec3 &angularVelocity) {
    m_angularVelocity = angularVelocity;
}

}  // namespace te
