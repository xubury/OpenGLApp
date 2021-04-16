#ifndef TEXTURE_LIST_HPP
#define TEXTURE_LIST_HPP

#include <vector>
#include <memory>
#include <unordered_map>
#include "Texture.hpp"
#include "ResourceManager.hpp"

class TextureList {
   public:
    bool loadFromFile(const std::string& path,
                      Texture::TextureType textureType);

    Texture& at(std::size_t id);

    const Texture& at(std::size_t id) const;

    std::size_t size() const;

   private:
    std::vector<Texture*> m_list;

    static ResourceManager<std::string, Texture> loadedTexture;
};

#endif
