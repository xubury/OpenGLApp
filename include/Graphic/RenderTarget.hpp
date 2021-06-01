#ifndef RENDER_TARGET_HPP
#define RENDER_TARGET_HPP

#include <Graphic/Export.hpp>
#include <Graphic/Drawable.hpp>

#include <glm/glm.hpp>
#include <vector>

class CameraBase;

class BufferObject;

class Primitive;

class Shader;

class GRAPHIC_API RenderTarget {
   public:
    RenderTarget(const RenderTarget &) = delete;

    RenderTarget &operator=(const RenderTarget &) = delete;

    void beginScene(const Shader &shader, const CameraBase &camera,
                    const LightBase &light);

    void beginDepthMap(const LightBase &light);

    void draw(const Drawable &drawable,
              const RenderStates &states = RenderStates::Default);

    void draw(const BufferObject &buffer,
              const RenderStates &states = RenderStates::Default);

    void clear(float r = 0.1f, float g = 0.2f, float b = 0.3f, float a = 1.f);

   protected:
    RenderTarget();

    virtual ~RenderTarget() = default;

   private:
    void applyShader(const Shader &shader);

    void applyTransform(const glm::mat4 &transform);

    void applyTexture(const TextureArray *textures);

    const Shader *m_shader;

    const TextureArray *m_textures;
};

#endif
