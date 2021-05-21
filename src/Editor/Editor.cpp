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

void Editor::computeMVP() {
    auto entity = context.entities->getPtr(m_activeEntityId);
    m_mvp = context.camera->getProjection() * context.camera->getView() *
            entity->component<Transform>()->getTransform();
}

glm::vec3 Editor::computeLocalToSrceen(const glm::vec3& localPos) {
    glm::vec4 clipPos = m_mvp * glm::vec4(localPos, 1.0f);
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

void Editor::renderAxis(const glm::vec2& origin, const glm::vec3& dir,
                        ImU32 color, float thickness) {
    glm::vec3 axis = computeLocalToSrceen(dir);
    if (axis.z - 1 < std::numeric_limits<float>::epsilon()) {
        m_drawList->AddLine(
            ImVec2(m_renderOrigin.x + origin.x, m_renderOrigin.y + origin.y),
            ImVec2(m_renderOrigin.x + axis.x, m_renderOrigin.y + axis.y), color,
            thickness);
    }
}

void Editor::renderModelAxes() {
    glm::vec3 origin = computeLocalToSrceen(glm::vec3(0));
    if (origin.z - 1 < std::numeric_limits<float>::epsilon()) {
        renderAxis(origin, glm::vec3(1, 0, 0), 0xFF0000FF);
        renderAxis(origin, glm::vec3(0, 1, 0), 0xFF00FF00);
        renderAxis(origin, glm::vec3(0, 0, 1), 0xFFFF0000);
    }
}

void Editor::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    computeMVP();

    ImGui::Begin("Settings");
    {
        ImGui::SetWindowSize(ImVec2(300, 600));
        ImGui::SetWindowPos(ImVec2(0, 0));
        if (ImGui::TreeNode("Camera")) {
            ImGui::TextColored(ImVec4(1, 1, 1, 1), "Camera Settings");
            drawTransformSheet(*context.camera->component<Transform>().get());
            context.camera->updateView();
            ImGui::Separator();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Entity Components")) {
            auto entity = context.entities->getPtr(m_activeEntityId);
            ImGui::Text("Name: %s", entity->getName().c_str());
            drawTransformSheet(*entity->component<Transform>().get());
            ImGui::Separator();
            ImGui::TreePop();
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
