#include "Core/Assert.hpp"
#include "Graphic/ShadowBuffer.hpp"
#include "Graphic/OpenGL.hpp"
#include "Graphic/LightBase.hpp"
#include <iostream>

namespace te {

ShaderLibrary ShadowBuffer::s_shaders;

static void attachDepthMapTexture(int framebuffer, int texture, int width,
                                  int height) {
    TE_CORE_ASSERT(width > 0 && height > 0,
                   "attachDepthMapTexture width or height <= 0");
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    const float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           texture, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        TE_CORE_ERROR("FrameBuffer is not complete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Ref<ShadowBuffer> ShadowBuffer::create(int width, int height) {
    return createRef<ShadowBuffer>(width, height);
}

ShadowBuffer::ShadowBuffer(int width, int height)
    : m_width(width), m_height(height) {
    if (!s_shaders.exists("shadow")) {
        const char *shadowVertex =
            "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "uniform mat4 uLightSpaceMatrix;\n"
            "uniform mat4 uModel;\n"
            "void main() {\n"
            "    gl_Position = uLightSpaceMatrix * uModel * vec4(aPos, 1.0);\n"
            "}";
        const char *shadowFragment =
            "#version 330 core\n"
            "void main() {\n"
            "}";

        s_shaders.add("shadow");
        s_shaders.get("shadow")->compile(shadowVertex, shadowFragment);
    }
    glGenFramebuffers(1, &m_frameBufferId);
    glGenTextures(1, &m_textureId);
    attachDepthMapTexture(m_frameBufferId, m_textureId, m_width, m_height);
}

ShadowBuffer::~ShadowBuffer() {
    glDeleteFramebuffers(1, &m_frameBufferId);
    glDeleteTextures(1, &m_textureId);
}

uint32_t ShadowBuffer::getDepthMap() const { return m_textureId; }

uint32_t ShadowBuffer::getFrameBuffer() const { return m_frameBufferId; }

uint32_t ShadowBuffer::getWidth() const { return m_width; }

uint32_t ShadowBuffer::getHeight() const { return m_height; }

// void ShadowBuffer::beginScene(const LightBase &light) {
//     applyShader(s_shaders.get("shadow"));
//     glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
//     glViewport(0, 0, m_width, m_height);
//     glClear(GL_DEPTH_BUFFER_BIT);
//     s_shaders.get("shadow")->setMat4("uLightSpaceMatrix",
//                                      light.getLightSpaceMatrix());
//     glCullFace(GL_FRONT);
// }

// void ShadowBuffer::endScene() {
//     glBindFramebuffer(GL_FRAMEBUFFER, 0);
//     glCullFace(GL_BACK);
// }

// void ShadowBuffer::draw(const BufferObject &buffer,
//                         const RenderStates &states) {
//     applyTransform(states.transform);
//     buffer.drawPrimitive();
// }

}  // namespace te
