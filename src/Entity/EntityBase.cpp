#include <Entity/EntityBase.hpp>
#include <Component/Transform.hpp>

ES_INIT_ENTITY(EntityBase)

EntityBase::EntityBase(EntityManager<EntityBase> *manager, uint32_t id)
    : Entity<EntityBase>(manager, id) {
    manager->addComponent<Transform>(id);
    m_name = "???";
}

void EntityBase::setPosition(const glm::vec3 &position) {
    component<Transform>()->setPosition(position);
}

glm::vec3 EntityBase::getPosition() const {
    return component<Transform>()->getPosition();
}

void EntityBase::setName(const std::string &name) { m_name = name; }

std::string EntityBase::getName() const { return m_name; }
