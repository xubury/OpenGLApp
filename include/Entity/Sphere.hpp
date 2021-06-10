#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Entity/EntityBase.hpp"
#include "Graphic/VertexArray.hpp"

namespace te {

class Sphere : public EntityBase {
   public:
    Sphere(EntityManager<EntityBase> *manager, uint32_t id, float radius,
           const TextureArray &textures);

    void draw(const Ref<Shader> &shader,
              const glm::mat4 &transform) const override;

   private:
    Ref<VertexArray> m_sphere;
    TextureArray m_textures;
};

}  // namespace te

#endif /* SPHERE_HPP */
