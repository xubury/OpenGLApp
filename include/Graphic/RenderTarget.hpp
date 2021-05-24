#ifndef RENDER_TARGET_HPP
#define RENDER_TARGET_HPP

#include <memory>
#include <glm/glm.hpp>

#include <Graphic/Drawable.hpp>

class Camera;
class VertexBuffer;
class ElementBuffer;
class Primitive;
class Shader;

class RenderTarget {
   public:
    RenderTarget(const RenderTarget &) = delete;

    RenderTarget &operator=(const RenderTarget &) = delete;

    void draw(const Drawable &drawable,
              const RenderStates &states = RenderStates::Default);

    void draw(const VertexBuffer &buffer,
              const RenderStates &states = RenderStates::Default);

    void draw(const ElementBuffer &buffer,
              const RenderStates &states = RenderStates::Default);

    void drawLine(const glm::vec3 &start, const glm::vec3 &end,
                  const glm::vec4 &color, float thickness,
                  const Camera *camera);

    void drawBox(const glm::vec3 &min, const glm::vec3 &max,
                 const glm::vec4 &color, const Camera *camera);

    void clear(float r = 0.1f, float g = 0.2f, float b = 0.3f, float a = 1.f);

   protected:
    RenderTarget();

    virtual ~RenderTarget() = default;

   private:
    void applyCamera(const Camera *camera);

    void applyShader(const Shader *shader);

    void applyTransform(const glm::mat4 &transform);

    void applyTexture(const TextureArray *textures);

    const Shader *m_shader;

    const TextureArray *m_textures;

    bool m_isRenderOnFrameBuffer;
};

#endif
