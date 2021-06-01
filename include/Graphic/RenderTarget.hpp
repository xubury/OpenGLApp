#ifndef RENDER_TARGET_HPP
#define RENDER_TARGET_HPP

#include <Graphic/Export.hpp>
#include <Graphic/Drawable.hpp>

#include <glm/glm.hpp>
#include <list>

class CameraBase;

class BufferObject;

class Primitive;

class Shader;

class GRAPHIC_API RenderTarget {
   public:
    RenderTarget(const RenderTarget &) = delete;

    RenderTarget &operator=(const RenderTarget &) = delete;

    void beginScene(const Shader &shader, const CameraBase &camera,
                    const std::list<LightBase *> &light);

    void endScene();

    virtual void draw(const BufferObject &buffer, const RenderStates &states);

    void clear(float r = 0.1f, float g = 0.2f, float b = 0.3f, float a = 1.f);

   protected:
    RenderTarget();

    virtual ~RenderTarget() = default;

    void applyShader(const Shader &shader);

    void applyTransform(const glm::mat4 &transform);

   private:
    void applyTexture(const TextureArray *textures);

    const Shader *m_shader;

    const TextureArray *m_textures;

    uint32_t m_textureReserved;
};

#endif
