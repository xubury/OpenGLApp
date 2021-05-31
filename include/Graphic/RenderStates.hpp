#ifndef RENDER_STATES_HPP
#define RENDER_STATES_HPP

#include <Graphic/Export.hpp>
#include <glm/glm.hpp>

class Shader;

class TextureArray;

class CameraBase;

class LightBase;

class GRAPHIC_API RenderStates {
   public:
    RenderStates();

    static const RenderStates Default;

    const Shader *shader;
    glm::mat4 transform;
    const TextureArray *textures;
    const CameraBase *camera;
    const LightBase *light;
    bool depthMapDraw;
    uint32_t depthMapTexture;
};

#endif
