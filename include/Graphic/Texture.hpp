#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

class Texture {
   public:
    enum TextureType { DIFFUSE, SPECULAR };

    Texture();

    ~Texture();

    bool loadFromFile(const std::string &path, TextureType textureType);

    uint32_t id() const;

    TextureType getType() const;

   private:
    uint32_t m_id;

    TextureType m_type;
};

#endif
