#ifndef RENDER_STATES_HPP
#define RENDER_STATES_HPP

#include "Core/Export.hpp"
#include <glm/glm.hpp>

namespace te {

class Shader;

class TextureArray;

class CameraBase;

class LightBase;

class TE_API RenderStates {
   public:
    RenderStates();

    static const RenderStates Default;

    glm::mat4 transform;
    const TextureArray *textures;
};

}  // namespace te

#endif
