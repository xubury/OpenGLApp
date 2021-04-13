#ifndef CUBE_HPP
#define CUBE_HPP

#include <stdint.h>

#include "Drawable.hpp"
#include "Transformable.hpp"
#include "VertexBuffer.hpp"

class Cube : public Drawable, public Transformable {
   public:
    Cube();
    virtual ~Cube() = default;

    virtual void draw(RenderTarget &target, RenderStates states) const override;

   private:
    static VertexBuffer m_buffer;
};

#endif
