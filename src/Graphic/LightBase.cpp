#include <Graphic/LightBase.hpp>
#include <Graphic/OpenGL.hpp>

const ShadowBuffer &LightBase::getShadowBuffer() const {
    return m_shadowBuffer;
}

ShadowBuffer &LightBase::getShadowBuffer() { return m_shadowBuffer; }
