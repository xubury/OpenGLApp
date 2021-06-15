#ifndef TEXTURE_PARAMETER_HPP
#define TEXTURE_PARAMETER_HPP

#include "Graphic/OpenGL.hpp"

namespace te {

struct TextureParameter {
    GLenum warp;
    GLenum filtering;
    float borderColor[4];
    TextureParameter() : warp(GL_CLAMP_TO_EDGE), filtering(GL_LINEAR) {}
    template <typename... T>
    TextureParameter(GLenum warp, GLenum filtering, T... color)
        : warp(warp), filtering(filtering), borderColor{color...} {}
};

}  // namespace te

#endif /* TEXTURE_PARAMETER_HPP */
