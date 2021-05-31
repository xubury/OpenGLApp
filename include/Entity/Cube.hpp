#ifndef CUBE_HPP
#define CUBE_HPP

#include <Graphic/TextureArray.hpp>
#include <Graphic/VertexBuffer.hpp>
#include <Entity/EntityBase.hpp>

class Cube : public EntityBase {
   public:
    Cube(EntityManager<EntityBase> *manager, uint32_t id);

    virtual void draw(RenderTarget &target, RenderStates states) const override;

   private:
    static VertexBuffer s_cube;
};

#endif
