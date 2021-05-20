#include <Entity/Model.hpp>
#include <Component/BoundingBox.hpp>
#include <Component/Transform.hpp>

Model::Model(EntityManager<EntityBase> *manager, uint32_t id)
    : EntityBase(manager, id) {
    manager->addComponent<BoundingBox>(id);
}

void Model::loadFromFile(const std::string &path) {
    m_model.loadModel(path);
    for (const auto &mesh : m_model.m_meshes) {
        component<BoundingBox>()->initialize(mesh.getVertex(), mesh.size());
    }
}

void Model::draw(RenderTarget &target, RenderStates states) const {
    states.transform = component<TransformComp>()->getTransform();
    target.draw(m_model, states);
}
