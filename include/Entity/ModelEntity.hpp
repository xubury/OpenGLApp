#ifndef MODEL_ENTITY_HPP
#define MODEL_ENTITY_HPP

#include <Entity/EntityBase.hpp>
#include <Graphic/Model.hpp>

class ModelEntity : public EntityBase {
   public:
    ModelEntity(EntityManager<EntityBase> *manager, uint32_t id);

    void load(const std::string &path);

    void draw(RenderTarget &target, RenderStates states) const override;

   private:
    Model m_model;
};

#endif
