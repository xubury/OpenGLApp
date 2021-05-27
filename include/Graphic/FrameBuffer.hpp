#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <Graphic/Shader.hpp>

class FrameBuffer {
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

    uint32_t getScreenTexture() const;

   private:
    uint32_t m_multiSampleFrameBufferId;
    uint32_t m_multiSampleTextureId;
    uint32_t m_multiSampleRenderBufferId;

    // multisample texture can't draw directly
    // need to copy it to another framebuffer with normal texture
    uint32_t m_frameBufferId;
    uint32_t m_textureId;

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
