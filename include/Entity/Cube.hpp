#ifndef CUBE_HPP
#define CUBE_HPP

#include <Graphic/VertexBuffer.hpp>
#include <Entity/EntityBase.hpp>

class Cube : public EntityBase {
   public:
    Cube(EntityManager<EntityBase> *manager, uint32_t id, float width,
         float height, float length);

    virtual void draw(RenderTarget &target, RenderStates states) const override;

   private:
    VertexBuffer s_cube;
};

#endif
