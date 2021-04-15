#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

class Texture {
   public:
    enum TextureType { DIFFUSE, SPECULAR, TEXTURE_COUNT };

   public:
    Texture();

    ~Texture();

    Texture(const Texture&) = delete;

    Texture& operator=(const Texture&) = delete;

    // TODO: make a json/xml that store texture resources' path and load from it
    bool loadFromFile(const std::string& path);

    bool loadTexture(const std::string& path, TextureType textureType,
                     int type);

    uint32_t id(TextureType type) const;

    float getShininess() const;

    void setShininess(float shininess);

   private:
    uint32_t m_id[TEXTURE_COUNT];
    float m_shininess;
};
#endif
