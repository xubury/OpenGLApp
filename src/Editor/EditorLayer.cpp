#include "Editor/EditorLayer.hpp"
#include "Core/Application.hpp"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Core/Math.hpp"
#include "Graphic/Primitive.hpp"
#include "Component/BoundingBox.hpp"
#include "Component/Light.hpp"
#include "Physics/Rigidbody.hpp"
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

static void renderRigidbodyProperty(Rigidbody& body) {
    ImGui::Separator();
    ImGui::Text("Rigidbody");
    bool kinematic = body.isKinematic();
    ImGui::Checkbox("Kinematic", &kinematic);
    body.setKinematic(kinematic);
    float mass = body.getMass();
    float restitution = body.getRestitution();
    float staticFriction = body.getStaticFriction();
    float dynamicFriction = body.getDynamicFriction();
    ImGui::InputFloat("Mass", &mass);
    ImGui::InputFloat("Restitution", &restitution);
    ImGui::InputFloat("Static friction", &staticFriction);
    ImGui::InputFloat("Dynamic friction", &dynamicFriction);
    glm::vec3 velocity = body.getVelocity();
    glm::vec3 angularVelocity = body.getAngularVelocity();
    ImGui::InputFloat3("Velocity", &velocity[0]);
    ImGui::InputFloat3("Angular velocity", &angularVelocity[0]);
}

static bool canActive(ImGuiMouseButton button) {
    return ImGui::IsMouseClicked(button) && !ImGui::IsAnyItemHovered() &&
           !ImGui::IsAnyItemActive();
}

EditorLayer::EditorLayer()
    : Layer("Editor"),
      m_width(800),
      m_height(600),
      m_leftMouseDown(false),
      m_rightMouseDown(false),
      m_moveType(MoveType::NONE),
      m_screenFactor(1.0) {
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

    FrameBufferSpecification spec;
    spec.width = m_width;
    spec.height = m_height;
    spec.attachmentsSpecs = {{FramebufferTextureFormat::RGB}};
    m_frameBuffer = createScope<FrameBuffer>(spec);

    spec.samples = 4;
    spec.attachmentsSpecs = {{FramebufferTextureFormat::RGB},
                             {FramebufferTextureFormat::DEPTH24STENCIL8}};
    m_multiSampleFramebuffer = createScope<FrameBuffer>(spec, true);

    m_camera = createRef<EditorCamera>(0, 0, 800, 600);
    m_camera->setPosition(glm::vec3(-8.f, 15.f, 21.f));
    m_camera->setEulerAngle(glm::vec3(glm::radians(-25.f), glm::radians(-28.f),
                                      glm::radians(1.5f)));
}

void EditorLayer::begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    m_multiSampleFramebuffer->bind();
}

