#ifndef TEXTURE_ARRAY_HPP
#define TEXTURE_ARRAY_HPP

#include <Graphic/Export.hpp>
#include <Graphic/Texture.hpp>
#include <vector>

class GRAPHIC_API TextureArray {
   public:
    bool loadFromFile(const std::string& path,
                      Texture::TextureType textureType);

    Texture& at(std::size_t id);

    const Texture& at(std::size_t id) const;

    std::size_t size() const;

   private:
    std::vector<const Texture*> m_list;
};

#endif
