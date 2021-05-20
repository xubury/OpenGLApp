#include <Editor/Editor.hpp>
#include <Graphic/FrameBuffer.hpp>
#include <Graphic/Camera.hpp>
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

Editor::Editor() {
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

void transForm(Transform& trans) {
    ImGui::Text("Transform");
    glm::vec3 eulerAngle = trans.getEulerAngle();
    ImGui::InputFloat3("Rotation", &eulerAngle[0]);
    trans.setEulerAngle(eulerAngle);

    glm::vec3 pos = trans.getPosition();
    ImGui::InputFloat3("Position", &pos[0]);
    trans.setPosition(pos);
}

void Editor::render(EditorContext& context) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Settings");
    {
        ImGui::SetWindowSize(ImVec2(300, 600));
        ImGui::SetWindowPos(ImVec2(0, 0));
        if (ImGui::TreeNode("Camera")) {
            ImGui::TextColored(ImVec4(1, 1, 1, 1), "Camera Settings");
            transForm(*context.camera);
            context.camera->updateView();
            ImGui::Separator();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Components")) {
            transForm(*context.trans);
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
        ImVec2 wsize = ImGui::GetWindowSize();
        ImVec2 pos = ImGui::GetWindowPos();
        ImVec2 bottomRight(pos.x + wsize.x, pos.y + wsize.y);
        // if game window not active, disable camera response
        context.camera->setActive(ImGui::IsWindowFocused() &&
                                  ImGui::IsMouseHoveringRect(pos, bottomRight));
        context.frameBuffer->update(wsize.x, wsize.y);
        context.camera->setSize(wsize.x, wsize.y);
        // Because I use the texture from OpenGL, I need to invert the V
        // from the UV.
        ImGui::Image((void*)(intptr_t)context.frameBuffer->getTextureId(),
                     wsize, ImVec2(0, 1), ImVec2(1, 0));
        std::string frameRate =
            "FPS:" + std::to_string(context.window->getFrameRate());
        ImGui::GetWindowDrawList()->AddText(pos, 0xFFFFFFFF, frameRate.c_str());
        ImGui::EndChild();
    }
    ImGui::End();

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
