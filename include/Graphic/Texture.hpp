#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Core/Export.hpp"
#include <glm/glm.hpp>
#include <string>

namespace te {

class TE_API Texture {
   public:
    enum TextureType { AMBIENT, DIFFUSE, SPECULAR };

    Texture();

    ~Texture();

    Texture(const Texture &) = delete;

    Texture &operator=(const Texture &) = delete;

    bool load(const std::string &path, TextureType textureType);

    bool load(const glm::vec3 &value, TextureType textureType);

    uint32_t id() const;

    TextureType getType() const;

   private:
    uint32_t m_id;

    TextureType m_type;
};

}  // namespace te

#endif
