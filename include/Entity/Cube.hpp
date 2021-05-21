#ifndef CUBE_HPP
#define CUBE_HPP

#include <stdint.h>

#include <Graphic/Drawable.hpp>
#include <Graphic/TextureArray.hpp>
#include <Graphic/VertexBuffer.hpp>
#include <Graphic/ElementBuffer.hpp>
#include <Entity/EntityBase.hpp>

class Cube : public EntityBase {
   public:
    Cube(EntityManager<EntityBase> *manager, uint32_t id);

    virtual void draw(RenderTarget &target, RenderStates states) const override;

    void setTextures(const TextureArray &textures);

   private:
    static VertexBuffer s_cube;
    TextureArray m_textures;
};

#endif
