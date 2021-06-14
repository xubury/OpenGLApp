#ifndef GBUFFER_LAYER_HPP
#define GBUFFER_LAYER_HPP

#include "Graphic/FrameBuffer.hpp"

namespace te {

class GBufferLayer {
   public:
    GBufferLayer(int width, int height);
    void begin();
    void end();
    private:
    Scope<FrameBuffer> m_gBuffer;
};

}  // namespace te

#endif /* GBUFFER_LAYER_HPP */
