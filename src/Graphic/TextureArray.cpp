#include <Graphic/TextureArray.hpp>
#include <ResourceManager.hpp>

static ResourceManager<std::string, Texture> s_loadedTexture;

bool TextureArray::loadFromFile(const std::string &path,
                                Texture::TextureType textureType) {
    m_list.emplace_back(&s_loadedTexture.getOrLoad(path, path, textureType));
    return true;
}

const Texture &TextureArray::at(std::size_t id) const { return *m_list[id]; }

std::size_t TextureArray::size() const { return m_list.size(); }
