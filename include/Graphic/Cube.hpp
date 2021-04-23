#ifndef CUBE_HPP
#define CUBE_HPP

#include <stdint.h>

#include <Graphic/Drawable.hpp>
#include <Graphic/TextureArray.hpp>
#include <Graphic/Transformable.hpp>
#include <Graphic/VertexBuffer.hpp>
#include <Graphic/ElementBuffer.hpp>
#include <Graphic/BoundingBox.hpp>
#include <Graphic/Axis.hpp>

class Cube : public Drawable, public Transformable {
   public:
    Cube();

    virtual void draw(RenderTarget &target, RenderStates states) const override;

    BoundingBox m_aabb;

    void setTextures(const TextureArray &textures);

   private:
    static VertexBuffer s_cube;
    TextureArray m_textures;
    Axis m_axis;
};

#endif
