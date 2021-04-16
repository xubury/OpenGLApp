#include <Graphic/TextureList.hpp>

ResourceManager<std::string, Texture> TextureList::loadedTexture;

bool TextureList::loadFromFile(const std::string &path,
                               Texture::TextureType textureType) {
    m_list.emplace_back(&loadedTexture.getOrLoad(path, path, textureType));
    return true;
}

Texture &TextureList::at(std::size_t id) { return *m_list[id]; }

const Texture &TextureList::at(std::size_t id) const { return *m_list[id]; }

std::size_t TextureList::size() const { return m_list.size(); }
