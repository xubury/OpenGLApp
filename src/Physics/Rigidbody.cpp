#include "Physics/Rigidbody.hpp"

namespace te {

Rigidbody::Rigidbody(float mass, bool isKinematic)
    : CollisionObject(true),
      m_centerOfMass(0.f),
      m_force(0.f),
      m_velocity(0.f),
      m_angularVelocity(0.f),
      m_mass(mass),
      m_restitution(0.5f),
      m_staticFriction(0.2f),
      m_dynamicFriction(0.2f),
      m_isKinematic(isKinematic) {
    TE_ASSERT(mass > 0, "Rigidbody mass can't be less than zero!")
}

void Rigidbody::step(const Time &deltaTime) {
    if (isKinematic()) {
        float dt = deltaTime.count();
        m_velocity += m_force / m_mass * dt;
        // TE_TRACE("speed:{0} {1} {2}", m_velocity.x, m_velocity.y,
        // m_velocity.z);
        owner()->translateWorld(m_velocity * dt);
        m_force = glm::vec3(0);
    }
}

void Rigidbody::addForce(const glm::vec3 &force, const glm::vec3 &) {
    m_force += force;
}

void Rigidbody::addImpulse(const glm::vec3 &impulse) {
    m_velocity += impulse / m_mass;
}

float Rigidbody::getMass() const { return m_mass; }

void Rigidbody::setMass(float mass) { m_mass = mass; }

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

glm::vec3 Rigidbody::getCenterOfMass() const { return m_centerOfMass; }

glm::vec3 Rigidbody::getCenterOfMassInWorld() const {
    return owner()->getTransform() * glm::vec4(m_centerOfMass, 1.0f);
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
