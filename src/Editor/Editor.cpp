#include <Editor/Editor.hpp>
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <Utility/Math.hpp>
#include <Graphic/Primitive.hpp>
#include <iostream>

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

static void drawTransformSheet(Transform& trans) {
    ImGui::Separator();
    ImGui::Text("Transform");
    glm::vec3 eulerAngle = glm::degrees(trans.getEulerAngle());
    if (ImGui::InputFloat3("Rotation", &eulerAngle[0], "%.3f")) {
        trans.setEulerAngle(glm::radians(eulerAngle));
    }

    glm::vec3 pos = trans.getPosition();
    if (ImGui::InputFloat3("Position", &pos[0], "%.3f")) {
        trans.setPosition(pos);
    }
}

void Editor::buildModelAxes(float clipLen) {
    const auto& model =
        context.getActiveEntityPtr()->component<Transform>()->getTransform();
    glm::vec3 originWorld = model[3];
    m_modelScreenAxes.origin =
        context.getCamera()->computeWorldToSrceen(originWorld);
    // axes screen coordinate and color
    for (int i = 0; i < 3; ++i) {
        m_modelScreenAxes.axes[i].pos =
            context.getCamera()->computeWorldToSrceen(
                originWorld +
                glm::vec3(model[i]) * context.getClipSizeInWorld(clipLen));
        m_modelScreenAxes.axes[i].color[3] = 1.0f;
        m_modelScreenAxes.axes[i].color[i] = 1.0f;
        // ((uint8_t*)&m_modelScreenAxes.axes[i].color)[i] = 0xFF;
    }
}

void Editor::buildModelPlane() {
    const auto& model = context.getActiveEntityPtr()->component<Transform>();
    glm::vec3 pos = model->getPosition();
    m_planeXY = buildPlane(pos, pos + model->getFront());
    m_planeXZ = buildPlane(pos, pos + model->getUp());
    m_planeYZ = buildPlane(pos, pos + model->getRight());
}

void Editor::renderFps() {
    std::string frameRate =
        "FPS:" + std::to_string(context.getWindow()->getFrameRate());
    context.addText(glm::vec2(0), 0xFFFFFFFF, frameRate.c_str());
}

void Editor::renderModelAxes() {
    for (int i = 0; i < 3; ++i) {
        const auto& axis = m_modelScreenAxes.axes[i];
        Primitive::instance().drawLine(m_modelScreenAxes.origin, axis.pos,
                                       axis.color, 5.0f);
    }
    Primitive::instance().drawCircleFilled(m_modelScreenAxes.origin, 10.0f,
                                           glm::vec4(1.0f));
}

void Editor::renderCameraAxes(float clipLen) {
    clipLen *= context.getCamera()->getViewportHeight() / 2.f;
    auto model = context.getCamera()->component<Transform>();
    glm::vec3 origin(context.getCamera()->getViewportSize() - clipLen, 0);
    // draw x
    glm::vec3 xAxis = model->getRight();
    xAxis.y = -xAxis.y;
    xAxis = origin + xAxis * clipLen;
    context.addLine(origin, xAxis, 0xFF0000FF, 2);
    // draw y
    glm::vec3 yAxis = model->getUp();
    yAxis.y = -yAxis.y;
    yAxis = origin + yAxis * clipLen;
    context.addLine(origin, yAxis, 0xFF00FF00, 2);
    // draw z
    glm::vec3 zAxis = model->getFront();
    zAxis.y = -zAxis.y;
    zAxis = origin + zAxis * clipLen;
    context.addLine(origin, zAxis, 0xFFFF0000, 2);
}

void Editor::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Settings");
    {
        ImGui::SetWindowSize(ImVec2(300, 600));
        ImGui::SetWindowPos(ImVec2(0, 0));
        if (ImGui::TreeNodeEx("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::TextColored(ImVec4(1, 1, 1, 1), "Camera Settings");
            drawTransformSheet(
                *context.getCamera()->component<Transform>().get());
            ImGui::Separator();
            ImGui::TreePop();
        }

        if (ImGui::TreeNodeEx("Entity", ImGuiTreeNodeFlags_DefaultOpen)) {
            auto entity = context.getActiveEntityPtr();
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
        // prepare the context for ImGui drawing and framebuffer drawing
        context.prepareContext();

        ImVec2 wsize = ImGui::GetWindowSize();
        // if game window not active, disable camera response
        context.getCamera()->setActive(ImGui::IsWindowFocused() &&
                                       ImGui::IsWindowHovered());
        context.getFrameBuffer()->update(wsize.x, wsize.y);
        context.getCamera()->setSize(wsize.x, wsize.y);
        // Because I use the texture from OpenGL, I need to invert the V
        // from the UV.
        ImGui::Image(
            (void*)(intptr_t)context.getFrameBuffer()->getScreenTexture(),
            wsize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::EndChild();
    }
    ImGui::End();

    buildModelAxes(0.2);
    buildModelPlane();
    context.getCamera()->computeCameraRay(m_camRayOrigin, m_camRayDir,
                                          context.getCursorPos());
    float len = intersectRayPlane(m_camRayOrigin, m_camRayDir, m_planeXY);
    if (len > 0) {
        glm::vec3 intersectPoint = m_camRayOrigin + len * m_camRayDir;
        glm::vec2 intersectScreenPos =
            context.getCamera()->computeWorldToSrceen(intersectPoint);

        glm::vec2 cloesetScreenPoint = findClosestPoint(
            intersectScreenPos, glm::vec2(m_modelScreenAxes.origin),
            glm::vec2(m_modelScreenAxes.axes[0].pos));
        context.addCircleFilled(cloesetScreenPoint, 10, 0xFFFFFFFF);
        if (glm::length(intersectScreenPos - cloesetScreenPoint) < 12.f) {
            std::cout << "yes" << std::endl;
        }
    }

    renderFps();
    // clear depth buffer to make axes not hidden by object
    glClear(GL_DEPTH_BUFFER_BIT);
    renderModelAxes();
    renderCameraAxes(0.2);
    context.addLine(context.getCamera()->computeWorldToSrceen(m_camRayOrigin),
                    context.getCamera()->computeWorldToSrceen(
                        m_camRayOrigin + m_camRayDir * 100.f),
                    0xFF0000FF);

    // unload context and quit drawing drawing on the framebuffer
    context.unloadContext();

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
