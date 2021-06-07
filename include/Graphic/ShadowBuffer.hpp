#ifndef SHADOW_BUFFER_HPP
#define SHADOW_BUFFER_HPP

#include "Core/Base.hpp"
#include "Core/Export.hpp"
#include "Graphic/Shader.hpp"
#include "Graphic/RenderTarget.hpp"

namespace te {

class TE_API ShadowBuffer : public RenderTarget {
   public:
    static Ref<ShadowBuffer> create(int width, int height);

    ShadowBuffer(int width, int height);

    ~ShadowBuffer();

    ShadowBuffer(const ShadowBuffer &) = delete;

    ShadowBuffer &operator=(const ShadowBuffer &) = delete;

    void beginScene(const Ref<LightBase> &light);

    void endScene();

    void draw(const BufferObject &buffer, const RenderStates &states) override;

    uint32_t getDepthMap() const;

    uint32_t getFrameBuffer() const;

    uint32_t getWidth() const;

    uint32_t getHeight() const;

   private:
    uint32_t m_width;
    uint32_t m_height;

    uint32_t m_frameBufferId;
    uint32_t m_textureId;

    static ShaderLibrary s_shaders;
};

}  // namespace te

#endif /* SHADOW_BUFFER_HPP */
