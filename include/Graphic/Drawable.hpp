#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

class RenderTarget;

#include <Graphic/RenderStates.hpp>

class Drawable {
   public:
    virtual void draw(RenderTarget &target, RenderStates states) const = 0;

    virtual ~Drawable() = default;
};

#endif
