#ifndef TEXTURE_ARRAY_HPP
#define TEXTURE_ARRAY_HPP

#include "Base.hpp"
#include "Graphic/Export.hpp"
#include "Graphic/Texture.hpp"
#include <list>

namespace te {

class GRAPHIC_API TextureArray {
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
