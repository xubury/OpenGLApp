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

void RenderTarget::beginScene(const Shader &shader, const CameraBase &camera,
                              const std::list<LightBase *> &lights) {
    applyShader(shader);
    glViewport(camera.getViewportX(), camera.getViewportY(),
               camera.getViewportWidth(), camera.getViewportHeight());
    clear();
    shader.setMat4("uProjection", camera.getProjection());
    shader.setMat4("uView", camera.getView());

    glActiveTexture(GL_TEXTURE0);
    
    // reserve texture for depth map
    m_textureReserved = lights.size();
    // handle multiple lights
    auto light = *lights.begin();
    glBindTexture(GL_TEXTURE_2D, light->getShadowBuffer().getDepthMapTexture());
    shader.setInt("uDepthMap", 0);

    shader.setMat4("uLightSpaceMatrix", light->getLightSpaceMatrix());
    shader.setVec3("uDirLight.direction", light->getDirection());
    shader.setVec3("uDirLight.ambient", light->amibent);
    shader.setVec3("uDirLight.diffuse", light->diffuse);
    shader.setVec3("uDirLight.specular", light->specular);
}

void RenderTarget::endScene() {}

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

void RenderTarget::applyShader(const Shader &shader) {
    m_shader = &shader;
    m_shader->use();
}

void RenderTarget::applyTransform(const glm::mat4 &transform) {
    assert(m_shader != nullptr);
    m_shader->setMat4("uModel", transform);
}

void RenderTarget::applyTexture(const TextureArray *textures) {
    assert(m_shader != nullptr);
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
