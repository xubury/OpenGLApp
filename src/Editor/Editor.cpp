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

RenderContext::RenderContext() : m_screenFactor(1.0f) {}

void RenderContext::prepareContext() {
    ImVec2 renderOrigin = ImGui::GetWindowPos();
    m_renderOrigin.x = renderOrigin.x;
    m_renderOrigin.y = renderOrigin.y;
    m_drawList = ImGui::GetWindowDrawList();
    float rightLen = camera->getSegmentLengthClipSpace(
        glm::vec3(0), camera->component<Transform>()->getRight());
    m_screenFactor = 1.0f / rightLen;
}

glm::vec2 RenderContext::getContextScreenPos() {
    ImGuiIO& io = ImGui::GetIO();
    return glm::vec2(io.MousePos.x, io.MousePos.y) - m_renderOrigin;
}

void RenderContext::addText(const glm::vec2& pos, uint32_t col,
                            const char* textBegin, const char* textEnd) {
    m_drawList->AddText(
        ImVec2(m_renderOrigin.x + pos.x, m_renderOrigin.y + pos.y), col,
        textBegin, textEnd);
}

void RenderContext::addLine(const glm::vec2& start, const glm::vec2& end,
                            uint32_t color, float thickness) {
    m_drawList->AddLine(
        ImVec2(m_renderOrigin.x + start.x, m_renderOrigin.y + start.y),
        ImVec2(m_renderOrigin.x + end.x, m_renderOrigin.y + end.y), color,
        thickness);
}

void RenderContext::addCircleFilled(const glm::vec2& center, float radius,
                                    uint32_t color, int numSegments) {
    m_drawList->AddCircleFilled(
        ImVec2(m_renderOrigin.x + center.x, m_renderOrigin.y + center.y),
        radius, color, numSegments);
}

void RenderContext::addRectFilled(const glm::vec2& tl, const glm::vec2& br,
                                  uint32_t color, float rounding,
                                  ImDrawCornerFlags roundingCorners) {
    m_drawList->AddRectFilled(
        ImVec2(m_renderOrigin.x + tl.x, m_renderOrigin.y + tl.y),
        ImVec2(m_renderOrigin.x + br.x, m_renderOrigin.y + br.y), color,
        rounding, roundingCorners);
}

float RenderContext::getClipSizeInWorld(float clipSize) const {
    return m_screenFactor * clipSize;
}

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
    auto entity = context.entities->getPtr(m_activeEntityId);
    const auto& model = entity->component<Transform>()->getTransform();
    glm::vec3 originWorld = model[3];
    m_modelScreenAxes.origin =
        context.camera->computeWorldToSrceen(originWorld);
    // axes screen coordinate and color
    for (int i = 0; i < 3; ++i) {
        m_modelScreenAxes.axes[i].pos = context.camera->computeWorldToSrceen(
            originWorld + glm::vec3(model[i]) * context.getClipSizeInWorld(clipLen));
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
    context.addText(glm::vec2(0), 0xFFFFFFFF, frameRate.c_str());
}

void Editor::renderModelAxes() {
    // backface culling
    if (std::abs(m_modelScreenAxes.origin.z) < 1.f) {
        for (int i = 0; i < 3; ++i) {
            const auto& axis = m_modelScreenAxes.axes[m_axesDrawingOrder[i]];
            if (std::abs(axis.pos.z) < 1.f) {
                context.addLine(m_modelScreenAxes.origin, axis.pos, axis.color);
            }
        }
        glm::vec3 hSize(5.0f, 5.0f, 0);
        context.addRectFilled(m_modelScreenAxes.origin - hSize,
                              m_modelScreenAxes.origin + hSize, 0xFFFFFFFF);
    }
}

void Editor::renderCameraAxes(float clipLen) {
    clipLen *= context.camera->getHeight() / 2.f;
    auto model = context.camera->component<Transform>();
    glm::vec3 origin(context.camera->getViewportSize() - clipLen, 0);
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
        context.prepareContext();

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

    buildModelAxes(0.2);
    buildModelPlane();
    context.camera->computeCameraRay(m_camRayOrigin, m_camRayDir,
                                     context.getContextScreenPos());
    float len = intersectRayPlane(m_camRayOrigin, m_camRayDir, m_planeXY);
    if (len > 0) {
        glm::vec3 intersectPoint = m_camRayOrigin + len * m_camRayDir;
        glm::vec2 intersectScreenPos =
            context.camera->computeWorldToSrceen(intersectPoint);

        glm::vec2 cloesetScreenPoint = findClosestPoint(
            intersectScreenPos, glm::vec2(m_modelScreenAxes.origin),
            glm::vec2(m_modelScreenAxes.axes[0].pos));
        context.addCircleFilled(cloesetScreenPoint, 10, 0xFFFFFFFF);
        if (glm::length(intersectScreenPos - cloesetScreenPoint) < 12.f) {
            std::cout << "yes" << std::endl;
        }
    }

    renderFps();
    renderModelAxes();
    renderCameraAxes(0.2);
    context.addLine(context.camera->computeWorldToSrceen(m_camRayOrigin),
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
