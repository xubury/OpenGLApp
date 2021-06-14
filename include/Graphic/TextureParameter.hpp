#ifndef TEXTURE_PARAMETER_HPP
#define TEXTURE_PARAMETER_HPP

#include "Graphic/OpenGL.hpp"

namespace te {

struct TextureParameter {
    GLenum warp;
    GLenum filtering;
    TextureParameter() : warp(GL_REPEAT), filtering(GL_NEAREST) {}
    TextureParameter(GLenum warp, GLenum filtering)
        : warp(warp), filtering(filtering) {}
};

}  // namespace te

#endif /* TEXTURE_PARAMETER_HPP */
