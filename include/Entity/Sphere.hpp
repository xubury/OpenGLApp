#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Entity/EntityBase.hpp"
#include "Graphic/VertexArray.hpp"
#include "Graphic/ModelTextures.hpp"

namespace te {

class Sphere : public EntityBase {
   public:
    Sphere(EntityManager<EntityBase> *manager, uint32_t id, float radius,
           const ModelTextures &textures);

    void draw(const Ref<Shader> &shader) const override;

   private:
    Ref<VertexArray> m_sphere;
    ModelTextures m_textures;
};

}  // namespace te

#endif /* SPHERE_HPP */
