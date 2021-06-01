#ifndef SHADOW_BUFFER_HPP
#define SHADOW_BUFFER_HPP

#include <Graphic/Export.hpp>
#include <Graphic/Shader.hpp>
#include <Graphic/RenderTarget.hpp>

class GRAPHIC_API ShadowBuffer : public RenderTarget {
   public:
    ShadowBuffer(int width = 800, int height = 600);

    ~ShadowBuffer();

    ShadowBuffer(const ShadowBuffer &) = delete;

    void beginScene(const Shader &shader, const LightBase &light);

    void endScene();

    void draw(const BufferObject &buffer, const RenderStates &states) override;

    ShadowBuffer &operator=(const ShadowBuffer &) = delete;

    uint32_t getDepthMapTexture() const;

    uint32_t getFrameBuffer() const;

    uint32_t getWidth() const;

    uint32_t getHeight() const;

   private:
    uint32_t m_width;
    uint32_t m_height;

    uint32_t m_frameBufferId;
    uint32_t m_textureId;
};

#endif /* SHADOW_BUFFER_HPP */
