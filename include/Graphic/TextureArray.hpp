#ifndef TEXTURE_ARRAY_HPP
#define TEXTURE_ARRAY_HPP

#include "Core/Base.hpp"
#include "Core/Export.hpp"
#include "Graphic/Texture.hpp"
#include <list>

namespace te {

class TE_API TextureArray {
   public:
    void loadFromFile(const std::string& path,
                      Texture::TextureType textureType);

    void loadFromValue(const glm::vec3& value,
                       Texture::TextureType textureType);

    std::size_t size() const;

    const std::list<std::shared_ptr<Texture>>& getList() const;

   private:
    std::list<Ref<Texture>> m_list;
};

}  // namespace te

#endif
