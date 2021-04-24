#ifndef CUBE_HPP
#define CUBE_HPP

#include <stdint.h>

#include <Graphic/Drawable.hpp>
#include <Graphic/TextureArray.hpp>
#include <Graphic/VertexBuffer.hpp>
#include <Graphic/ElementBuffer.hpp>
#include <Graphic/BoundingBox.hpp>
#include <ECS/Entity.hpp>

class Cube : public DefaultEntity {
   public:
    Cube(EntityManager<DefaultEntity> *manager, uint32_t id);

    virtual void draw(RenderTarget &target, RenderStates states) const override;

    void setTextures(const TextureArray &textures);

   private:
    static VertexBuffer s_cube;
    TextureArray m_textures;
};

#endif
