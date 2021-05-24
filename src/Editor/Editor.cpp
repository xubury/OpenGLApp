#include <Editor/Editor.hpp>
#include <Graphic/FrameBuffer.hpp>
#include <Entity/Camera.hpp>
#include <Graphic/RenderWindow.hpp>
#include <Component/Transform.hpp>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <Utility/Math.hpp>
#include <iostream>

Editor& Editor::instance() {
    static Editor s_instance;
    return s_instance;
}

Editor::Editor() : m_activeEntityId(1), m_axesDrawingOrder{0, 1, 2} {
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
    if (ImGui::InputFloat3("Rotation", &eulerAngle[0], "%.3f")) {
        trans.setEulerAngle(eulerAngle);
    }

    glm::vec3 pos = trans.getPosition();
    if (ImGui::InputFloat3("Position", &pos[0], "%.3f")) {
        trans.setPosition(pos);
    }
}

void Editor::buildModelAxes(float len) {
    auto entity = context.entities->getPtr(m_activeEntityId);
    const auto& model = entity->component<Transform>()->getTransform();
    glm::vec3 originWorld = model[3];
    glm::vec3 renderOrigin = glm::vec3(m_renderOrigin, 0);
    m_modelScreenAxes.origin =
        context.camera->computeWorldToSrceen(originWorld) + renderOrigin;
    // axes screen coordinate and color
    for (int i = 0; i < 3; ++i) {
        m_modelScreenAxes.axes[i].pos = context.camera->computeWorldToSrceen(
                                            originWorld + glm::vec3(model[i])) +
                                        renderOrigin;
        m_modelScreenAxes.axes[i].pos =
            m_modelScreenAxes.origin +
            glm::normalize(m_modelScreenAxes.axes[i].pos -
                           m_modelScreenAxes.origin) *
                len;
        m_modelScreenAxes.axes[i].color = 0xFF000000;
        ((uint8_t*)&m_modelScreenAxes.axes[i].color)[i] = 0xFF;
    }
    // sort the drawing order, so the axis with heighest Z value draw first
    std::sort(std::begin(m_axesDrawingOrder), std::end(m_axesDrawingOrder),
              [&axes = m_modelScreenAxes](const auto& lhs, const auto& rhs) {
                  return axes.axes[lhs].pos.z > axes.axes[rhs].pos.z;
              });
}

void Editor::buildModelPlane() {
    auto entity = context.entities->getPtr(m_activeEntityId);
    const auto& model = entity->component<Transform>();
    glm::vec3 pos = model->getPosition();
    m_planeXY = buildPlane(pos, pos + model->getFront());
    m_planeXZ = buildPlane(pos, pos + model->getUp());
    m_planeYZ = buildPlane(pos, pos + model->getRight());
}

void Editor::renderFps() {
    std::string frameRate =
        "FPS:" + std::to_string(context.window->getFrameRate());
    m_drawList->AddText(ImVec2(m_renderOrigin.x, m_renderOrigin.y), 0xFFFFFFFF,
                        frameRate.c_str());
}

void Editor::renderAxis(const glm::vec2& origin, const glm::vec2& axis,
                        ImU32 color, float thickness) {
    m_drawList->AddLine(ImVec2(origin.x, origin.y), ImVec2(axis.x, axis.y),
                        color, thickness);
}

void Editor::renderModelAxes() {
    // backface culling
    if (m_modelScreenAxes.origin.z > 0.f) {
        for (int i = 0; i < 3; ++i) {
            const auto& axis = m_modelScreenAxes.axes[m_axesDrawingOrder[i]];
            if (axis.pos.z > 0.f) {
                renderAxis(m_modelScreenAxes.origin, axis.pos, axis.color);
            }
        }
        float hWidth = 5.0f;
        float hHeight = 5.0f;
        m_drawList->AddRectFilled(ImVec2(m_modelScreenAxes.origin.x - hWidth,
                                         m_modelScreenAxes.origin.y - hHeight),
                                  ImVec2(m_modelScreenAxes.origin.x + hWidth,
                                         m_modelScreenAxes.origin.y + hHeight),
                                  0xFFFFFFFF);
    }
}

void Editor::renderCameraAxes(float len) {
    auto model = context.camera->component<Transform>();
    glm::vec3 origin(m_renderOrigin + context.camera->getViewportSize() - len,
                     0);
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

    ImGui::Begin("Settings");
    {
        ImGui::SetWindowSize(ImVec2(300, 600));
        ImGui::SetWindowPos(ImVec2(0, 0));
        if (ImGui::TreeNodeEx("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::TextColored(ImVec4(1, 1, 1, 1), "Camera Settings");
            drawTransformSheet(*context.camera->component<Transform>().get());
            ImGui::Separator();
            ImGui::TreePop();
        }

        if (ImGui::TreeNodeEx("Entity", ImGuiTreeNodeFlags_DefaultOpen)) {
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
        ImVec2 renderOrigin = ImGui::GetWindowPos();
        m_renderOrigin.x = renderOrigin.x;
        m_renderOrigin.y = renderOrigin.y;
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
    ImGuiIO& io = ImGui::GetIO();
    buildModelAxes(50.0f);
    buildModelPlane();
    context.camera->computeCameraRay(m_camRayOrigin, m_camRayDir,
                     glm::vec2(io.MousePos.x, io.MousePos.y));
    float len = intersectRayPlane(m_camRayOrigin, m_camRayDir, m_planeXY);
    if (len > 0) {
        glm::vec3 intersectPoint = m_camRayOrigin + len * m_camRayDir;
        glm::vec2 intersectScreenPos =
            context.camera->computeWorldToSrceen(intersectPoint);

        glm::vec2 cloesetScreenPoint = findClosestPoint(
            intersectScreenPos, glm::vec2(m_modelScreenAxes.origin),
            glm::vec2(m_modelScreenAxes.axes[0].pos));
        m_drawList->AddCircleFilled(
            ImVec2(cloesetScreenPoint.x, cloesetScreenPoint.y), 10, 0xFFFFFFFF);
        if (glm::length(intersectScreenPos - cloesetScreenPoint) < 12.f) {
            std::cout << "yes" << std::endl;
        } else {
            std::cout << "no" << std::endl;
        }
    }

    renderFps();
    renderModelAxes();
    renderCameraAxes(50.0f);
    renderAxis(context.camera->computeWorldToSrceen(m_camRayOrigin),
               context.camera->computeWorldToSrceen(m_camRayOrigin +
                                                    m_camRayDir * 100.f),
               0xFF0000FF);

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
