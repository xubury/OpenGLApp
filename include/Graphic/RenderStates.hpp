#ifndef RENDER_STATES_HPP
#define RENDER_STATES_HPP

#include <glm/glm.hpp>

class Shader;

class TextureArray;

class Camera;

class RenderStates {
   public:
    RenderStates();

    // Note: the transform is not copied to avoid the transform accumulation
    RenderStates(const RenderStates &);

    static const RenderStates Default;

    const Shader *shader;
    glm::mat4 transform;
    const TextureArray *textures;
    const Camera *camera;
};

#endif
