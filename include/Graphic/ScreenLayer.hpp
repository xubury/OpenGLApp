#ifndef SCREENLAYER_HPP
#define SCREENLAYER_HPP

#include "Graphic/Shader.hpp"
#include "Graphic/VertexArray.hpp"
#include "Graphic/FrameBuffer.hpp"

namespace te {

class ScreenLayer {
   public:
    ScreenLayer(int width, int height, int samples);

    void begin() const;

    void end() const;

    uint32_t getScreenTexture() const;

    void onResize(int width, int height);

   private:
    Ref<Shader> m_shader;
    Ref<VertexArray> m_quad;
    Scope<FrameBuffer> m_frameBuffer;
    Scope<FrameBuffer> m_multiSampleFramebuffer;
    int m_width;
    int m_height;
};

}  // namespace te

#endif /* SCREENLAYER_HPP */
