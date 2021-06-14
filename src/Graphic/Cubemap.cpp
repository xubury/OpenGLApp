#include "Graphic/Cubemap.hpp"
#include "Graphic/OpenGL.hpp"
#include "stb_image.h"

namespace te {

Cubemap::Cubemap(const TextureParameter &params) {
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_textureId);
    bind();
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, params.filtering);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, params.filtering);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, params.warp);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, params.warp);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, params.warp);
}

Cubemap::~Cubemap() { glDeleteTextures(1, &m_textureId); }

bool Cubemap::loadFromFile(const std::vector<std::string> &path) {
    int width, height, nrChannels;
    unsigned char *data;
    for (unsigned int i = 0; i < path.size(); i++) {
        data = stbi_load(path[i].c_str(), &width, &height, &nrChannels, 0);
        if (data == nullptr) return false;
        bind();
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width,
                     height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    unbind();
    return true;
}

void Cubemap::bind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId); }

void Cubemap::unbind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, 0); }

}  // namespace te
