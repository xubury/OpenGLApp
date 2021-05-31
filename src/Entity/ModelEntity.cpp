#include <Entity/ModelEntity.hpp>
#include <Component/BoundingBox.hpp>
#include <Component/Transform.hpp>

ModelEntity::ModelEntity(EntityManager<EntityBase> *manager, uint32_t id)
    : EntityBase(manager, id) {
    manager->addComponent<BoundingBox>(id);
}

void ModelEntity::load(const std::string &path) {
    m_model.loadModel(path);
    for (const auto &mesh : m_model.m_meshes) {
        component<BoundingBox>()->initialize(mesh.getVertex(), mesh.size());
    }
}

void ModelEntity::draw(RenderTarget &target, RenderStates states) const {
    states.transform = component<Transform>()->getMatrix();
    target.draw(m_model, states);
}
