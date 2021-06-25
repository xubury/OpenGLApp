#ifndef GBUFFER_LAYER_HPP
#define GBUFFER_LAYER_HPP

#include "Core/Layer.hpp"
#include "Graphic/FrameBuffer.hpp"
#include "Graphic/Shader.hpp"

namespace te {

class GBufferLayer : public Layer {
   public:
    GBufferLayer(int width, int height);

    void onRender() override;

    void onImGuiRender() override;

   private:
    Scope<FrameBuffer> m_gBuffer;
    Scope<Shader> m_shader;
};

}  // namespace te

#endif /* GBUFFER_LAYER_HPP */
