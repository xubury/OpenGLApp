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

void ModelEntity::draw(const Ref<Shader> &shader, const glm::mat4 &transform) const {
    m_model.draw(shader, transform);
}

}  // namespace te
