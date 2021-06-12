#include "Graphic/ModelTextures.hpp"
#include "Graphic/OpenGL.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/Log.hpp"

namespace te {

static ResourceManager<std::string, Texture> s_loadedTexture;

void ModelTextures::loadFromFile(const std::string &path,
                                Texture::TextureType textureType) {
    m_list.emplace_back(s_loadedTexture.getOrLoad(path, path, textureType));
}

void ModelTextures::loadFromValue(const glm::vec3 &value,
                                 Texture::TextureType textureType) {
    m_list.emplace_back(createRef<Texture>());
    m_list.back()->loadFromValue(value, textureType);
}

std::size_t ModelTextures::size() const { return m_list.size(); }

const std::list<Ref<Texture>> &ModelTextures::getList() const { return m_list; }

void ModelTextures::prepare(const Ref<Shader> &shader) const {
    shader->bind();
    const int textureReserved = 1;
    shader->setFloat("uMaterial.shininess", 64);
    uint32_t ambient = 0;
    uint32_t diffuse = 0;
    uint32_t specular = 0;
    std::size_t i = textureReserved;
    for (const auto &texture : getList()) {
        std::string name;
        if (texture->getType() == Texture::AMBIENT) {
            name = "uMaterial.ambient" + std::to_string(ambient++);
        } else if (texture->getType() == Texture::DIFFUSE) {
            name = "uMaterial.diffuse" + std::to_string(diffuse++);
        } else if (texture->getType() == Texture::SPECULAR) {
            name = "uMaterial.specular" + std::to_string(specular++);
        } else {
            TE_CORE_WARN("Invalid Texture");
        }
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texture->id());
        // set the GL_TEXTUREX correspondence
        shader->setInt(name, i);
        ++i;
    }
}

}  // namespace te
