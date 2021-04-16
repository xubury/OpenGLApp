#include "Graphic/TextureList.hpp"

std::unordered_map<std::string, GlBuffer<Texture>> TextureList::loadedTexture;

bool TextureList::loadFromFile(const std::string &path,
                               Texture::TextureType textureType) {
    if (loadedTexture.count(path) != 0) {
        m_list.emplace_back(loadedTexture.at(path));
        return true;
    }
    GlBuffer<Texture> texture(new Texture);
    if (texture->loadFromFile(path, textureType)) {
        m_list.emplace_back(texture);
        loadedTexture.emplace(path, m_list.back());
        return true;
    }
    return false;
}

Texture &TextureList::operator[](std::size_t id) { return *m_list[id]; }

Texture &TextureList::at(std::size_t id) { return *m_list[id]; }

const Texture &TextureList::at(std::size_t id) const { return *m_list[id]; }

std::size_t TextureList::size() const { return m_list.size(); }
