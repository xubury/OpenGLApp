#include <Editor/Editor.hpp>
#include <Graphic/FrameBuffer.hpp>
#include <Graphic/Camera.hpp>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void Editor::intialize() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void Editor::render(EditorContext& context) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("GameWindow");
    {
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
        ImGui::GetWindowDrawList()->AddCircleFilled(
            ImVec2(10 + pos.x, 10 + pos.y), 10, 0xFF0000FF);
        ImGui::EndChild();
    }
    ImGui::End();

    ImGui::Begin("Settings");
    {
        ImGui::BeginChild("Camera");
        ImGui::TextColored(ImVec4(1, 1, 1, 1), "Camera Settings");
        ImGui::EndChild();
    }
    ImGui::End();

    ImGui::Render();
    glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::close() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
