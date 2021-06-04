#ifndef RENDER_STATES_HPP
#define RENDER_STATES_HPP

#include <glm/glm.hpp>
#include "Graphic/Export.hpp"

class Shader;

class TextureArray;

class CameraBase;

class LightBase;

class GRAPHIC_API RenderStates {
   public:
    RenderStates();

    static const RenderStates Default;

    glm::mat4 transform;
    const TextureArray *textures;
};

#endif
