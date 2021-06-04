#ifndef CUBE_HPP
#define CUBE_HPP

#include <Graphic/VertexBuffer.hpp>
#include <Entity/EntityBase.hpp>

namespace te {

class Cube : public EntityBase {
   public:
    Cube(EntityManager<EntityBase> *manager, uint32_t id, float width,
         float height, float length);

    virtual void draw(RenderTarget &target, RenderStates states) const override;

   private:
    VertexBuffer m_cube;
};

}  // namespace te

#endif
