#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include "Core/Export.hpp"
#include "Graphic/Shader.hpp"

namespace te {

class TE_API Drawable {
   public:
    virtual void draw(const Shader &shader,
                      const glm::mat4 &transform) const = 0;
};

}  // namespace te

#endif
