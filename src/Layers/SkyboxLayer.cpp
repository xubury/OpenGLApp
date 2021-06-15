#include "Layers/SkyboxLayer.hpp"
#include "Apps/Application.hpp"
#include "Graphic/Renderer.hpp"
#include "Graphic/Camera.hpp"

namespace te {

SkyboxLayer::SkyboxLayer()
    : Layer("Skybox layer"),
      m_cubemap(TextureParameter(GL_CLAMP_TO_EDGE, GL_LINEAR)) {
    const char *vertex = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    layout (std140) uniform Camera
    {
        mat4 uProjectionView;
        vec3 uViewPos;
    };

    out vec3 texCoord;
    void main() {
        texCoord = aPos;
        vec4 pos = uProjectionView * vec4(aPos, 1.0f);
        gl_Position = pos.xyww;
    }
    )";
    const char *fragment = R"(
        #version 330 core
        out vec4 fragColor;
        in vec3 texCoord;

        uniform samplerCube skybox;
        void main() {
            fragColor = texture(skybox, texCoord);
        }
    )";
    m_shader = createRef<Shader>();
    m_shader->compile(vertex, fragment);

    float skyboxVertices[] = {
        // positions
        -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
        1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

        -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
        -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

        1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
        1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};
    m_vertexArray = createRef<VertexArray>();
    Ref<VertexBuffer> buffer =
        createRef<VertexBuffer>(skyboxVertices, sizeof(skyboxVertices));
    buffer->setLayout({{ShaderDataType::Float3, "aPos"}});
    m_vertexArray->addVertexBuffer(buffer);
    std::vector<std::string> faces = {
        "resources/skybox/right.jpg", "resources/skybox/left.jpg",
        "resources/skybox/top.jpg",   "resources/skybox/bottom.jpg",
        "resources/skybox/front.jpg", "resources/skybox/back.jpg"};
    m_cubemap.loadFromFile(faces);
    m_shader->bind();
    m_shader->setInt("skybox", 0);
    glActiveTexture(GL_TEXTURE0);
    m_cubemap.bind();
}

void SkyboxLayer::onRender() {
    glDepthFunc(GL_LEQUAL);
    Ref<Camera> cam = Application::instance().getMainCamera();
    glm::vec3 pos = cam->getPosition();
    cam->setPosition(glm::vec3(0));
    Renderer::beginScene(*cam, Application::instance().getFramebuffer());
    Renderer::submit(*m_shader, *m_vertexArray, GL_TRIANGLES, false);
    Renderer::endScene();
    cam->setPosition(pos);
    glDepthFunc(GL_LEQUAL);
}
}  // namespace te
