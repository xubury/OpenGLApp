#include "Layers/ShadowLayer.hpp"
#include "Apps/Application.hpp"
#include "Graphic/Renderer.hpp"
#include "Entity/Light.hpp"
#include "Component/ShadowMap.hpp"

namespace te {

const uint32_t SHADOW_MAP_WIDTH = 4096;
const uint32_t SHADOW_MAP_HEIGHT = 4096;

ShadowLayer::ShadowLayer() : Layer("Shadow layer") {
    FrameBufferSpecification spec;
    spec.width = SHADOW_MAP_WIDTH;
    spec.height = SHADOW_MAP_HEIGHT;
    spec.attachmentsSpecs = {{FramebufferTextureFormat::RGB},
                             {FramebufferTextureFormat::DEPTH32,
                              TextureParameter(GL_CLAMP_TO_BORDER, GL_LINEAR,
                                               1.0f, 1.0f, 1.0f, 1.0f)}};
    m_framebuffer = createRef<FrameBuffer>(spec, false);

    const char *shadowVertex = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aTexCoord;
        layout (location = 2) in vec3 aNormal;
        layout (location = 3) in ivec4 aBoneIds;
        layout (location = 4) in vec4 aWeights;

        struct DirLight {
            vec3 direction;
            vec3 ambient;
            vec3 diffuse;
            vec3 specular;
        };
        const int MAX_BONES = 100;
        const int MAX_BONE_INFLUENCE = 4;
        uniform mat4 uBoneTransform[MAX_BONES];

        layout (std140) uniform Light
        {
            mat4 uLightSpaceMatrix;
            DirLight uDirLight;
        };
        uniform mat4 uModel;
        out float zValue;
        void main() {
            vec4 totalPosition = vec4(0.f);
            bool hasBoneAnim = false;
            for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++) {
                if(aBoneIds[i] == -1)
                    continue;
                if(aBoneIds[i] >= MAX_BONES) {
                    totalPosition = vec4(aPos,1.0f);
                    break;
                }
                vec4 localPosition = uBoneTransform[aBoneIds[i]] * vec4(aPos, 1.0f);
                totalPosition += localPosition * aWeights[i];
                vec3 localNormal = mat3(uBoneTransform[aBoneIds[i]]) * aNormal;
                hasBoneAnim = true;
            }

            if (hasBoneAnim) {
                gl_Position = uLightSpaceMatrix * uModel * totalPosition;
            } else {
                gl_Position = uLightSpaceMatrix * uModel * vec4(aPos, 1.0f);
            }
            zValue = gl_Position.z / gl_Position.w;
            zValue = smoothstep(0, 1.0, zValue);
        }

    )";
    const char *shadowFragment = R"(
        #version 330 core
        out vec4 fragColor;
        in float zValue;
        void main() {
            fragColor = vec4(vec3(zValue), 1.0);
        };
    )";
    m_shader = createRef<Shader>();
    m_shader->compile(shadowVertex, shadowFragment);
}

void ShadowLayer::onUpdate(const Time &) {
    Ref<SceneManager<EntityBase>> scene =
        Application::instance().getActiveScene();
    ShadowMap::Handle shadowMap;
    scene->entities.getByComponents(shadowMap).begin();
    if (!shadowMap.isValid()) return;

    Camera *cam = Application::instance().getMainCamera();
    shadowMap->computeLightSpaceMatrix(*cam);
}

void ShadowLayer::onRender() {
    Ref<SceneManager<EntityBase>> scene =
        Application::instance().getActiveScene();
    ShadowMap::Handle shadowMap;
    scene->entities.getByComponents(shadowMap).begin();
    if (!shadowMap.isValid()) return;

    Renderer::beginShadowCast(shadowMap.get(), m_framebuffer);
    std::size_t size = scene->entities.size();
    for (std::size_t i = 0; i < size; ++i) {
        scene->entities.get(i)->draw(*m_shader);
    }
    Renderer::endShadowCast();
}

void ShadowLayer::onImGuiRender() {
    ImGui::Begin("Shadow map");
    {
        ImVec2 wsize = ImGui::GetWindowSize();
        ImGui::Image((void *)(intptr_t)m_framebuffer->getColorAttachmentId(0),
                     wsize, ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
}

}  // namespace te
