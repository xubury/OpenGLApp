#include "Graphic/Material.hpp"
#include "Graphic/OpenGL.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/Log.hpp"

namespace te {

static ResourceManager<std::string, Texture> s_loadedTexture;

void Material::loadFromFile(const std::string &path,
                            Texture::Type textureType) {
    m_list.emplace_back(s_loadedTexture.getOrLoad(path, path, textureType));
}

void Material::loadFromValue(const glm::vec3 &value,
                             Texture::Type textureType) {
    m_list.emplace_back(createRef<Texture>());
    m_list.back()->loadFromValue(value, textureType);
}

std::size_t Material::size() const { return m_list.size(); }

const std::list<Ref<Texture>> &Material::getList() const { return m_list; }

}  // namespace te
