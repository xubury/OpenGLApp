#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

class RenderTarget;

#include "Graphic/Export.hpp"
#include "Graphic/RenderStates.hpp"

class GRAPHIC_API Drawable {
   public:
    virtual ~Drawable() = default;

    virtual void draw(RenderTarget &target, RenderStates states) const = 0;
};

#endif
