#ifndef TEXTURE_LIST_HPP
#define TEXTURE_LIST_HPP

#include <vector>
#include <unordered_map>
#include "Texture.hpp"

class TextureList {
   public:
    TextureList() = default;

    bool loadFromFile(const std::string& path,
                      Texture::TextureType textureType);

    Texture& operator[](std::size_t id);

    Texture& at(std::size_t id);

    const Texture& at(std::size_t id) const;

    Texture* data();

    const Texture* data() const;

    std::size_t size() const;

   private:
    std::vector<Texture> m_list;
    static std::unordered_map<std::string, Texture> loadedTexture;
};

#endif
