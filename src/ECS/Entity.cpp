#include <ECS/Entity.hpp>
#include <Transform.hpp>

ES_INIT_ENTITY(DefaultEntity)

DefaultEntity::DefaultEntity(EntityManager<DefaultEntity> *manager, uint32_t id)
    : Entity<DefaultEntity>(manager, id) {
    manager->addComponent<Transform>(id);
    m_name = "???";
}

void DefaultEntity::translate(const glm::vec3 &pos) {
    component<Transform>()->translate(pos);
}
