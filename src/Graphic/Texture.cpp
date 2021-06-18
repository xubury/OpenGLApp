#include "Graphic/OpenGL.hpp"
#include "Core/Log.hpp"
#include "Graphic/Texture.hpp"
#include "stb_image.h"
#include <iostream>

namespace te {

Texture::Texture() : m_id(0) {
    glGenTextures(1, &m_id);
    bind();
}

Texture::~Texture() { glDeleteTextures(1, &m_id); }

bool Texture::loadFromFile(const std::string &path,
                           const TextureParameter &params) {
    stbi_set_flip_vertically_on_load(true);
    int texWidth;
    int texHeight;
    int nChannels;

    uint8_t *data =
        stbi_load(path.c_str(), &texWidth, &texHeight, &nChannels, 0);
    GLenum type = nChannels == 3 ? GL_RGB : GL_RGBA;
    if (data) {
        bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.warp);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, params.warp);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.filtering);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.filtering);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR,
                         params.borderColor);
        glTexImage2D(GL_TEXTURE_2D, 0, type, texWidth, texHeight, 0, type,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        TE_CORE_ERROR("Failed to load texture!");
        return false;
    }
    stbi_image_free(data);
    unbind();
    return true;
}

void Texture::loadFromValue(const glm::vec3 &value,
                            const TextureParameter &params) {
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.warp);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, params.warp);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.filtering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.filtering);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR,
                     params.borderColor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_FLOAT, &value.x);
    glGenerateMipmap(GL_TEXTURE_2D);
    unbind();
}

void Texture::bind() const { glBindTexture(GL_TEXTURE_2D, m_id); }

void Texture::unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

uint32_t Texture::getId() const { return m_id; }

}  // namespace te
