#ifndef CUBE_HPP
#define CUBE_HPP

#include <stdint.h>

#include "Drawable.hpp"
#include "Transform.hpp"
#include "VertexBuffer.hpp"

class Cube : public Drawable, public Transform {
   public:
    Cube();

    virtual ~Cube() = default;

    virtual void draw(RenderTarget &target, RenderStates states) const override;

   private:
    static VertexBuffer m_buffer;
};

#endif
