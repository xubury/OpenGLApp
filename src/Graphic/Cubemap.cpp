#include "Graphic/Cubemap.hpp"
#include "Graphic/OpenGL.hpp"
#include "Graphic/Image.hpp"

namespace te {

Cubemap::Cubemap() { glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_textureId); }

Cubemap::~Cubemap() { glDeleteTextures(1, &m_textureId); }

void Cubemap::setParameters(const TextureParameter &params) {
    bind();
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,
                    params.filtering);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
                    params.filtering);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, params.warp);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, params.warp);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, params.warp);
}

bool Cubemap::loadFromFile(const std::vector<std::string> &path) {
    for (unsigned int i = 0; i < path.size(); i++) {
        Image image;
        image.loadFromFile(path[i]);
        if (!image.valid()) {
            return false;
        }
        GLenum format = image.getGLFormat();
        GLenum type = image.GetGLType();
        bind();
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format,
                     image.width(), image.height(), 0, format, type,
                     image.data());
    }
    unbind();
    return true;
}

void Cubemap::bind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId); }

void Cubemap::unbind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, 0); }

}  // namespace te
