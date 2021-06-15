#include "Entity/ModelEntity.hpp"
#include "Component/BoundingBox.hpp"
#include "Core/ResourceManager.hpp"

namespace te {

ResourceManager<std::string, Model> s_loadedModels;

ModelEntity::ModelEntity(EntityManager<EntityBase> *manager, uint32_t id)
    : EntityBase(manager, id) {
    manager->addComponent<BoundingBox>(id);
}

void ModelEntity::loadFromFile(const std::string &path) {
    m_model = s_loadedModels.getOrLoad(path, path);
    for (const auto &mesh : m_model->getMeshes()) {
        component<BoundingBox>()->initialize(mesh.getVertex(), mesh.size());
    }
}

void ModelEntity::draw(const Shader &shader) const {
    m_model->draw(shader, getTransform());
}

}  // namespace te
