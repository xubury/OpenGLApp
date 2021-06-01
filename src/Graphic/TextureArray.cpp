#include <Graphic/TextureArray.hpp>
#include <ResourceManager.hpp>

static ResourceManager<std::string, Texture> s_loadedTexture;

void TextureArray::loadFromFile(const std::string &path,
                                Texture::TextureType textureType) {
    m_list.emplace_back(s_loadedTexture.getOrLoad(path, path, textureType));
}

void TextureArray::loadFromValue(const glm::vec3 &value,
                                 Texture::TextureType textureType) {
    m_list.emplace_back(createRef<Texture>());
    m_list.back()->load(value, textureType);
}

std::size_t TextureArray::size() const { return m_list.size(); }

const std::list<Ref<Texture>> &TextureArray::getList() const { return m_list; }
