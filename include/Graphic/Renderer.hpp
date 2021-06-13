#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Graphic/Shader.hpp"
#include "Graphic/VertexArray.hpp"
#include "Graphic/Camera.hpp"
#include "Graphic/LightBase.hpp"
#include "Graphic/FrameBuffer.hpp"
#include "Graphic/Material.hpp"
#include "Component/ShadowMap.hpp"

namespace te {

class Renderer {
   public:
    static void init();

    static void beginScene(const Ref<Camera> &camera,
                           const Ref<FrameBuffer> &framebuffer = nullptr);

    static void endScene();

    static void beginShadowCast(ShadowMap *light,
                                const Ref<FrameBuffer> &framebuffer);

    static void endShadowCast();

    static void submit(const Ref<Shader> &shader,
                       const Ref<VertexArray> &vertexArray, GLenum type,
                       bool indexed,
                       const glm::mat4 &transform = glm::mat4(1.0),
                       const Ref<Material> &material = nullptr);
    static void clear(float r = 0.1f, float g = 0.2f, float b = 0.3f,
                      float a = 1.f);

   private:
    static void prepareTextures(const Ref<Shader> &shader,
                                const Ref<Material> &material);
    enum class RenderState { RENDER_NONE, RENDER_SCENE, RENDER_SHADOW };

    struct SceneData {
        Ref<UniformBuffer> cameraUBO;
        Ref<UniformBuffer> lightUBO;
        uint32_t shadowMap;
    };

    struct CameraData {
        glm::mat4 projectionView;
        glm::vec3 viewPos;
    };

    struct ShadowData {
        glm::mat4 lightSpaceMatrix;
        glm::vec3 direction;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };

    static SceneData s_sceneData;
    static RenderState s_state;
};
}  // namespace te

#endif /* RENDERER_HPP */
