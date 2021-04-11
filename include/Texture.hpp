#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

class Texture {
   public:
    int id;
    bool loadFromFile(const std::string& path, int type);
};
#endif
