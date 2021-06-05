#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP


#include "Core/Export.hpp"
#include "Graphic/RenderStates.hpp"

namespace te {

class RenderTarget;

class TE_API Drawable {
   public:
    virtual ~Drawable() = default;

    virtual void draw(RenderTarget &target, RenderStates states) const = 0;
};

}  // namespace te

#endif
