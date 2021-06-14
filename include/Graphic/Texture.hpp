#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Core/Export.hpp"
#include "Graphic/TextureParameter.hpp"
#include <glm/glm.hpp>
#include <string>

namespace te {

class TE_API Texture {
   public:
    enum Type {
        TEXTURE_NONE,
        TEXTURE_AMBIENT,
        TEXTURE_DIFFUSE,
        TEXTURE_SPECULAR
    };

    Texture(const TextureParameter &params = TextureParameter());

    ~Texture();

    Texture(const Texture &) = delete;

    Texture &operator=(const Texture &) = delete;

    bool loadFromFile(const std::string &path, Type textureType);

    void loadFromValue(const glm::vec3 &value, Type textureType);

    Type getType() const;

    void bind() const;

    void unbind() const;

    uint32_t getId() const;
   private:
    uint32_t m_id;

    Type m_type;
};

}  // namespace te

#endif
