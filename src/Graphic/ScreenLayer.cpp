#include "Graphic/ScreenLayer.hpp"
#include "Graphic/Renderer.hpp"

namespace te {

ScreenLayer::ScreenLayer(int width, int height, int samples)
    : m_width(width), m_height(height) {
    m_shader = createRef<Shader>();
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

    m_shader->compile(fbVertex, fbFragment);
    m_shader->bind();
    m_shader->setInt("uScreenTexture", 0);
    const float quadVertices[] = {-1.0f, 1.0f, 0.0f, 1.0f,  -1.0f, -1.0f,
                                  0.0f,  0.0f, 1.0f, -1.0f, 1.0f,  0.0f,

                                  -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  -1.0f,
                                  1.0f,  0.0f, 1.0f, 1.0f,  1.0f,  1.0f};
    m_quad = createRef<VertexArray>(GL_TRIANGLES);
    Ref<VertexBuffer> buffer =
        createRef<VertexBuffer>(quadVertices, sizeof(quadVertices));
    buffer->setLayout({{ShaderDataType::Float2, "aPos"},
                       {ShaderDataType::Float2, "aTexCoords"}});
    m_quad->addVertexBuffer(buffer);

    FrameBufferSpecification spec;
    spec.width = width;
    spec.height = height;
    spec.attachmentsSpecs = {{FramebufferTextureFormat::RGB}};
    m_frameBuffer = createScope<FrameBuffer>(spec);


    //TODO: attach render buffer object
    spec.samples = samples;
    spec.attachmentsSpecs = {{FramebufferTextureFormat::RGB},
                             {FramebufferTextureFormat::DEPTH24STENCIL8}};
    m_multiSampleFramebuffer = createScope<FrameBuffer>(spec);
}
void ScreenLayer::begin() const { m_multiSampleFramebuffer->bind(); }

void ScreenLayer::end() const {
    // copy
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_multiSampleFramebuffer->getId());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frameBuffer->getId());
    glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);
    m_multiSampleFramebuffer->unbind();

    // draw
    m_shader->bind();
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, getScreenTexture());
    Ref<VertexBuffer> buffer = m_quad->getVertexBuffers()[0];
    std::size_t cnt = buffer->getSize() / buffer->getLayout().getStride();
    m_quad->bind();
    glDrawArrays(m_quad->getType(), 0, cnt);
    glEnable(GL_DEPTH_TEST);
    m_shader->unbind();
}

uint32_t ScreenLayer::getScreenTexture() const {
    return m_frameBuffer->getColorAttachmentId(0);
}

void ScreenLayer::onResize(int width, int height) {
    if (width != m_width || height != m_height) {
        m_frameBuffer->resize(width, height);
        m_multiSampleFramebuffer->resize(width, height);
        m_width = width;
        m_height = height;
    }
}

}  // namespace te
