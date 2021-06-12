#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Graphic/Shader.hpp"
#include "Graphic/VertexArray.hpp"
#include "Graphic/Camera.hpp"
#include "Graphic/LightBase.hpp"
#include "Graphic/FrameBuffer.hpp"

namespace te {

class Renderer {
   public:
    static void init();

    static void beginScene(const Ref<Camera> &camera,
                           const Ref<FrameBuffer> &framebuffer = nullptr);

    static void beginShadowCast(const Ref<FrameBuffer> &framebuffer);

    static void endShadowCast();

    static void setShadowCaster(LightBase *light);

    static LightBase *getLightSource() { return s_sceneData.shadowCaster; }

    static void endScene();

    static void submit(const Ref<Shader> &shader,
                       const Ref<VertexArray> &vertexArray, GLenum type,
                       bool indexed,
                       const glm::mat4 &transform = glm::mat4(1.0));
    static void clear(float r = 0.1f, float g = 0.2f, float b = 0.3f,
                      float a = 1.f);

   private:
    struct SceneData {
        glm::mat4 projection;
        glm::mat4 view;

        LightBase *shadowCaster;
    };

    static SceneData s_sceneData;
};
}  // namespace te

#endif /* RENDERER_HPP */
