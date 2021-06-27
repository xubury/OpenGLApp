#ifndef TEXTURE_PARAMETER_HPP
#define TEXTURE_PARAMETER_HPP

#include "Core/Export.hpp"
#include "Graphic/OpenGL.hpp"

namespace te {

struct TE_API TextureParameter {
    // GL_REPEAT GL_MIRRORED_REPEAT GL_CLAMP_TO_EDGE GL_CLAMP_TO_EDGE
    GLenum warp;
    // GL_LINEAR GL_NEAREST
    GLenum filtering;
    float borderColor[4];
    TextureParameter() : warp(GL_CLAMP_TO_EDGE), filtering(GL_LINEAR) {}
    template <typename... T>
    TextureParameter(GLenum warp, GLenum filtering, T... color)
        : warp(warp), filtering(filtering), borderColor{color...} {}
};

}  // namespace te

#endif /* TEXTURE_PARAMETER_HPP */
