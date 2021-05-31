#include <Graphic/FrameBuffer.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>
#include <glbinding/gl/gl.h>
#include <glm/glm.hpp>

using namespace gl;

Shader FrameBuffer::s_shader;
Shader FrameBuffer::s_shadowShader;

static const float SHADOW_WIDTH = 800;
static const float SHADOW_HEIGHT = 600;

static void attachShadowTexture(int framebuffer, int texture) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH,
                 SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
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

static void attachScreenTexture(int framebuffer, int texture, int width,
                                int height) {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glBindTexture(GL_TEXTURE_2D, texture);

    // create multisample texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // attach texture to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           texture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!"
                  << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

static void attachMultisampleTexture(int frameBuffer, int renderBuffer,
                                     int texture, int width, int height,
                                     int samples) {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);

    // create multisample texture
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width,
                            height, GL_TRUE);
    // attach texture to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D_MULTISAMPLE, texture, 0);

    // create render buffer
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples,
                                     GL_DEPTH24_STENCIL8, width, height);
    // attach render buffer to currently bound framebuffer object
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, renderBuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!"
                  << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

FrameBuffer::FrameBuffer()
    : m_multiSampleFrameBufferId(0),
      m_multiSampleTextureId(0),
      m_multiSampleRenderBufferId(0),
      m_frameBufferId(0),
      m_screenTextureId(0),
      m_VBO(0),
      m_VAO(0),
      m_width(0),
      m_height(0),
      m_sample(1) {}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &m_multiSampleFrameBufferId);
    glDeleteFramebuffers(1, &m_frameBufferId);

    glDeleteRenderbuffers(1, &m_multiSampleRenderBufferId);

    glDeleteTextures(1, &m_multiSampleTextureId);
    glDeleteTextures(1, &m_screenTextureId);
}

void FrameBuffer::initialize(int width, int height, int sample) {
    glGenFramebuffers(1, &m_multiSampleFrameBufferId);
    glGenFramebuffers(1, &m_frameBufferId);
    glGenFramebuffers(1, &m_shadowFrameBufferId);

    glGenRenderbuffers(1, &m_multiSampleRenderBufferId);

    glGenTextures(1, &m_multiSampleTextureId);
    glGenTextures(1, &m_screenTextureId);
    glGenTextures(1, &m_shadowTextureId);

    // screen texture vbo vao
    const float quadVertices[] = {-1.0f, 1.0f, 0.0f, 1.0f,  -1.0f, -1.0f,
                                  0.0f,  0.0f, 1.0f, -1.0f, 1.0f,  0.0f,

                                  -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  -1.0f,
                                  1.0f,  0.0f, 1.0f, 1.0f,  1.0f,  1.0f};
    glGenBuffers(1, &m_VBO);
    glGenVertexArrays(1, &m_VAO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices,
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)(2 * sizeof(float)));
    // shadow
    attachShadowTexture(m_shadowFrameBufferId, m_shadowTextureId);

    update(width, height, sample);
}

void FrameBuffer::update(int width, int height, int sample) {
    assert(width > 0 && height > 0);
    // if width, height or sample changed, do an update
    if (m_width != width || m_height != height ||
        (sample > 0 && m_sample != sample)) {
        m_width = width;
        m_height = height;
        if (sample > 0) m_sample = sample;
        attachMultisampleTexture(
            m_multiSampleFrameBufferId, m_multiSampleRenderBufferId,
            m_multiSampleTextureId, m_width, m_height, m_sample);
        attachScreenTexture(m_frameBufferId, m_screenTextureId, width, height);
    }
}

void FrameBuffer::activate() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_multiSampleFrameBufferId);
}

void FrameBuffer::deactivate() const {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_multiSampleFrameBufferId);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frameBufferId);
    glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::draw() {
    deactivate();
    glDisable(GL_DEPTH_TEST);
    s_shader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_screenTextureId);
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
}

void FrameBuffer::setupDepthDraw() {
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFrameBufferId);
    glClear(GL_DEPTH_BUFFER_BIT);
}

uint32_t FrameBuffer::getScreenTexture() const { return m_screenTextureId; }

uint32_t FrameBuffer::getDepthMapTexture() const { return m_shadowTextureId; }
