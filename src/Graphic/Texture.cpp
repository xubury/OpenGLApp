#include <glad/glad.h>
#include <iostream>
#include "Graphic/Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture() : m_id{0} {}

Texture::~Texture() { glDeleteTextures(TEXTURE_COUNT, m_id); }

uint32_t Texture::id(TextureType type) const { return m_id[type]; }

bool Texture::loadFromFile(const std::string&) { return true; }

bool Texture::loadTexture(const std::string& path, TextureType textureType,
                          int type) {
    stbi_set_flip_vertically_on_load(true);
    if (m_id[textureType] == 0) glGenTextures(1, m_id + textureType);
    glBindTexture(GL_TEXTURE_2D, m_id[textureType]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int texWidth;
    int texHeight;
    int nChannels;

    uint8_t* data =
        stbi_load(path.c_str(), &texWidth, &texHeight, &nChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, type, texWidth, texHeight, 0, type,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture." << std::endl;
        return false;
    }
    stbi_image_free(data);
    return true;
}
