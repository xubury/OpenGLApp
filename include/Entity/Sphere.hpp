#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <Entity/EntityBase.hpp>
#include <Graphic/ElementBuffer.hpp>

class Sphere : public EntityBase {
   public:
    Sphere(EntityManager<EntityBase> *manager, uint32_t id);

    virtual void draw(RenderTarget &target, RenderStates states) const override;

   private:
    ElementBuffer m_shpere;
};

#endif /* SPHERE_HPP */
