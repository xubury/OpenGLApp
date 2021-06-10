#include "Core/Assert.hpp"
#include "Graphic/FrameBuffer.hpp"
#include "Graphic/OpenGL.hpp"
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>

namespace te {

static GLenum textureTarget(bool multisampled) {
    return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
}

static void createTextures(bool multisampled, uint32_t* outId,
                           std::size_t count) {
    glCreateTextures(textureTarget(multisampled), count, outId);
}

static void bindTexture(bool multisampled, uint32_t id) {
    glBindTexture(textureTarget(multisampled), id);
}

static void attachColorTexture(uint32_t id, int samples, GLenum internalFormat,
                               GLenum format, uint32_t width, uint32_t height,
                               int index) {
    bool multisampled = samples > 1;
    if (multisampled) {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples,
                                internalFormat, width, height, GL_FALSE);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format,
                     GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index,
                           textureTarget(multisampled), id, 0);
}

static void attachDepthTexture(uint32_t id, int samples, GLenum format,
                               GLenum attachmentType, uint32_t width,
                               uint32_t height) {
    bool multisampled = samples > 1;
    if (multisampled) {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format,
                                width, height, GL_FALSE);
    } else {
        glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType,
                           textureTarget(multisampled), id, 0);
}

static bool isDepthFormat(FramebufferTextureFormat format) {
    switch (format) {
        case FramebufferTextureFormat::DPETH:
            return true;
        case FramebufferTextureFormat::DEPTH24STENCIL8:
            return true;
        default:
            return false;
    }
}

FrameBuffer::FrameBuffer(const FrameBufferSpecification& spec)
    : m_specification(spec),
      m_bufferId(0),
      m_depthAttachmentSpec(FramebufferTextureFormat::NONE),
      m_depthAttachment(0) {
    for (const auto& spec : m_specification.attachmentsSpecs.specifications) {
        if (!isDepthFormat(spec.textureFormat)) {
            m_colorAttachmentSpecs.emplace_back(spec);
        } else {
            m_depthAttachmentSpec = spec;
        }
    }
    invalidate();
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &m_bufferId);
    glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
    glDeleteTextures(1, &m_depthAttachment);
}

void FrameBuffer::invalidate() {
    if (m_bufferId) {
        glDeleteFramebuffers(1, &m_bufferId);
        glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
        glDeleteTextures(1, &m_depthAttachment);

        m_colorAttachments.clear();
        m_depthAttachment = 0;
    }
    glCreateFramebuffers(1, &m_bufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_bufferId);
    bool multiSample = m_specification.samples > 1;
    if (m_colorAttachmentSpecs.size()) {
        m_colorAttachments.resize(m_colorAttachmentSpecs.size());
        createTextures(multiSample, m_colorAttachments.data(),
                       m_colorAttachments.size());
        for (std::size_t i = 0; i < m_colorAttachments.size(); ++i) {
            bindTexture(multiSample, m_colorAttachments[i]);
            switch (m_colorAttachmentSpecs[i].textureFormat) {
                case FramebufferTextureFormat::RGB:
                    attachColorTexture(m_colorAttachments[i],
                                       m_specification.samples, GL_RGB, GL_RGB,
                                       m_specification.width,
                                       m_specification.height, i);
                    break;
                case FramebufferTextureFormat::RGBA8:
                    attachColorTexture(m_colorAttachments[i],
                                       m_specification.samples, GL_RGBA8,
                                       GL_RGB, m_specification.width,
                                       m_specification.height, i);
                    break;
                case FramebufferTextureFormat::RED_INTEGER:
                    attachColorTexture(m_colorAttachments[i],
                                       m_specification.samples, GL_R32I,
                                       GL_RED_INTEGER, m_specification.width,
                                       m_specification.height, i);
                    break;
                default:
                    TE_CORE_ASSERT(false,
                                   "Framebuffer texture format not handled!");
            }
        }
    }
    if (m_depthAttachmentSpec.textureFormat != FramebufferTextureFormat::NONE) {
        createTextures(multiSample, &m_depthAttachment, 1);
        bindTexture(multiSample, m_depthAttachment);
        switch (m_depthAttachmentSpec.textureFormat) {
            case FramebufferTextureFormat::DPETH:
                attachDepthTexture(m_depthAttachment, m_specification.samples,
                                   GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT,
                                   m_specification.width,
                                   m_specification.height);
                break;
            case FramebufferTextureFormat::DEPTH24STENCIL8:
                attachDepthTexture(
                    m_depthAttachment, m_specification.samples,
                    GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT,
                    m_specification.width, m_specification.height);
                break;
            default:
                TE_CORE_ASSERT(false,
                               "Framebuffer texture format not handled!");
        }
    }
    if (m_colorAttachments.size() > 1) {
        TE_CORE_ASSERT(m_colorAttachments.size() <= 4, "color attachments > 4");
        GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                             GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
        glDrawBuffers(m_colorAttachments.size(), buffers);
    } else if (m_colorAttachments.empty()) {
        glDrawBuffer(GL_NONE);
    }
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        TE_CORE_ERROR("FrameBuffer is not complete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void FrameBuffer::resize(int width, int height) {
    TE_CORE_ASSERT(width > 0 && height > 0,
                   "FrameBuffer::update width or height <= 0");
    m_specification.height = height;
    m_specification.width = width;

    invalidate();
}

void FrameBuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_bufferId);
}

void FrameBuffer::unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

uint32_t FrameBuffer::getColorAttachmentId(uint32_t index) const {
    TE_CORE_ASSERT(index < m_colorAttachments.size(),
                   "color attachment's index reach out of bound");
    return m_colorAttachments[index];
}

FrameBufferSpecification FrameBuffer::getSpecification() const {
    return m_specification;
}

uint32_t FrameBuffer::getId() const { return m_bufferId; }

}  // namespace te
