#ifndef RENDER_TARGET_HPP
#define RENDER_TARGET_HPP

#include <glm/glm.hpp>

class Drawable;
class VertexBuffer;

class RenderTarget {
   public:
    RenderTarget();

    void setView(const glm::mat4 &view);

    void draw(const Drawable &drawable);

    void draw(const VertexBuffer &buffer);

   private:
    glm::mat4 m_view;
    uint32_t m_VAO;
};

#endif
