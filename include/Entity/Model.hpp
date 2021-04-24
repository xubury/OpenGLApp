#ifndef MODEL_HPP
#define MODEL_HPP

#include <Graphic/ModelResource.hpp>
#include <ECS/Entity.hpp>

class Model : public EntityBase {
   public:
    Model(EntityManager<EntityBase> *manager, uint32_t id);

    void loadFromFile(const std::string &path);

    void draw(RenderTarget &target, RenderStates states) const override;

   private:
    ModelResource m_model;
};

#endif
