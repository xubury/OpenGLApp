#ifndef CUBE_HPP
#define CUBE_HPP

#include "Entity/EntityBase.hpp"
#include "Graphic/VertexArray.hpp"
#include "Graphic/Material.hpp"

namespace te {

class Cube : public EntityBase {
   public:
    Cube(EntityManager<EntityBase> *manager, uint32_t id, float width,
         float height, float length, Ref<Material> material);

    void draw(const Shader &shader, const glm::mat4 &transform) const override;

   private:
    Ref<VertexArray> m_cube;

    Ref<Material> m_material;
};

}  // namespace te

#endif
