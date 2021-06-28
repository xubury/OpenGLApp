#include "Entity/EntityBase.hpp"

namespace te {

ES_INIT_ENTITY(EntityBase)

EntityBase::EntityBase(EntityManager<EntityBase> *manager, uint32_t id)
    : Entity<EntityBase>(manager, id) {
    m_name = "???";
}

void EntityBase::setName(const std::string &name) { m_name = name; }

std::string EntityBase::getName() const { return m_name; }

void EntityBase::draw(const Shader &, const glm::mat4 &) const {}

}  // namespace te
