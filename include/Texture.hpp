#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

class Texture {
   public:
    bool loadFromFile(const std::string& path, int type);

    uint32_t id() const;

   private:
    uint32_t m_id;
};
#endif
