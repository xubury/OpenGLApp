#ifndef RENDER_TARGET_HPP
#define RENDER_TARGET_HPP

#include <glm/glm.hpp>
#include "Camera.hpp"

class Drawable;

class RenderStates;

class VertexBuffer;

class RenderTarget {
   public:
    RenderTarget();

    bool create();

    void setCamera(const Camera &camera);

    Camera &getCamera();

    void draw(const Drawable &drawable, const RenderStates &states);

    void draw(const VertexBuffer &buffer, const RenderStates &states);

    void clear(float r = 0.1f, float g = 0.2f, float b = 0.3f, float a = 1.f);

   private:
    Camera m_camera;

    uint32_t m_VAO;
};

#endif
