#ifndef MODEL_ENTITY_HPP
#define MODEL_ENTITY_HPP

#include "Entity/EntityBase.hpp"
#include "Model/Model.hpp"

namespace te {

class ModelEntity : public EntityBase {
   public:
    ModelEntity(EntityManager<EntityBase> *manager, uint32_t id);

    void loadFromFile(const std::string &path);

    void draw(const Shader &shader) const override;

   private:
    Ref<Model> m_model;
};

}  // namespace te

#endif
