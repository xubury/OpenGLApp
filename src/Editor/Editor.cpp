#include <Editor/Editor.hpp>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <Core/Math.hpp>
#include <Graphic/Primitive.hpp>
#include <Component/BoundingBox.hpp>
#include <Component/Light.hpp>
#include <iostream>

namespace te {

static const float quadMin = 0.4f;
static const float quadMax = 0.8f;
static const float quadUV[8] = {quadMin, quadMin, quadMin, quadMax,
                                quadMax, quadMax, quadMax, quadMin};
static const float axisOriginRadius = 0.1f;

static const float axisRotateCircleRadius = 1.5;

static const float lineThickness = 5.f;

static const glm::vec4 selectionColor(1.f, 0.5f, 0.f, 0.7f);

static const float axisTransparency = 0.8f;

static void renderTransformProperty(Transform& trans) {
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

static void renderLightProperty(Light& light) {
    ImGui::Separator();
    ImGui::Text("Light");
    ImGui::InputFloat3("Ambient", &light.amibent[0], "%.3f");
    ImGui::InputFloat3("Diffuse", &light.diffuse[0], "%.3f");
    ImGui::InputFloat3("Specular", &light.specular[0], "%.3f");
}

static bool canActive(ImGuiMouseButton button) {
    return ImGui::IsMouseClicked(button) && !ImGui::IsAnyItemHovered() &&
           !ImGui::IsAnyItemActive();
}

Editor& Editor::instance() {
    static Editor s_instance;
    return s_instance;
}

Editor::Editor()
    : m_leftMouseDown(false),
      m_rightMouseDown(false),
      m_moveType(MoveType::NONE) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(RenderWindow::getCurrentContext(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void Editor::buildModelAxes(float clipLen) {
    m_axisSizeFactor = context.getClipSizeInWorld(clipLen);
    auto trans = context.getActiveEntityPtr()->component<Transform>();
    const glm::mat3& rMatrix = trans->getMatrix();
    glm::vec3 originWorld = trans->getPosition();
    m_modelAxes.origin = originWorld;
    // axes screen coordinate and color
    for (int i = 0; i < 3; ++i) {
        m_modelAxes.axes[i] =
            originWorld + glm::vec3(rMatrix[i]) * m_axisSizeFactor;
        for (int j = 0; j < 4; ++j) {
            glm::vec3 cornerWorldPos =
                originWorld + (rMatrix[(i + 1) % 3] * quadUV[j * 2] +
                               rMatrix[(i + 2) % 3] * quadUV[j * 2 + 1]) *
                                  m_axisSizeFactor;
            m_modelQuads[i][j] = cornerWorldPos;
        }
    }
}

void Editor::renderFps() {
    std::string frameRate =
        "FPS:" + std::to_string(context.getWindow()->getFrameRate());
    context.addText(glm::vec2(0), 0xFFFFFFFF, frameRate.c_str());
}

void Editor::renderBoundingBox() {
    if (context.getActiveEntityPtr()->has<BoundingBox>()) {
        const auto bbox =
            context.getActiveEntityPtr()->component<BoundingBox>();
        Primitive::instance().drawCube(bbox->getWorldMin(), bbox->getWorldMax(),
                                       glm::vec4(0.f, 1.0f, 0.f, 1.0f));
    }
}

void Editor::renderModelAxes() {
    for (int i = 0; i < 3; ++i) {
        const auto& axis = m_modelAxes.axes[i];
        glm::vec4 axisColor(0.f);
        axisColor[i] = 1.0f;
        axisColor[3] = axisTransparency;
        if (m_moveType == TRANSLATE_X + i) {
            axisColor = selectionColor;
        }
        Primitive::instance().drawLine(m_modelAxes.origin, axis, axisColor,
                                       lineThickness);

        std::vector<glm::vec3> vertices(4);
        glm::vec4 panelColor(0.f);
        panelColor[i] = 1.0f;
        panelColor[3] = axisTransparency;
        if (m_moveType == TRANSLATE_YZ + i) {
            panelColor = selectionColor;
        }

        Primitive::instance().drawQuadFilled(
            std::vector<glm::vec3>(m_modelQuads[i], m_modelQuads[i] + 4),
            panelColor);

        glm::vec4 circleColor(0);
        circleColor[i] = 1.0f;
        circleColor[3] = axisTransparency;
        if (m_moveType == ROTATE_X + i) {
            circleColor = selectionColor;
        }
        Primitive::instance().drawCircle(
            m_modelAxes.origin, axis - m_modelAxes.origin, circleColor,
            m_axisSizeFactor * axisRotateCircleRadius);
    }
    float worldRadius = m_axisSizeFactor * axisOriginRadius;
    if (m_moveType == TRANSLATE_XYZ) {
        Primitive::instance().drawSphere(m_modelAxes.origin, selectionColor,
                                         worldRadius);
    } else {
        Primitive::instance().drawSphere(m_modelAxes.origin, glm::vec4(1.0f),
                                         worldRadius);
    }
}

void Editor::renderCameraAxes(float clipLen) {
    Ref<Camera> cam = context.getCamera();
    Transform::Handle trans = context.getCamera()->component<Transform>();
    clipLen *= cam->getViewportHeight() / 2.f;
    glm::vec3 origin(cam->getViewportSize() - clipLen, 0);
    // draw x
    glm::vec3 xAxis = trans->getRight();
    xAxis.y = -xAxis.y;
    xAxis = origin + xAxis * clipLen;
    context.addLine(origin, xAxis, 0xFF0000FF, 2);
    // draw y
    glm::vec3 yAxis = trans->getUp();
    yAxis.y = -yAxis.y;
    yAxis = origin + yAxis * clipLen;
    context.addLine(origin, yAxis, 0xFF00FF00, 2);
    // draw z
    glm::vec3 zAxis = trans->getFront();
    zAxis.y = -zAxis.y;
    zAxis = origin + zAxis * clipLen;
    context.addLine(origin, zAxis, 0xFFFF0000, 2);
}

void Editor::computeTranslateType() {
    const auto& trans = context.getActiveEntityPtr()->component<Transform>();
    const glm::mat3& rotation = trans->getMatrix();
    glm::vec3 modelWorldPos = trans->getPosition();
    m_moveType = MoveType::NONE;

    // if on the model axis center
    glm::vec4 translatePlane = buildPlane(
        modelWorldPos, context.getCamera()->component<Transform>()->getFront());
    float len = intersectRayPlane(m_camRayOrigin, m_camRayDir, translatePlane);
    glm::vec3 intersectWorldPos = m_camRayOrigin + len * m_camRayDir;
    if (glm::length(intersectWorldPos - modelWorldPos) <
        axisOriginRadius * m_axisSizeFactor) {
        m_moveType = MoveType::TRANSLATE_XYZ;
        m_movePlane = translatePlane;
        m_intersectWorldPos = modelWorldPos;
        return;
    }
    float minZ = std::numeric_limits<float>::max();
    bool onAxis = false;
    for (uint32_t i = 0; i < 3; ++i) {
        glm::vec3 start =
            context.getCamera()->computeWorldToSrceen(m_modelAxes.origin);
        glm::vec3 end =
            context.getCamera()->computeWorldToSrceen(m_modelAxes.axes[i]);
        glm::vec3 intersectScreenPos =
            context.getCamera()->computeWorldToSrceen(intersectWorldPos);
        glm::vec3 cloesetScreenPos =
            findClosestPoint(intersectScreenPos, start, end);
        if (glm::length(intersectScreenPos - cloesetScreenPos) <
            lineThickness) {
            if (cloesetScreenPos.z < minZ) {
                minZ = cloesetScreenPos.z;
                m_moveType = static_cast<MoveType>(TRANSLATE_X + i);
                m_movePlane = translatePlane;
                m_intersectWorldPos = intersectWorldPos;
                onAxis = true;
            }
        }
    }
    if (onAxis) return;

    minZ = std::numeric_limits<float>::max();
    for (uint32_t i = 0; i < 3; ++i) {
        translatePlane = buildPlane(modelWorldPos, rotation[i]);
        float len =
            intersectRayPlane(m_camRayOrigin, m_camRayDir, translatePlane);
        if (len > 0) {
            intersectWorldPos = m_camRayOrigin + len * m_camRayDir;
            if (std::abs(glm::length(intersectWorldPos - modelWorldPos) -
                         axisRotateCircleRadius * m_axisSizeFactor) <
                0.1f * m_axisSizeFactor) {
                m_rotationVector =
                    glm::normalize(m_intersectWorldPos - m_modelAxes.origin);
                m_movePlane = translatePlane;
                m_intersectWorldPos = intersectWorldPos;
                m_moveType = static_cast<MoveType>(ROTATE_X + i);
                break;
            }
            glm::vec3 intersectScreenPos =
                context.getCamera()->computeWorldToSrceen(intersectWorldPos);

            float projectionUV[2];
            for (int j = 1; j <= 2; ++j) {
                int axisId = (i + j) % 3;
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
                    m_intersectWorldPos = intersectWorldPos;
                    m_moveType = static_cast<MoveType>(TRANSLATE_YZ + i);
                }
            }
        }
    }
}

void Editor::handleMouseLeftButton() {
    glm::vec2 mousePos(context.getCursorPos());
    if (m_leftMouseDown) {
        ImGui::CaptureMouseFromApp();
        auto trans = context.getActiveEntityPtr()->component<Transform>();
        if (m_moveType != NONE) {
            float len =
                intersectRayPlane(m_camRayOrigin, m_camRayDir, m_movePlane);
            glm::vec3 intersectWorldPos = m_camRayOrigin + len * m_camRayDir;
            glm::vec3 translation = intersectWorldPos - m_intersectWorldPos;
            if (m_moveType <= TRANSLATE_Z && m_moveType >= TRANSLATE_X) {
                const glm::vec3& axis =
                    trans->getMatrix()[m_moveType - TRANSLATE_X];
                translation = glm::dot(axis, translation) * axis;
                m_movePlane = buildPlane(
                    trans->getPosition(),
                    context.getCamera()->component<Transform>()->getFront());
            }
            if (m_moveType <= TRANSLATE_XYZ) {
                trans->translateWorld(translation);
            } else {
                // rotate
                glm::vec3 localPos = intersectWorldPos - m_modelAxes.origin;
                localPos = glm::normalize(localPos);
                glm::vec3 perpendicularVec =
                    glm::cross(m_rotationVector, glm::vec3(m_movePlane));
                perpendicularVec = glm::normalize(perpendicularVec);
                float acosAngle =
                    std::clamp(glm::dot(localPos, m_rotationVector), -1.f, 1.f);
                float angle = std::acos(acosAngle);
                angle *=
                    (glm::dot(localPos, perpendicularVec) < 0.f) ? 1.f : -1.f;

                glm::vec3 localNormal =
                    glm::inverse(trans->getMatrix()) *
                    glm::vec4(m_movePlane.x, m_movePlane.y, m_movePlane.z, 0);
                localNormal = glm::normalize(localNormal);
                trans->rotateLocal(angle, localNormal);
                m_rotationVector = localPos;
            }
            m_intersectWorldPos = intersectWorldPos;
        } else {
            glm::vec2 offset = (mousePos - m_mouseClickPos) * 0.1f;
            glm::mat4 transform(1.0f);
            const glm::vec3& cameraUp =
                context.getCamera()->component<Transform>()->getUp();
            const glm::vec3& cameraRight =
                context.getCamera()->component<Transform>()->getRight();
            const glm::vec3& modelWorldPos =
                context.getActiveEntityPtr()->getPosition();
            transform = glm::translate(transform, modelWorldPos);
            transform =
                glm::rotate(transform, glm::radians(-offset.x), cameraUp);
            transform =
                glm::rotate(transform, glm::radians(-offset.y), cameraRight);
            transform = glm::translate(transform, -modelWorldPos);
            context.getCamera()->component<Transform>()->transform(transform);
        }

        if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            m_leftMouseDown = false;
        }
    } else {
        if (canActive(ImGuiMouseButton_Left)) {
            m_leftMouseDown = true;
        }
        computeTranslateType();
    }
    m_mouseClickPos = mousePos;
}

void Editor::handleMouseRightButton() {
    glm::vec2 mousePos(context.getCursorPos());
    if (m_rightMouseDown) {
        ImGui::CaptureMouseFromApp();
        if (!ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
            m_rightMouseDown = false;
        }
    } else {
        if (canActive(ImGuiMouseButton_Right)) {
            m_rightMouseDown = true;
        }
    }
    m_mouseClickPos = mousePos;
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
            renderTransformProperty(
                *context.getCamera()->component<Transform>().get());
            ImGui::Separator();
            ImGui::TreePop();
        }

        if (ImGui::TreeNodeEx("Entity", ImGuiTreeNodeFlags_DefaultOpen)) {
            auto entity = context.getActiveEntityPtr();
            ImGui::Text("Name: %s", entity->getName().c_str());
            renderTransformProperty(*entity->component<Transform>().get());
            if (entity->has<Light>()) {
                renderLightProperty(*entity->component<Light>().get());
            }
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
        context.getCamera()->setViewportSize(wsize.x, wsize.y);
        // Because I use the texture from OpenGL, I need to invert the V
        // from the UV.
        ImGui::Image(
            (void*)(intptr_t)context.getFrameBuffer()->getScreenTexture(),
            wsize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::EndChild();
    }
    ImGui::End();

    ImGui::Begin("Scene");
    {
        ImGui::SetWindowSize(ImVec2(300, 600));
        ImGui::SetWindowPos(ImVec2(1100, 0));
        ImGui::BeginChild("SceneList");
        std::size_t size = context.getEntityManager()->size();
        char entityLabel[128];
        for (std::size_t i = 0; i < size; ++i) {
            sprintf(entityLabel, "ID:%lld Name:%s", i,
                    context.getEntityManager()->get(i)->getName().c_str());
            if (ImGui::Selectable(entityLabel,
                                  i == context.getActiveEntityId())) {
                const float dist =
                    glm::length(context.getActiveEntityPtr()->getPosition() -
                                context.getCamera()->getPosition());

                context.setActiveEntityId(i);
                glm::vec3 worldPos =
                    context.getActiveEntityPtr()->getPosition() +
                    glm::normalize(context.getCamera()
                                       ->component<Transform>()
                                       ->getFront()) *
                        dist;
                context.getCamera()->setPosition(worldPos);
            }
        }
        ImGui::EndChild();
    }
    ImGui::End();

    context.getCamera()->computeCameraRay(m_camRayOrigin, m_camRayDir,
                                          context.getCursorPos());
    buildModelAxes(0.2);

    // clear depth buffer to make axes not hidden by object
    glClear(GL_DEPTH_BUFFER_BIT);
    handleMouseLeftButton();

    handleMouseRightButton();

    renderFps();

    renderBoundingBox();

    renderModelAxes();

    renderCameraAxes(0.2);

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

}  // namespace te
