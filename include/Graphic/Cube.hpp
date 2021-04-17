#ifndef CUBE_HPP
#define CUBE_HPP

#include <stdint.h>

#include <Graphic/Drawable.hpp>
#include <Graphic/Transformable.hpp>
#include <Graphic/VertexArray.hpp>
#include <Graphic/VertexElement.hpp>

class Cube : public Drawable, public Transformable {
   public:
    Cube();

    virtual void draw(RenderTarget &target, RenderStates states) const override;

   private:
    static VertexArray m_buffer;
    VertexElement m_aabbs;
};

#endif
