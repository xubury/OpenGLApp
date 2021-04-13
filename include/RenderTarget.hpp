#ifndef RENDER_TARGET_HPP
#define RENDER_TARGET_HPP

#include <memory>
#include <glm/glm.hpp>

#include "Drawable.hpp"
#include "Camera.hpp"

class Drawable;
class VertexBuffer;
class Shader;
class Transform;
class Texture;
class Camera;

class RenderTarget {
   public:
    void draw(const Drawable &drawable,
              const RenderStates &states = RenderStates::Default);

    void draw(const VertexBuffer &buffer,
              const RenderStates &states = RenderStates::Default);

    void clear(float r = 0.1f, float g = 0.2f, float b = 0.3f, float a = 1.f);

    const Camera *getCamera();

    void setCamera(std::unique_ptr<Camera> camera);

    bool processEvent(Event &event) const;

    void processEvents() const;

   protected:
    RenderTarget();

    ~RenderTarget();

   private:
    void applyShader(const Shader *shader);

    void applyTransform(const glm::mat4 &transform) const;

    void applyTexture(const Texture *texture) const;

    uint32_t m_VAO;

    std::unique_ptr<Camera> m_camera;

    const Shader *m_shader;
};

#endif
