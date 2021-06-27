#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "Core/Export.hpp"
#include "Core/Assert.hpp"
#include "Graphic/Shader.hpp"
#include "Graphic/LightBase.hpp"
#include "Graphic/TextureParameter.hpp"
#include <vector>

namespace te {

enum class FramebufferTextureFormat {
    NONE = 0,

    // Color
    RED_INTEGER,
    RGB8,
    RGB16F,
    RGBA8,
    RGBA16F,

    // Dpeth
    DEPTH24,
    DEPTH32,

    // Depth/stencil
    DEPTH24STENCIL8
};

struct TE_API FramebufferTextureSpecification {
    FramebufferTextureSpecification()
        : textureFormat(FramebufferTextureFormat::NONE) {}
    FramebufferTextureSpecification(
        FramebufferTextureFormat format,
        TextureParameter params = TextureParameter())
        : textureFormat(format), textureParams(params) {}

    FramebufferTextureFormat textureFormat;
    TextureParameter textureParams;
};

struct TE_API FramebufferAttachmentSpecification {
    std::vector<FramebufferTextureSpecification> specifications;

    FramebufferAttachmentSpecification() = default;
    FramebufferAttachmentSpecification(
        std::initializer_list<FramebufferTextureSpecification> specs)
        : specifications(specs) {}
};

struct TE_API FrameBufferSpecification {
    uint32_t width;
    uint32_t height;
    uint32_t samples;
    FramebufferAttachmentSpecification attachmentsSpecs;

    FrameBufferSpecification() : width(0), height(0), samples(0) {}
};

class TE_API FrameBuffer {
   public:
    FrameBuffer(const FrameBufferSpecification &spec,
                bool depthWriteOnly = false);

    ~FrameBuffer();

    FrameBuffer(const FrameBuffer &) = delete;

    FrameBuffer &operator=(const FrameBuffer &) = delete;

    void resize(int width, int height);

    void bind() const;

    void unbind() const;

    void invalidate();

    uint32_t getColorAttachmentId(uint32_t index = 0) const;

    uint32_t getDepthAttachmentId() const;

    FrameBufferSpecification getSpecification() const;

    uint32_t getId() const;

    void destroy();

    void copy(const FrameBuffer &dst, GLenum attachment, ClearBufferMask mask,
              GLenum filtering) const;

   private:
    FrameBufferSpecification m_specification;
    uint32_t m_bufferId;

    std::vector<FramebufferTextureSpecification> m_colorAttachmentSpecs;
    std::vector<uint32_t> m_colorAttachments;

    FramebufferTextureSpecification m_depthAttachmentSpec;
    uint32_t m_depthAttachment;
    const bool m_depthWriteOnly;
};

}  // namespace te

#endif /* FRAMEBUFFER_HPP */
