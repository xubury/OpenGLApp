#include "Core/Assert.hpp"
#include "Graphic/FrameBuffer.hpp"
#include "Graphic/OpenGL.hpp"
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>

namespace te {

ShaderLibrary FrameBuffer::s_shaders;

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
        TE_CORE_ERROR("Framebuffer is not complete!");

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
        TE_CORE_ERROR("FrameBuffer is not complete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

FrameBuffer::FrameBuffer(int width, int height, int sample)
    : m_multiSampleFrameBufferId(0),
      m_multiSampleTextureId(0),
      m_multiSampleRenderBufferId(0),
      m_frameBufferId(0),
      m_screenTextureId(0),
      m_VBO(0),
      m_VAO(0),
      m_width(0),
      m_height(0),
      m_sample(1) {
    if (!s_shaders.exists("frameBuffer")) {
        const char* fbVertex =
            "#version 330 core\n"
            "layout (location = 0) in vec2 aPos;\n"
            "layout (location = 1) in vec2 aTexCoords;\n"
            "out vec2 texCoords;\n"
            "void main() {\n"
            "    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
            "    texCoords = aTexCoords;\n"
            "}";

        const char* fbFragment =
            "#version 330 core\n"
            "out vec4 fragColor;\n"
            "in vec2 texCoords;\n"
            "uniform sampler2D uScreenTexture;\n"
            "void main() {\n"
            "    fragColor = texture(uScreenTexture, texCoords);\n"
            "}";

        s_shaders.add("frameBuffer");
        s_shaders.get("frameBuffer")->compile(fbVertex, fbFragment);
    }
    glGenFramebuffers(1, &m_multiSampleFrameBufferId);
    glGenFramebuffers(1, &m_frameBufferId);

    glGenRenderbuffers(1, &m_multiSampleRenderBufferId);

    glGenTextures(1, &m_multiSampleTextureId);
    glGenTextures(1, &m_screenTextureId);

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
                          (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void*)(2 * sizeof(float)));

    update(width, height, sample);
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &m_multiSampleFrameBufferId);
    glDeleteFramebuffers(1, &m_frameBufferId);

    glDeleteRenderbuffers(1, &m_multiSampleRenderBufferId);

    glDeleteTextures(1, &m_multiSampleTextureId);
    glDeleteTextures(1, &m_screenTextureId);
}

void FrameBuffer::update(int width, int height, int sample) {
    TE_CORE_ASSERT(width > 0 && height > 0,
                   "FrameBuffer::update width or height <= 0");
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
    glDisable(GL_DEPTH_TEST);
    s_shaders.get("frameBuffer")->bind();
    s_shaders.get("frameBuffer")->setInt("uScreenTexture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_screenTextureId);
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
}

uint32_t FrameBuffer::getScreenTexture() const { return m_screenTextureId; }

}  // namespace te
