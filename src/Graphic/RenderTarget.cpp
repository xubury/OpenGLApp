#include <Graphic/RenderTarget.hpp>

#include <Graphic/OpenGL.hpp>

#include <Graphic/Shader.hpp>
#include <Graphic/TextureArray.hpp>
#include <Graphic/BufferObject.hpp>
#include <Graphic/CameraBase.hpp>
#include <iostream>

RenderTarget::RenderTarget() : m_textures(nullptr) {}

void RenderTarget::draw(const Drawable &drawable, const RenderStates &states) {
    drawable.draw(*this, states);
}

void RenderTarget::draw(const BufferObject &buffer,
                        const RenderStates &states) {
    assert(buffer.isInit());

    applyShader(states.shader);
    applyCamera(states.camera);
    applyTransform(states.transform);
    applyTexture(states.textures);

    buffer.drawPrimitive();
}

void RenderTarget::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderTarget::applyCamera(const CameraBase *camera) {
    assert(camera != nullptr);
    glViewport(camera->getViewportX(), camera->getViewportY(),
               camera->getViewportWidth(), camera->getViewportHeight());
    m_shader->setMat4("projection", camera->getProjection());
    m_shader->setMat4("view", camera->getView());
}

void RenderTarget::applyShader(const Shader *shader) {
    assert(shader != nullptr);
    m_shader = shader;
    m_shader->use();
}

void RenderTarget::applyTransform(const glm::mat4 &transform) {
    m_shader->setMat4("model", transform);
}

void RenderTarget::applyTexture(const TextureArray *textures) {
    if (textures == nullptr || textures == m_textures) return;
    if (m_textures != nullptr) {
        // clean up old textures
        std::size_t size = m_textures->size();
        for (std::size_t i = 0; i < size; ++i) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
    m_textures = textures;
    m_shader->setFloat("material.shininess", 64);
    uint32_t ambient = 0;
    uint32_t diffuse = 0;
    uint32_t specular = 0;
    std::size_t size = m_textures->size();
    for (std::size_t i = 0; i < size; ++i) {
        std::string name;
        if (m_textures->at(i).getType() == Texture::AMBIENT) {
            name = "material.ambient" + std::to_string(ambient++);
        } else if (m_textures->at(i).getType() == Texture::DIFFUSE) {
            name = "material.diffuse" + std::to_string(diffuse++);
        } else if (m_textures->at(i).getType() == Texture::SPECULAR) {
            name = "material.specular" + std::to_string(specular++);
        } else {
            std::cout << "Invalid Texture." << std::endl;
        }
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_textures->at(i).id());
        // set the GL_TEXTUREX correspondence
        m_shader->setInt(name, i);
    }
}
