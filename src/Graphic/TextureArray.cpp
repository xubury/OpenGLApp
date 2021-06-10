#include "Graphic/TextureArray.hpp"
#include "Graphic/OpenGL.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/Log.hpp"

namespace te {

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

void TextureArray::prepare(const Ref<Shader> &shader) const {
    for (std::size_t i = size(); i < 32; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    shader->setFloat("uMaterial.shininess", 64);
    uint32_t ambient = 0;
    uint32_t diffuse = 0;
    uint32_t specular = 0;
    std::size_t i = 0;
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
