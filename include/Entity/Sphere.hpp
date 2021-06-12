#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Entity/EntityBase.hpp"
#include "Graphic/VertexArray.hpp"
#include "Graphic/Material.hpp"

namespace te {

class Sphere : public EntityBase {
   public:
    Sphere(EntityManager<EntityBase> *manager, uint32_t id, float radius,
           Ref<Material> material);

    void draw(const Ref<Shader> &shader) const override;

   private:
    Ref<VertexArray> m_sphere;
    Ref<Material> m_material;
};

}  // namespace te

#endif /* SPHERE_HPP */
