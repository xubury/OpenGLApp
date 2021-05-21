#include <ECS/Entity.hpp>
#include <Component/Transform.hpp>

ES_INIT_ENTITY(EntityBase)

EntityBase::EntityBase(EntityManager<EntityBase> *manager, uint32_t id)
    : Entity<EntityBase>(manager, id) {
    manager->addComponent<Transform>(id);
    m_name = "???";
}

glm::vec3 EntityBase::getPosition() const {
    return component<Transform>()->getPosition();
}

