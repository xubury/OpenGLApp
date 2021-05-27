#ifndef MODEL_ENTITY_HPP
#define MODEL_ENTITY_HPP

#include <Graphic/Model.hpp>
#include <Entity/EntityBase.hpp>

class ModelEntity : public EntityBase {
   public:
    ModelEntity(EntityManager<EntityBase> *manager, uint32_t id);

    void loadFromFile(const std::string &path);

    void draw(RenderTarget &target, RenderStates states) const override;

   private:
    Model m_model;
};

#endif
