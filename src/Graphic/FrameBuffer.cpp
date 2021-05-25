#include <Graphic/FrameBuffer.hpp>
#include <iostream>
#include <glad/glad.h>

FrameBufferShader &FrameBufferShader::instance() {
    static FrameBufferShader s_instance;
    return s_instance;
}

FrameBufferShader::FrameBufferShader() {
    const char *vertexCode =
        "#version 330 core\n"
        "layout (location = 0) in vec2 aPos;\n"
        "layout (location = 1) in vec2 aTexCoords;\n"
        "out vec2 texCoords;\n"
        "void main() {\n"
        "    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
        "    texCoords = aTexCoords;\n"
        "}";

    const char *fragmentCode =
        "#version 330 core\n"
        "out vec4 fragColor;\n"
        "in vec2 texCoords;\n"
        "uniform sampler2D screenTexture;\n"
        "void main() {\n"
        "    fragColor = texture(screenTexture, texCoords);\n"
        "}";

    compile(vertexCode, fragmentCode);
    use();
    setInt("screenTexture", 0);
}

FrameBuffer::FrameBuffer()
    : m_multiSampleFrameBufferId(0),
      m_multiSampleTextureId(0),
      m_multiSampleRenderBufferId(0),
      m_frameBufferId(0),
      m_textureId(0),
      m_VBO(0),
      m_width(0),
      m_height(0),
      m_sample(1) {}

void FrameBuffer::initialize(int width, int height, int sample) {
    glGenFramebuffers(1, &m_multiSampleFrameBufferId);
    glGenFramebuffers(1, &m_frameBufferId);

    glGenRenderbuffers(1, &m_multiSampleRenderBufferId);

    glGenTextures(1, &m_multiSampleTextureId);
    glGenTextures(1, &m_textureId);

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
    update(width, height, sample);
}

static void attachMultisample(int frameBuffer, int renderBuffer, int texture,
                              int width, int height, int samples) {
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

static void attachNormalTexture(int framebuffer, int texture, int width,
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

void FrameBuffer::update(int width, int height, int sample) {
    assert(width > 0 && height > 0);
    // if width, height or sample changed, do an update
    if (m_width != width || m_height != height ||
        (sample > 0 && m_sample != sample)) {
        m_width = width;
        m_height = height;
        if (sample > 0) m_sample = sample;
        attachMultisample(m_multiSampleFrameBufferId,
                          m_multiSampleRenderBufferId, m_multiSampleTextureId,
                          m_width, m_height, m_sample);
        attachNormalTexture(m_frameBufferId, m_textureId, width, height);
    }
}

void FrameBuffer::activate() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_multiSampleFrameBufferId);
    glEnable(GL_DEPTH_TEST);
}

void FrameBuffer::deactivate() const {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_multiSampleFrameBufferId);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frameBufferId);
    glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
}

void FrameBuffer::draw() {
    deactivate();
    FrameBufferShader::instance().use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

uint32_t FrameBuffer::getFrameBufferId() const { return m_frameBufferId; }

uint32_t FrameBuffer::getScreenTexture() const { return m_textureId; }
