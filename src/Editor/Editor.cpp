#include <Editor/Editor.hpp>
#include <Graphic/FrameBuffer.hpp>
#include <Entity/Camera.hpp>
#include <Graphic/RenderWindow.hpp>
#include <Component/Transform.hpp>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

Editor& Editor::instance() {
    static Editor s_instance;
    return s_instance;
}

Editor::Editor() : m_activeEntityId(0) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

static void drawTransformSheet(Transform& trans) {
    ImGui::Separator();
    ImGui::Text("Transform");
    glm::vec3 eulerAngle = trans.getEulerAngle();
    if (ImGui::InputFloat3("Rotation", &eulerAngle[0], "%.3f",
                           ImGuiInputTextFlags_EnterReturnsTrue)) {
        trans.setEulerAngle(eulerAngle);
    }

    glm::vec3 pos = trans.getPosition();
    if (ImGui::InputFloat3("Position", &pos[0], "%.3f",
                           ImGuiInputTextFlags_EnterReturnsTrue)) {
        trans.setPosition(pos);
    }
}

void Editor::computeProjectionView() {
    m_projectionView =
        context.camera->getProjection() * context.camera->getView();
}

glm::vec3 Editor::computeWorldToSrceen(const glm::vec3& worldPos) {
    glm::vec4 clipPos = m_projectionView * glm::vec4(worldPos, 1.0f);
    clipPos /= clipPos.w;

    float width = context.camera->getWidth();
    float height = context.camera->getHeight();
    float x = context.camera->getX();
    float y = context.camera->getY();

    glm::vec3 screenPos;
    screenPos.x = (clipPos.x + 1) * 0.5 * width + x;
    screenPos.y = (1 - clipPos.y) * 0.5 * height + y;
    screenPos.z = clipPos.z;
    return screenPos;
}

void Editor::renderFps() {
    std::string frameRate =
        "FPS:" + std::to_string(context.window->getFrameRate());
    m_drawList->AddText(m_renderOrigin, 0xFFFFFFFF, frameRate.c_str());
}

void Editor::renderAxis(const glm::vec2& origin, const glm::vec2& axis,
                        ImU32 color, float thickness) {
    m_drawList->AddLine(
        ImVec2(m_renderOrigin.x + origin.x, m_renderOrigin.y + origin.y),
        ImVec2(m_renderOrigin.x + axis.x, m_renderOrigin.y + axis.y), color,
        thickness);
}

void Editor::renderModelAxes() {
    auto entity = context.entities->getPtr(m_activeEntityId);
    auto model = entity->component<Transform>();
    glm::vec3 origin = computeWorldToSrceen(model->getPosition());
    // backface culling
    if (origin.z - 1 < std::numeric_limits<float>::epsilon()) {
        // draw x
        glm::vec3 xAxis = computeWorldToSrceen(model->getRight());
        if (xAxis.z - 1 < std::numeric_limits<float>::epsilon()) {
            renderAxis(origin, xAxis, 0xFF0000FF);
        }
        // draw y
        glm::vec3 yAxis = computeWorldToSrceen(model->getUp());
        if (yAxis.z - 1 < std::numeric_limits<float>::epsilon()) {
            renderAxis(origin, yAxis, 0xFF00FF00);
        }
        // draw z
        glm::vec3 zAxis = computeWorldToSrceen(model->getFront());
        if (zAxis.z - 1 < std::numeric_limits<float>::epsilon()) {
            renderAxis(origin, zAxis, 0xFFFF0000);
        }
    }
}

void Editor::renderCameraAxes() {
    float width = 800;
    float height = 600;
    float len = 50.0f;
    auto model = context.camera->component<Transform>();
    glm::vec3 origin(width - len * 1.2, height - len * 1.2, 0);
    // draw x
    glm::vec3 xAxis = model->getRight();
    xAxis.y = -xAxis.y;
    xAxis = origin + xAxis * len;
    renderAxis(origin, xAxis, 0xFF0000FF, 2);
    // draw y
    glm::vec3 yAxis = model->getUp();
    yAxis.y = -yAxis.y;
    yAxis = origin + yAxis * len;
    renderAxis(origin, yAxis, 0xFF00FF00, 2);
    // draw z
    glm::vec3 zAxis = model->getFront();
    zAxis.y = -zAxis.y;
    zAxis = origin + zAxis * len;
    renderAxis(origin, zAxis, 0xFFFF0000, 2);
}

void Editor::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    computeProjectionView();

    ImGui::Begin("Settings");
    {
        ImGui::SetWindowSize(ImVec2(300, 600));
        ImGui::SetWindowPos(ImVec2(0, 0));
        if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::TextColored(ImVec4(1, 1, 1, 1), "Camera Settings");
            drawTransformSheet(*context.camera->component<Transform>().get());
            ImGui::Separator();
        }

        if (ImGui::CollapsingHeader("Entity", ImGuiTreeNodeFlags_DefaultOpen)) {
            auto entity = context.entities->getPtr(m_activeEntityId);
            ImGui::Text("Name: %s", entity->getName().c_str());
            drawTransformSheet(*entity->component<Transform>().get());
            ImGui::Separator();
        }
    }
    ImGui::End();

    ImGui::Begin("GameWindow");
    {
        ImGui::SetWindowSize(ImVec2(800, 600));
        ImGui::SetWindowPos(ImVec2(300, 0));

        ImGui::BeginChild("GameRender");
        m_renderOrigin = ImGui::GetWindowPos();
        m_drawList = ImGui::GetWindowDrawList();

        ImVec2 wsize = ImGui::GetWindowSize();
        // if game window not active, disable camera response
        context.camera->setActive(ImGui::IsWindowFocused() &&
                                  ImGui::IsWindowHovered());
        context.frameBuffer->update(wsize.x, wsize.y);
        context.camera->setSize(wsize.x, wsize.y);
        // Because I use the texture from OpenGL, I need to invert the V
        // from the UV.
        ImGui::Image((void*)(intptr_t)context.frameBuffer->getTextureId(),
                     wsize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::EndChild();
    }
    ImGui::End();

    renderFps();
    renderModelAxes();
    renderCameraAxes();

    ImGui::Render();
    glClearColor(0.3, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::close() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
