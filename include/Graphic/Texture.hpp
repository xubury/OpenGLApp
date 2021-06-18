#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Core/Export.hpp"
#include "Graphic/TextureParameter.hpp"
#include <glm/glm.hpp>
#include <string>

namespace te {

class TE_API Texture {
   public:
    Texture();

    ~Texture();

    Texture(const Texture &) = delete;

    Texture &operator=(const Texture &) = delete;

    bool loadFromFile(const std::string &path, const TextureParameter &params);

    void loadFromValue(const glm::vec3 &value, const TextureParameter &params);

    void bind() const;

    void unbind() const;

    uint32_t getId() const;

   private:
    uint32_t m_id;
};

}  // namespace te

#endif
