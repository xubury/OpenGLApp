#ifndef SHADOW_LAYER_HPP
#define SHADOW_LAYER_HPP

#include "Core/Layer.hpp"
#include "Graphic/FrameBuffer.hpp"

namespace te {

class ShadowLayer : public Layer {
   public:
    ShadowLayer();

    void onUpdate(const Time &deltaTime) override;

    void onRender() override;

    void onImGuiRender() override;

   private:
    Ref<Shader> m_shader;
    Ref<FrameBuffer> m_framebuffer;
};

}  // namespace te

#endif /* SHADOW_LAYER_HPP */
