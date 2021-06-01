#include <Graphic/ShadowBuffer.hpp>
#include <Graphic/OpenGL.hpp>
#include <Graphic/LightBase.hpp>
#include <Graphic/BufferObject.hpp>
#include <iostream>

static void attachDepthMapTexture(int framebuffer, int texture, int width,
                                  int height) {
    assert(width > 0 && height > 0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           texture, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!"
                  << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

ShadowBuffer::ShadowBuffer(int width, int height)
    : m_width(width), m_height(height) {
    glGenFramebuffers(1, &m_frameBufferId);
    glGenTextures(1, &m_textureId);
    attachDepthMapTexture(m_frameBufferId, m_textureId, m_width, m_height);
}

ShadowBuffer::~ShadowBuffer() {
    glDeleteFramebuffers(1, &m_frameBufferId);
    glDeleteTextures(1, &m_textureId);
}

uint32_t ShadowBuffer::getDepthMapTexture() const { return m_textureId; }

uint32_t ShadowBuffer::getFrameBuffer() const { return m_frameBufferId; }

uint32_t ShadowBuffer::getWidth() const { return m_width; }

uint32_t ShadowBuffer::getHeight() const { return m_height; }

void ShadowBuffer::beginScene(const Shader &shader, const LightBase &light) {
    applyShader(shader);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
    glViewport(0, 0, m_width, m_height);
    glClear(GL_DEPTH_BUFFER_BIT);
    shader.setMat4("uLightSpaceMatrix", light.getLightSpaceMatrix());
}

void ShadowBuffer::endScene() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void ShadowBuffer::draw(const BufferObject &buffer,
                        const RenderStates &states) {
    applyTransform(states.transform);
    buffer.drawPrimitive();
}
