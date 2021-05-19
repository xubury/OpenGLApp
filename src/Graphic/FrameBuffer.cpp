#include <Graphic/FrameBuffer.hpp>
#include <iostream>
#include <glad/glad.h>
#include <Graphic/Shader.hpp>

static const char *vertexCode =
    "#version 330 core\n"
    "layout (location = 0) in vec2 aPos;\n"
    "layout (location = 1) in vec2 aTexCoords;\n"
    "out vec2 texCoords;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
    "    texCoords = aTexCoords;\n"
    "}";

static const char *fragmentCode =
    "#version 330 core\n"
    "out vec4 fragColor;\n"
    "in vec2 texCoords;\n"
    "uniform sampler2D screenTexture;\n"
    "void main() {\n"
    "    fragColor = texture(screenTexture, texCoords);\n"
    "}";

static const float quadVertices[] = {
    -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f,

    -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f, 1.0f};

static uint32_t quadVAO = 0;

static uint32_t quadVBO = 0;

static Shader screenShader;

FrameBuffer::FrameBuffer()
    : m_frameBufferId(0),
      m_textureId(0),
      m_renderBufferId(0),
      m_width(0),
      m_height(0),
      m_isInitialized(false) {}

void FrameBuffer::create(int width, int height) {
    if (!screenShader.isInitialized()) {
        screenShader.compile(vertexCode, fragmentCode);
        screenShader.use();
        screenShader.setInt("screenTexture", 0);
    }
    if (quadVAO == 0) {
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices,
                     GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                              (void *)(2 * sizeof(float)));
    }
    if (!m_isInitialized) {
        glGenFramebuffers(1, &m_frameBufferId);
        glGenTextures(1, &m_textureId);
        glGenRenderbuffers(1, &m_renderBufferId);
        update(width, height);
        m_isInitialized = true;
    } else {
        update(width, height);
    }
}

void FrameBuffer::update(int width, int height) {
    assert(width > 0 && height > 0);
    if (m_width != width || m_height != height) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
        glBindTexture(GL_TEXTURE_2D, m_textureId);
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferId);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // attach it to currently bound framebuffer object
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, m_textureId, 0);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width,
                              height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                                  GL_RENDERBUFFER, m_renderBufferId);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!"
                      << std::endl;
        m_width = width;
        m_height = height;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }
}

void FrameBuffer::activate() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
    glEnable(GL_DEPTH_TEST);
}

void FrameBuffer::deactivate() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
}

void FrameBuffer::draw() const {
    deactivate();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    screenShader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

uint32_t FrameBuffer::getFrameBufferId() const { return m_frameBufferId; }

uint32_t FrameBuffer::getTextureId() const { return m_textureId; }
