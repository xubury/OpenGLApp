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
      m_staticFriction(0.5f),
      m_dynamicFriction(0.5f),
      m_isKinematic(isKinematic) {
    TE_ASSERT(mass > 0, "Rigidbody mass can't be less than zero!")
}

void Rigidbody::step(const Time &deltaTime) {
    if (isKinematic()) {
        float dt = deltaTime.count();
        m_velocity += m_force / m_mass * dt;
        // TE_TRACE("speed:{0} {1} {2}", m_velocity.x, m_velocity.y,
        // m_velocity.z);
        owner()->component<Transform>()->translateWorld(m_velocity * dt);
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

float Rigidbody::getRestitution() const { return m_restitution; }

float Rigidbody::getStaticFriction() const { return m_staticFriction; }

float Rigidbody::getDynamicFriction() const { return m_dynamicFriction; }

bool Rigidbody::isKinematic() const { return m_isKinematic; }

glm::vec3 Rigidbody::getCenterOfMass() const { return m_centerOfMass; }

glm::vec3 Rigidbody::getCenterOfMassInWorld() const {
    return owner()->component<Transform>()->getMatrix() *
           glm::vec4(m_centerOfMass, 1.0f);
}

glm::vec3 Rigidbody::getVelocity() const { return m_velocity; }

glm::vec3 Rigidbody::getAngularVelocity() const { return m_angularVelocity; }

}  // namespace te
