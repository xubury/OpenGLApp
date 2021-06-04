#include "Core/Assert.hpp"

#include <Graphic/RenderTarget.hpp>
#include <Graphic/OpenGL.hpp>

#include <Graphic/Shader.hpp>
#include <Graphic/TextureArray.hpp>
#include <Graphic/BufferObject.hpp>
#include <Graphic/CameraBase.hpp>
#include <Graphic/FrameBuffer.hpp>
#include <Graphic/LightBase.hpp>
#include <iostream>

RenderTarget::RenderTarget() : m_shader(nullptr), m_textures(nullptr) {}

void RenderTarget::beginScene(Ref<Shader> shader,
                              const Ref<CameraBase> &camera) {
    applyShader(shader);
    glViewport(camera->getViewportX(), camera->getViewportY(),
               camera->getViewportWidth(), camera->getViewportHeight());
    clear();
    shader->setMat4("uProjection", camera->getProjection());
    shader->setMat4("uView", camera->getView());
}

void RenderTarget::setLighting(
    const std::vector<const LightBase *> &lights,
    const std::vector<Ref<ShadowBuffer>> &shadowBuffers) {
    // reserve texture for depth map
    m_textureReserved = shadowBuffers.size();
    if (!shadowBuffers.size()) return;
    // TODO: handle multiple lights
    for (int i = 0; i < 1; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, shadowBuffers[i]->getDepthMap());
        m_shader->setInt("uDepthMap", i);

        m_shader->setMat4("uLightSpaceMatrix",
                          lights[i]->getLightSpaceMatrix());
        m_shader->setVec3("uDirLight.direction", lights[i]->getDirection());
        m_shader->setVec3("uDirLight.ambient", lights[i]->amibent);
        m_shader->setVec3("uDirLight.diffuse", lights[i]->diffuse);
        m_shader->setVec3("uDirLight.specular", lights[i]->specular);
    }
}

void RenderTarget::endScene() {
    m_shader->unbind();
    m_shader.reset();
}

void RenderTarget::draw(const BufferObject &buffer,
                        const RenderStates &states) {
    applyTransform(states.transform);
    applyTexture(states.textures);

    buffer.drawPrimitive();
}

void RenderTarget::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderTarget::applyShader(Ref<Shader> shader) {
    m_shader = shader;
    m_shader->bind();
}

void RenderTarget::applyTransform(const glm::mat4 &transform) {
    TE_CORE_ASSERT(m_shader != nullptr,
                   "RenderTarget::applyTransform shader is not specified!");

    m_shader->setMat4("uModel", transform);
}

void RenderTarget::applyTexture(const TextureArray *textures) {
    TE_CORE_ASSERT(m_shader != nullptr,
                   "RenderTarget::applyTransform shader is not specified!");
    if (textures == nullptr || textures == m_textures) return;
    if (m_textures != nullptr) {
        // clean up old textures
        std::size_t size = m_textures->size();
        for (std::size_t i = 0; i < size; ++i) {
            glActiveTexture(GL_TEXTURE0 + i + m_textureReserved);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
    m_textures = textures;
    m_shader->setFloat("uMaterial.shininess", 64);
    uint32_t ambient = 0;
    uint32_t diffuse = 0;
    uint32_t specular = 0;
    std::size_t i = m_textureReserved;
    for (const auto &texture : m_textures->getList()) {
        std::string name;
        if (texture->getType() == Texture::AMBIENT) {
            name = "uMaterial.ambient" + std::to_string(ambient++);
        } else if (texture->getType() == Texture::DIFFUSE) {
            name = "uMaterial.diffuse" + std::to_string(diffuse++);
        } else if (texture->getType() == Texture::SPECULAR) {
            name = "uMaterial.specular" + std::to_string(specular++);
        } else {
            std::cout << "Invalid Texture." << std::endl;
        }
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texture->id());
        // set the GL_TEXTUREX correspondence
        m_shader->setInt(name, i);
        ++i;
    }
}
