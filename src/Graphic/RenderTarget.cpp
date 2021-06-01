#include <Graphic/RenderTarget.hpp>

#include <Graphic/OpenGL.hpp>

#include <Graphic/Shader.hpp>
#include <Graphic/TextureArray.hpp>
#include <Graphic/BufferObject.hpp>
#include <Graphic/CameraBase.hpp>
#include <Graphic/FrameBuffer.hpp>
#include <Graphic/LightBase.hpp>
#include <iostream>

RenderTarget::RenderTarget() : m_textures(nullptr) {}

void RenderTarget::beginScene(const Shader &shader, const CameraBase &camera,
                              const LightBase &light) {
    applyShader(shader);
    glViewport(camera.getViewportX(), camera.getViewportY(),
               camera.getViewportWidth(), camera.getViewportHeight());
    clear();
    shader.setMat4("projection", camera.getProjection());
    shader.setMat4("view", camera.getView());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, light.getShadowBuffer().getDepthMapTexture());
    shader.setInt("depthMap", 0);

    shader.setMat4("lightSpaceMatrix", light.getViewMatirx());
    shader.setVec3("dirLight.direction", light.getDirection());
    shader.setVec3("dirLight.ambient", light.amibent);
    shader.setVec3("dirLight.diffuse", light.diffuse);
    shader.setVec3("dirLight.specular", light.specular);
}

void RenderTarget::beginDepthMap(const LightBase &light) {
    const ShadowBuffer &buffer = light.getShadowBuffer();
    applyShader(buffer.s_shadowShader);
    glViewport(0, 0, buffer.getWidth(), buffer.getHeight());
    glBindFramebuffer(GL_FRAMEBUFFER, buffer.getFrameBuffer());
    glClear(GL_DEPTH_BUFFER_BIT);
    m_shader->setMat4("lightSpaceMatrix", light.getViewMatirx());
}

void RenderTarget::draw(const Drawable &drawable, const RenderStates &states) {
    drawable.draw(*this, states);
}

void RenderTarget::draw(const BufferObject &buffer,
                        const RenderStates &states) {
    assert(buffer.isInit());

    applyTransform(states.transform);
    applyTexture(states.textures);

    buffer.drawPrimitive();
}

void RenderTarget::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderTarget::applyShader(const Shader &shader) {
    m_shader = &shader;
    m_shader->use();
}

void RenderTarget::applyTransform(const glm::mat4 &transform) {
    assert(m_shader != nullptr);
    m_shader->setMat4("model", transform);
}

void RenderTarget::applyTexture(const TextureArray *textures) {
    assert(m_shader != nullptr);
    if (textures == nullptr || textures == m_textures) return;
    if (m_textures != nullptr) {
        // clean up old textures
        std::size_t size = m_textures->size();
        for (std::size_t i = 0; i < size; ++i) {
            glActiveTexture(GL_TEXTURE0 + i + 1);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
    m_textures = textures;
    m_shader->setFloat("material.shininess", 64);
    uint32_t ambient = 0;
    uint32_t diffuse = 0;
    uint32_t specular = 0;
    std::size_t i = 0;
    for (const auto &texture : m_textures->getList()) {
        std::string name;
        if (texture->getType() == Texture::AMBIENT) {
            name = "material.ambient" + std::to_string(ambient++);
        } else if (texture->getType() == Texture::DIFFUSE) {
            name = "material.diffuse" + std::to_string(diffuse++);
        } else if (texture->getType() == Texture::SPECULAR) {
            name = "material.specular" + std::to_string(specular++);
        } else {
            std::cout << "Invalid Texture." << std::endl;
        }
        glActiveTexture(GL_TEXTURE0 + i + 1);
        glBindTexture(GL_TEXTURE_2D, texture->id());
        // set the GL_TEXTUREX correspondence
        m_shader->setInt(name, i + 1);
        ++i;
    }
}
