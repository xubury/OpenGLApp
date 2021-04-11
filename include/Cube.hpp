#ifndef CUBE_HPP
#define CUBE_HPP

#include <stdint.h>

#include "Drawable.hpp"
#include "VertexBuffer.hpp"

class Cube : public Drawable {
   public:
    Cube();

    virtual ~Cube() = default;

    virtual void draw(RenderTarget &target) const override;

   private:
    VertexBuffer m_buffer;
};

#endif
