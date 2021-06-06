#include <Entity/ModelEntity.hpp>
#include <Component/BoundingBox.hpp>
#include <Component/Transform.hpp>

namespace te {

ModelEntity::ModelEntity(EntityManager<EntityBase> *manager, uint32_t id)
    : EntityBase(manager, id) {
    manager->addComponent<BoundingBox>(id);
}

void ModelEntity::load(const std::string &path) {
    m_model.loadModel(path);
    for (const auto &mesh : m_model.getMeshes()) {
        component<BoundingBox>()->initialize(mesh.getVertex(), mesh.size());
    }
}

void ModelEntity::draw(RenderTarget &target, RenderStates states) const {
    states.transform = component<Transform>()->getMatrix();
    m_model.draw(target, states);
}

}  // namespace te
