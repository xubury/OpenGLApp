#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Graphic/GlResource.hpp"

#include <string>

class Texture : public GlResource {
   public:
    enum TextureType { DIFFUSE, SPECULAR };

    Texture();

    bool loadFromFile(const std::string &path, TextureType textureType);

    uint32_t id() const;

    TextureType getType() const;

   private:
    void destroy() override;

    uint32_t m_id;

    TextureType m_type;
};

#endif
