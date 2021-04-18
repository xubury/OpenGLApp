#include <Graphic/TextureArray.hpp>
#include <iostream>

ResourceManager<std::string, Texture> TextureArray::loadedTexture;

bool TextureArray::loadFromFile(const std::string &path,
                                Texture::TextureType textureType) {
    m_list.emplace_back(&loadedTexture.getOrLoad(path, path, textureType));
    return true;
}

Texture &TextureArray::at(std::size_t id) { return *m_list[id]; }

const Texture &TextureArray::at(std::size_t id) const { return *m_list[id]; }

std::size_t TextureArray::size() const { return m_list.size(); }
