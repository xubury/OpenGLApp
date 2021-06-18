#include "Graphic/Material.hpp"
#include "Graphic/OpenGL.hpp"
#include "Graphic/Image.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/Log.hpp"

namespace te {

void Material::emplace(Ref<Texture> texture, Type textureType) {
    m_list.emplace_back(texture, textureType);
}

void Material::loadFromFile(const std::string& path, Type textureType,
                            const TextureParameter& params) {
    m_list.emplace_back(createRef<Texture>(), textureType);
    m_list.back().first->loadFromFile(path);
    m_list.back().first->setParameters(params);
}

void Material::loadFromValue(const glm::vec3& value, Type textureType,
                             const TextureParameter& params) {
    m_list.emplace_back(createRef<Texture>(), textureType);
    m_list.back().first->setValue(value);
    m_list.back().first->setParameters(params);
}

std::size_t Material::size() const { return m_list.size(); }

const std::list<std::pair<Ref<Texture>, Material::Type>>& Material::getList()
    const {
    return m_list;
}

}  // namespace te
