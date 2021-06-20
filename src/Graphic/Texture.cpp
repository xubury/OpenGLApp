#include "Graphic/OpenGL.hpp"
#include "Core/Log.hpp"
#include "Graphic/Texture.hpp"
#include <iostream>

namespace te {

Texture::Texture() : m_id(0) {
    glGenTextures(1, &m_id);
    bind();
}

Texture::~Texture() { glDeleteTextures(1, &m_id); }

bool Texture::loadFromFile(const std::string &path) {
    Image image;
    image.loadFromFile(path);
    if (image.valid()) {
        GLenum format = image.getGLFormat();
        GLenum type = image.GetGLType();
        bind();
        glTexImage2D(GL_TEXTURE_2D, 0, format, image.width(), image.height(), 0,
                     format, type, image.data());
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        TE_CORE_ERROR("Fail to load texture!");
        return false;
    }
    unbind();
    return true;
}

void Texture::setValue(const glm::vec3 &value) {
    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_FLOAT, &value.x);
    glGenerateMipmap(GL_TEXTURE_2D);
    unbind();
}

void Texture::setParameters(const TextureParameter &params) {
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.warp);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, params.warp);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.filtering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.filtering);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR,
                     params.borderColor);
    unbind();
}

void Texture::bind() const { glBindTexture(GL_TEXTURE_2D, m_id); }

void Texture::unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

uint32_t Texture::getId() const { return m_id; }

}  // namespace te
