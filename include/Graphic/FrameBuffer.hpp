#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <Graphic/Export.hpp>
#include <Graphic/Shader.hpp>
#include <Graphic/LightBase.hpp>

class GRAPHIC_API FrameBuffer {
   public:
    FrameBuffer();

    ~FrameBuffer();

    FrameBuffer(const FrameBuffer &) = delete;

    FrameBuffer &operator=(const FrameBuffer &) = delete;

    void initialize(int width, int height, int smaple);

    void update(int width, int height, int smaple = 0);

    void activate() const;

    void deactivate() const;

    void draw();

    void setupDepthDraw();

    uint32_t getScreenTexture() const;

    uint32_t getDepthMapTexture() const;

    static Shader s_shadowShader;

   private:
    uint32_t m_multiSampleFrameBufferId;
    uint32_t m_multiSampleTextureId;
    uint32_t m_multiSampleRenderBufferId;

    // multisample texture can't draw directly
    // need to copy it to another framebuffer with normal texture
    uint32_t m_frameBufferId;
    uint32_t m_screenTextureId;

    uint32_t m_shadowFrameBufferId;
    uint32_t m_shadowTextureId;

    uint32_t m_VBO;
    uint32_t m_VAO;

    int m_width;
    int m_height;
    int m_sample;

   private:
    friend class Shader;
    static Shader s_shader;
};

#endif /* FRAMEBUFFER_HPP */
