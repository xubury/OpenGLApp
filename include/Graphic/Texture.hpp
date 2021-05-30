#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <Graphic/Export.hpp>
#include <string>

class GRAPHIC_API Texture {
   public:
    enum TextureType { DIFFUSE, SPECULAR };

    Texture();

    ~Texture();

    Texture(const Texture &) = delete;

    Texture &operator=(const Texture &) = delete;

    bool loadFromFile(const std::string &path, TextureType textureType);

    uint32_t id() const;

    TextureType getType() const;

   private:
    uint32_t m_id;

    TextureType m_type;
};

#endif
