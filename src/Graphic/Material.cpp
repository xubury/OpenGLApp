#include "Graphic/Material.hpp"
#include "Graphic/OpenGL.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/Log.hpp"

namespace te {

static ResourceManager<std::string, Texture> s_loadedTexture;

void Material::loadFromFile(const std::string& path, Type textureType,
                            const TextureParameter& params) {
    m_list.emplace_back(s_loadedTexture.getOrLoad(path, path, params),
                        textureType);
}

void Material::loadFromValue(const glm::vec3& value, Type textureType,
                             const TextureParameter& params) {
    m_list.emplace_back(createRef<Texture>(), textureType);
    m_list.back().first->loadFromValue(value, params);
}

std::size_t Material::size() const { return m_list.size(); }

const std::list<std::pair<Ref<Texture>, Material::Type>>& Material::getList()
    const {
    return m_list;
}

}  // namespace te
