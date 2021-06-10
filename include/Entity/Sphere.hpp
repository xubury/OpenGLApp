#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Entity/EntityBase.hpp"
#include "Graphic/VertexArray.hpp"

namespace te {

class Sphere : public EntityBase {
   public:
    Sphere(EntityManager<EntityBase> *manager, uint32_t id);

    void draw(const Ref<Shader> &shader,
              const glm::mat4 &transform) const override;

   private:
    Ref<VertexArray> m_sphere;
};

}  // namespace te

#endif /* SPHERE_HPP */
