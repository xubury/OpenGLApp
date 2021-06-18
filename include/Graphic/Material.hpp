#ifndef TEXTURE_ARRAY_HPP
#define TEXTURE_ARRAY_HPP

#include "Core/Base.hpp"
#include "Core/Export.hpp"
#include "Graphic/Texture.hpp"
#include "Graphic/Shader.hpp"
#include <list>

namespace te {

class TE_API Material {
   public:
    enum Type {
        TEXTURE_NONE,
        TEXTURE_AMBIENT,
        TEXTURE_DIFFUSE,
        TEXTURE_SPECULAR
    };

   public:
    void emplace(Ref<Texture> texture, Type textureType);

    void loadFromFile(const std::string& path, Type textureType,
                      const TextureParameter& params = TextureParameter());

    void loadFromValue(const glm::vec3& value, Type textureType,
                       const TextureParameter& params = TextureParameter());

    std::size_t size() const;

    const std::list<std::pair<Ref<Texture>, Type>>& getList() const;

   private:
    std::list<std::pair<Ref<Texture>, Type>> m_list;
};

}  // namespace te

#endif
