#ifndef RENDER_STATES_HPP
#define RENDER_STATES_HPP

#include <glm/glm.hpp>

class Shader;

class Texture;

class Camera;

class RenderStates {
   public:
    RenderStates();
    static const RenderStates Default;

    const Shader *shader;
    glm::mat4 transform;
    const Texture *texture;
    const Camera *camera;
};

#endif
