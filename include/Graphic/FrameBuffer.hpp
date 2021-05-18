#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <stdint.h>

class FrameBuffer {
   public:
    FrameBuffer();

    void create(int width, int height);

    void use() const;

    void draw() const;

    uint32_t getTextureId() const;

    uint32_t getFrameBufferId() const;
   private:
    uint32_t m_frameBufferId;
    uint32_t m_textureId;
    uint32_t m_renderBufferId;
};

#endif /* FRAMEBUFFER_HPP */