void EditorLayer::end() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_multiSampleFramebuffer->getId());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frameBuffer->getId());
    glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);
    m_multiSampleFramebuffer->unbind();
    ImGui::Render();
    glClearColor(0.3, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorLayer::buildModelAxes(float clipLen) {
    m_axisSizeFactor = getClipSizeInWorld(clipLen);
    auto trans = getActiveEntityPtr()->component<Transform>();
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

void EditorLayer::renderFps() {
    std::string frameRate =
        "FPS:" +
        std::to_string(Application::instance().getWindow().getFrameRate());
    context.addText(glm::vec2(0), 0xFFFFFFFF, frameRate.c_str());
}

void EditorLayer::renderBoundingBox() {
    if (getActiveEntityPtr()->has<BoundingBox>()) {
        const auto bbox = getActiveEntityPtr()->component<BoundingBox>();
        Primitive::instance().drawCube(bbox->getWorldMin(), bbox->getWorldMax(),
                                       glm::vec4(0.f, 1.0f, 0.f, 1.0f));
    }
}

void EditorLayer::renderModelAxes() {
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

void EditorLayer::renderCameraAxes(float clipLen) {
    clipLen *= m_camera->getViewportHeight() / 2.f;
    glm::vec3 origin(glm::vec2(m_camera->getViewportSize()) - clipLen, 0);
    // draw x
    glm::vec3 xAxis = m_camera->getRight();
    xAxis.y = -xAxis.y;
    xAxis = origin + xAxis * clipLen;
    context.addLine(origin, xAxis, 0xFF0000FF, 2);
    // draw y
    glm::vec3 yAxis = m_camera->getUp();
    yAxis.y = -yAxis.y;
    yAxis = origin + yAxis * clipLen;
    context.addLine(origin, yAxis, 0xFF00FF00, 2);
    // draw z
    glm::vec3 zAxis = m_camera->getFront();
    zAxis.y = -zAxis.y;
    zAxis = origin + zAxis * clipLen;
    context.addLine(origin, zAxis, 0xFFFF0000, 2);
}

void EditorLayer::computeTranslateType() {
    const auto& trans = getActiveEntityPtr()->component<Transform>();
    const glm::mat3& rotation = trans->getMatrix();
    glm::vec3 modelWorldPos = trans->getPosition();
    m_moveType = MoveType::NONE;

    // if on the model axis center
    glm::vec4 translatePlane = buildPlane(modelWorldPos, m_camera->getFront());
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
        glm::vec3 start = m_camera->computeWorldToSrceen(m_modelAxes.origin);
        glm::vec3 end = m_camera->computeWorldToSrceen(m_modelAxes.axes[i]);
        glm::vec3 intersectScreenPos =
            m_camera->computeWorldToSrceen(intersectWorldPos);
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
                m_camera->computeWorldToSrceen(intersectWorldPos);

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

void EditorLayer::handleMouseLeftButton() {
    glm::vec2 mousePos(context.getCursorPos());
    if (m_leftMouseDown) {
        ImGui::CaptureMouseFromApp();
        auto trans = getActiveEntityPtr()->component<Transform>();
        if (m_moveType != NONE) {
            float len =
                intersectRayPlane(m_camRayOrigin, m_camRayDir, m_movePlane);
            glm::vec3 intersectWorldPos = m_camRayOrigin + len * m_camRayDir;
            glm::vec3 translation = intersectWorldPos - m_intersectWorldPos;
            if (m_moveType <= TRANSLATE_Z && m_moveType >= TRANSLATE_X) {
                const glm::vec3& axis =
                    trans->getMatrix()[m_moveType - TRANSLATE_X];
                translation = glm::dot(axis, translation) * axis;
                m_movePlane =
                    buildPlane(trans->getPosition(), m_camera->getFront());
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
            const glm::vec3& cameraUp = m_camera->getUp();
            const glm::vec3& cameraRight = m_camera->getRight();
            const glm::vec3& modelWorldPos =
                getActiveEntityPtr()->getPosition();
            transform = glm::translate(transform, modelWorldPos);
            transform =
                glm::rotate(transform, glm::radians(-offset.x), cameraUp);
            transform =
                glm::rotate(transform, glm::radians(-offset.y), cameraRight);
            transform = glm::translate(transform, -modelWorldPos);
            m_camera->transform(transform);
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

void EditorLayer::handleMouseRightButton() {
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

void EditorLayer::onImGuiRender() {
    ImGui::Begin("Settings");
    {
        ImGui::SetWindowSize(ImVec2(300, 600));
        ImGui::SetWindowPos(ImVec2(0, 0));
        if (ImGui::TreeNodeEx("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::TextColored(ImVec4(1, 1, 1, 1), "Camera Settings");
            renderTransformProperty(*m_camera);
            ImGui::Separator();
            ImGui::TreePop();
        }

        if (ImGui::TreeNodeEx("Entity", ImGuiTreeNodeFlags_DefaultOpen)) {
            auto entity = getActiveEntityPtr();
            ImGui::Text("Name: %s", entity->getName().c_str());
            renderTransformProperty(*entity->component<Transform>().get());
            if (entity->has<Light>()) {
                renderLightProperty(*entity->component<Light>().get());
            }
            if (entity->has<Rigidbody>()) {
                renderRigidbodyProperty(*entity->component<Rigidbody>().get());
            }
            ImGui::Separator();
            ImGui::TreePop();
        }
    }
    ImGui::End();

    ImGui::Begin("Scene");
    {
        ImGui::SetWindowSize(ImVec2(800, 600));
        ImGui::SetWindowPos(ImVec2(300, 0));

        ImGui::BeginChild("SceneRenderer");
        // prepare the context for ImGui drawing and framebuffer drawing
        context.prepareContext();
        glm::vec3 pos =
            getActiveEntityPtr()->component<Transform>()->getPosition();
        float rightLen = m_camera->getSegmentLengthClipSpace(
            pos, pos + m_camera->getRight());
        m_screenFactor = 1.0f / rightLen;

        ImVec2 wsize = ImGui::GetWindowSize();
        // if game window not active, disable camera response
        bool isFocus = ImGui::IsWindowFocused() && ImGui::IsWindowHovered();
        setBlockEvent(!isFocus);
        if (m_width != wsize.x || m_height != wsize.y) {
            m_frameBuffer->resize(wsize.x, wsize.y);
            m_multiSampleFramebuffer->resize(wsize.x, wsize.y);
            m_camera->setViewportSize(wsize.x, wsize.y);
            m_width = wsize.x;
            m_height = wsize.y;
        }
        // Because I use the texture from OpenGL, I need to invert the V
        // from the UV.
        ImGui::Image((void*)(intptr_t)m_frameBuffer->getColorAttachmentId(0),
                     wsize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::EndChild();
    }
    ImGui::End();

    ImGui::Begin("Entities");
    {
        ImGui::SetWindowSize(ImVec2(300, 600));
        ImGui::SetWindowPos(ImVec2(1100, 0));
        std::size_t size = m_scene->entities.size();
        char entityLabel[128];
        for (std::size_t i = 0; i < size; ++i) {
            sprintf(entityLabel, "ID:%lld Name:%s", i,
                    m_scene->entities.get(i)->getName().c_str());
            if (ImGui::Selectable(entityLabel, i == m_activeEntityId)) {
                m_activeEntityId = i;
                m_camera->setPosition(getActiveEntityPtr()->getPosition());
                m_camera->setEulerAngle(
                    glm::vec3(glm::radians(-45.f), glm::radians(45.f), 0));
                m_camera->translateLocal(glm::vec3(0, 0, 10));
            }
        }
    }
    ImGui::End();

    m_camera->computeCameraRay(m_camRayOrigin, m_camRayDir,
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
}

EditorLayer::~EditorLayer() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

float EditorLayer::getClipSizeInWorld(float clipSize) const {
    return m_screenFactor * clipSize;
}

EntityBase* EditorLayer::getActiveEntityPtr() {
    return m_scene->entities.get(m_activeEntityId);
}

void EditorLayer::onEventPoll(const Event& event) {
    m_camera->processEvent(event);
}

void EditorLayer::onEventProcess() { m_camera->processEvents(); }

}  // namespace te
