#include "Physics/Rigidbody.hpp"

namespace te {

Rigidbody::Rigidbody(float mass, bool isKinematic)
    : CollisionObject(true),
      m_gravity(0, 0, 0),
      m_force(0),
      m_velocity(0),
      m_mass(mass),
      m_isKinematic(isKinematic) {
    TE_ASSERT(mass > 0, "Rigidbody mass can't be less than zero!")
}

void Rigidbody::step(const Time &deltaTime) {
    float dt = deltaTime.count();
    m_force += m_mass * m_gravity;
    m_velocity += m_force / m_mass * dt;
    // TE_TRACE("speed:{0} {1} {2}", m_velocity.x, m_velocity.y,
    // m_velocity.z);
    owner()->component<Transform>()->translateWorld(m_velocity * dt);
    m_force = glm::vec3(0);
}

void Rigidbody::addForce(const glm::vec3 &force, const glm::vec3 &) {
    m_force += force;
}

float Rigidbody::getMass() const { return m_mass; }

bool Rigidbody::isKinematic() const { return m_isKinematic; }

}  // namespace te
