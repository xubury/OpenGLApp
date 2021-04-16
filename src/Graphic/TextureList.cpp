#include "Graphic/TextureList.hpp"

std::unordered_map<std::string, Texture> TextureList::loadedTexture;

bool TextureList::loadFromFile(const std::string &path,
                               Texture::TextureType textureType) {
    if (loadedTexture.count(path) != 0) {
        m_list.emplace_back(loadedTexture.at(path));
        return true;
    }
    Texture texture;
    if (texture.loadFromFile(path, textureType)) {
        m_list.emplace_back(texture);
        loadedTexture.emplace(path, m_list.back());
        return true;
    }
    return false;
}

Texture &TextureList::operator[](std::size_t id) { return m_list[id]; }

Texture &TextureList::at(std::size_t id) { return m_list[id]; }

const Texture &TextureList::at(std::size_t id) const { return m_list[id]; }

Texture *TextureList::data() { return m_list.data(); }

const Texture *TextureList::data() const { return m_list.data(); }

std::size_t TextureList::size() const { return m_list.size(); }
