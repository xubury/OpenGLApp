#ifndef CUBEMAP_HPP
#define CUBEMAP_HPP

#include <vector>
#include <string>
#include "Graphic/TextureParameter.hpp"

namespace te {

class Cubemap {
   public:
    Cubemap();

    ~Cubemap();

    Cubemap(const Cubemap &) = delete;

    Cubemap &operator=(const Cubemap &) = delete;

    void setParameters(const TextureParameter &params);

    bool loadFromFile(const std::vector<std::string> &path);

    void bind() const;

    void unbind() const;

   private:
    uint32_t m_textureId;
};

}  // namespace te

#endif /* CUBEMAP_HPP */
