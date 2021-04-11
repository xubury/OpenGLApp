#ifndef RENDER_STATES_HPP
#define RENDER_STATES_HPP

#include <glm/glm.hpp>

class RenderStates {
   public:
    void setTransform(const glm::mat4 &transform);

   private:
    glm::mat4 m_transform;
};

#endif
