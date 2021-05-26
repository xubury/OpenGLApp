#include <Editor/Editor.hpp>
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <Utility/Math.hpp>
#include <Graphic/Primitive.hpp>
#include <iostream>

static const float quadMin = 0.2f;
static const float quadMax = 0.8f;
static const float quadUV[8] = {quadMin, quadMin, quadMin, quadMax,
                                quadMax, quadMax, quadMax, quadMin};

static const float lineThickness = 5.f;
static const float axisOriginRadius = 10.f;

static const glm::vec4 selectionColor(1.f, 0.5f, 0.f, 0.5f);

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

static bool canActive() {
    return ImGui::IsMouseClicked(0) && !ImGui::IsAnyItemHovered() &&
           !ImGui::IsAnyItemActive();
}

Editor& Editor::instance() {
    static Editor s_instance;
    return s_instance;
}

Editor::Editor() : m_leftMouseDown(false), m_moveType(MoveType::NONE) {
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

void Editor::buildModelAxes(float clipLen) {
    m_axisSizeFactor = context.getClipSizeInWorld(clipLen);
    auto trans = context.getActiveEntityPtr()->component<Transform>();
    const glm::mat3& rMatrix = trans->getMatrix();
    glm::vec3 originWorld = trans->getPosition();
    m_modelScreenAxes.origin =
        context.getCamera()->computeWorldToSrceen(originWorld);
    // axes screen coordinate and color
    for (int i = 0; i < 3; ++i) {
        m_modelScreenAxes.axes[i] = context.getCamera()->computeWorldToSrceen(
            originWorld + glm::vec3(rMatrix[i]) * m_axisSizeFactor);
        for (int j = 0; j < 4; ++j) {
            glm::vec3 cornerWorldPos =
                originWorld + (rMatrix[(i + 1) % 3] * quadUV[j * 2] +
                               rMatrix[(i + 2) % 3] * quadUV[j * 2 + 1]) *
                                  m_axisSizeFactor;
            m_modelScreenQuads[i][j] =
                context.getCamera()->computeWorldToSrceen(cornerWorldPos);
        }
    }
}

void Editor::renderFps() {
    std::string frameRate =
        "FPS:" + std::to_string(context.getWindow()->getFrameRate());
    context.addText(glm::vec2(0), 0xFFFFFFFF, frameRate.c_str());
}

void Editor::renderModelAxes() {
    for (int i = 0; i < 3; ++i) {
        const auto& axis = m_modelScreenAxes.axes[i];
        glm::vec4 axisColor(0.f);
        axisColor[i] = 1.0f;
        axisColor[3] = 1.0f;
        if (m_moveType == TRANSLATE_X + i) {
            axisColor = selectionColor;
        }
        Primitive::instance().drawLine(
            DebugVertex(m_modelScreenAxes.origin, axisColor),
            DebugVertex(axis, axisColor), lineThickness);

        std::vector<DebugVertex> vertices(4);
        glm::vec4 panelColor(0.f);
        panelColor[i] = 1.0f;
        panelColor[3] = 1.0f;
        if (m_moveType == TRANSLATE_YZ + i) {
            panelColor = selectionColor;
        }
        for (int j = 0; j < 4; ++j) {
            vertices[j].position = m_modelScreenQuads[i][j];
            vertices[j].color = panelColor;
        }
        Primitive::instance().drawPath(vertices, 2.0f);
    }
    if (m_moveType == TRANSLATE_XYZ) {
        Primitive::instance().drawCircleFilled(
            DebugVertex(m_modelScreenAxes.origin, selectionColor),
            axisOriginRadius);
    } else {
        Primitive::instance().drawCircleFilled(
            DebugVertex(m_modelScreenAxes.origin, glm::vec4(1.0f)),
            axisOriginRadius);
    }
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

void Editor::computeMoveType() {
    const auto& trans = context.getActiveEntityPtr()->component<Transform>();
    const glm::mat3& rotation = trans->getMatrix();
    glm::vec3 modelWorldPos = trans->getPosition();
    m_moveType = MoveType::NONE;

    // if on the model axis center
    if (glm::length(context.getCursorPos() -
                    glm::vec2(context.getCamera()->computeWorldToSrceen(
                        modelWorldPos))) < axisOriginRadius) {
        m_moveType = MoveType::TRANSLATE_XYZ;
        m_movePlane =
            buildPlane(modelWorldPos,
                       context.getCamera()->component<Transform>()->getFront());
        m_originalPos = modelWorldPos;
        return;
    }
    float minZ = std::numeric_limits<float>::max();
    for (uint32_t i = 0; i < 3; ++i) {
        glm::vec4 translatePlane = buildPlane(modelWorldPos, rotation[i]);
        float len =
            intersectRayPlane(m_camRayOrigin, m_camRayDir, translatePlane);
        if (len > 0) {
            glm::vec3 intersectWorldPos = m_camRayOrigin + len * m_camRayDir;
            glm::vec3 intersectScreenPos =
                context.getCamera()->computeWorldToSrceen(intersectWorldPos);

            float projectionUV[2];
            for (int j = 1; j <= 2; ++j) {
                int axisId = (i + j) % 3;
                glm::vec2 axis = m_modelScreenAxes.axes[axisId];
                glm::vec2 cloesetScreenPoint =
                    findClosestPoint(glm::vec2(intersectScreenPos),
                                     glm::vec2(m_modelScreenAxes.origin), axis);
                // check if on axis
                if (glm::length(glm::vec2(intersectScreenPos) -
                                cloesetScreenPoint) < lineThickness) {
                    m_moveType = static_cast<MoveType>(TRANSLATE_X + axisId);
                    m_movePlane = translatePlane;
                    m_originalPos = intersectWorldPos;
                    break;
                }
                projectionUV[j - 1] = glm::dot(
                    rotation[axisId],
                    (intersectWorldPos - modelWorldPos) / m_axisSizeFactor);
            }
            // check projection uv inside quad
            if (projectionUV[0] >= quadUV[0] && projectionUV[0] <= quadUV[4] &&
                projectionUV[1] >= quadUV[1] && projectionUV[1] <= quadUV[5]) {
                // takes the nearest screen quad
                if (intersectScreenPos.z < minZ) {
                    minZ = intersectScreenPos.z;
                    m_movePlane = translatePlane;
                    m_originalPos = intersectWorldPos;
                    m_moveType = static_cast<MoveType>(TRANSLATE_YZ + i);
                }
            }
        }
    }
}

void Editor::handleTranslation() {
    if (m_leftMouseDown) {
        auto trans = context.getActiveEntityPtr()->component<Transform>();
        if (m_moveType != NONE) {
            float len =
                intersectRayPlane(m_camRayOrigin, m_camRayDir, m_movePlane);
            glm::vec3 intersectPoint = m_camRayOrigin + len * m_camRayDir;
            glm::vec3 translation = intersectPoint - m_originalPos;
            if (m_moveType <= TRANSLATE_Z && m_moveType >= TRANSLATE_X) {
                const glm::vec3& axis =
                    trans->getMatrix()[m_moveType - TRANSLATE_X];
                translation = glm::dot(axis, translation) * axis;
            }
            trans->translateWorld(translation);
            m_originalPos = intersectPoint;
        }

        if (!ImGui::IsMouseDown(0)) {
            m_leftMouseDown = false;
            m_moveType = NONE;
        }
    } else {
        if (canActive()) {
            m_leftMouseDown = true;
        }
        computeMoveType();
    }
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

    context.getCamera()->computeCameraRay(m_camRayOrigin, m_camRayDir,
                                          context.getCursorPos());
    buildModelAxes(0.2);
    handleTranslation();

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
