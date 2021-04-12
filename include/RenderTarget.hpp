#ifndef RENDER_TARGET_HPP
#define RENDER_TARGET_HPP

#include <glm/glm.hpp>

class Drawable;
class VertexBuffer;

class RenderTarget {
   public:
    RenderTarget();

    bool create();

    void setView(const glm::mat4 &view);

    void draw(const Drawable &drawable);

    void draw(const VertexBuffer &buffer);

    void clear(float r = 0.1f, float g = 0.2f, float b = 0.3f, float a = 1.f);

   private:
    glm::mat4 m_view;
    uint32_t m_VAO;
};

#endif
