#ifndef GBUFFER_LAYER_HPP
#define GBUFFER_LAYER_HPP

#include "Core/Layer.hpp"
#include "Graphic/FrameBuffer.hpp"
#include "Graphic/Shader.hpp"
#include "Graphic/VertexArray.hpp"

namespace te {

class GBufferLayer : public Layer {
   public:
    GBufferLayer(int width, int height);

    void onRender() override;

    void onUpdate(const Time &deltaTime) override;

    void onImGuiRender() override;

    void onEventPoll(const Event &event) override;
   private:
    Scope<FrameBuffer> m_multiSampledGBuffer;
    Scope<FrameBuffer> m_gBuffer;
    Scope<Shader> m_gBufferShader;
};

}  // namespace te

#endif /* GBUFFER_LAYER_HPP */
