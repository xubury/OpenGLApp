#ifndef CUBE_HPP
#define CUBE_HPP

#include "Entity/EntityBase.hpp"
#include "Graphic/VertexArray.hpp"

namespace te {

class Cube : public EntityBase {
   public:
    Cube(EntityManager<EntityBase> *manager, uint32_t id, float width,
         float height, float length, const TextureArray &textures);

    void draw(const Ref<Shader> &shader,
              const glm::mat4 &transform) const override;

   private:
    Ref<VertexArray> m_cube;

    TextureArray m_textures;
};

}  // namespace te

#endif
