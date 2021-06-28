#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Graphic/Shader.hpp"
#include "Graphic/VertexArray.hpp"
#include "Graphic/Camera.hpp"
#include "Graphic/LightBase.hpp"
#include "Graphic/FrameBuffer.hpp"
#include "Graphic/Material.hpp"

namespace te {

class TE_API Renderer {
   public:
    static void init();

    static void beginScene(const Camera &camera,
                           const FrameBuffer *framebuffer = nullptr);

    static void endScene();

    static void beginShadowCast(const glm::mat4 &lightSpaceMatrix,
                                const glm::vec3 &lightDir,
                                const LightBase &light,
                                const Ref<FrameBuffer> &framebuffer);

    static void endShadowCast();

    static void beginGBuffer(const Camera &camera,
                             const FrameBuffer *framebuffer, uint32_t gPosition,
                             uint32_t gNormal, uint32_t gAlbedoSpec,
                             uint32_t gAmbient);

    static void endGBuffer();

    static void submit(const Shader &shader, const VertexArray &vertexArray,
                       GLenum type, bool indexed,
                       const glm::mat4 &transform = glm::mat4(1.0),
                       const Material *material = nullptr);
    static void clear(float r = 0.1f, float g = 0.2f, float b = 0.3f,
                      float a = 1.f);

   private:
    friend class Primitive;

    static void prepareTextures(const Shader &shader, const Material *material);
    enum class RenderState {
        RENDER_NONE,
        RENDER_SCENE,
        RENDER_SHADOW,
        RENDER_GBUFFER
    };

    struct SceneData {
        Ref<UniformBuffer> cameraUBO;
        Ref<UniformBuffer> lightUBO;
        glm::vec2 viewportSize;
        uint32_t shadowMap = 0;
        uint32_t gPosition = 0;
        uint32_t gNormal = 0;
        uint32_t gAlbedoSpec = 0;
        uint32_t gAmbient = 0;
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
