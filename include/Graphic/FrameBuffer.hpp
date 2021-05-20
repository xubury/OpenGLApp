#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <Graphic/Shader.hpp>

class FrameBuffer {
   public:
    FrameBuffer();

    void create(int width, int height);

    void update(int width, int height);

    void activate() const;

    void deactivate() const;

    void draw() const;

    uint32_t getTextureId() const;

    uint32_t getFrameBufferId() const;

   private:
    uint32_t m_frameBufferId;
    uint32_t m_VBO;
    uint32_t m_VAO;
    uint32_t m_textureId;
    uint32_t m_renderBufferId;

    int m_width;
    int m_height;
};

class FrameBufferShader : public Shader {
   public:
    static FrameBufferShader &instance();
   private:
    FrameBufferShader();
};

#endif /* FRAMEBUFFER_HPP */
