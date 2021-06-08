#ifndef RENDER_TARGET_HPP
#define RENDER_TARGET_HPP

#include "Core/Base.hpp"
#include "Core/Export.hpp"
#include "Graphic/Drawable.hpp"

#include <glm/glm.hpp>
#include <vector>

namespace te {

class CameraBase;

class BufferObject;

class ShadowBuffer;

class Shader;

class TE_API RenderTarget {
   public:
    RenderTarget(const RenderTarget &) = delete;

    RenderTarget &operator=(const RenderTarget &) = delete;

    void beginScene(Ref<Shader> shader, const CameraBase &camera);

    void setLighting(const std::vector<const LightBase *> &lights,
                     const std::vector<Ref<ShadowBuffer>> &shadowBuffers);

    void endScene();

    virtual void draw(const BufferObject &buffer, const RenderStates &states);

    void clear(float r = 0.1f, float g = 0.2f, float b = 0.3f, float a = 1.f);

   protected:
    RenderTarget();

    virtual ~RenderTarget() = default;

    void applyShader(Ref<Shader> shader);

    void applyTransform(const glm::mat4 &transform);

   private:
    void applyTexture(const TextureArray *textures);

    Ref<Shader> m_shader;

    const TextureArray *m_textures;

    uint32_t m_textureReserved;
};

}  // namespace te

#endif
