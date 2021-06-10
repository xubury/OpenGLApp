#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include "Core/Export.hpp"
#include "Graphic/Renderer.hpp"

namespace te {

class RenderTarget;

class TE_API Drawable {
   public:
    virtual void draw(const Ref<Shader> &shader,
                      const glm::mat4 &transform) const = 0;
};

}  // namespace te

#endif
