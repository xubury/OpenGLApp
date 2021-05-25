#include <Editor/RenderContext.hpp>
#include <Graphic/Primitive.hpp>
#include <glad/glad.h>

RenderContext::RenderContext() : m_activeEntityId(2), m_screenFactor(1.0f) {}

void RenderContext::prepareContext() {
    ImVec2 renderOrigin = ImGui::GetWindowPos();
    m_renderOrigin.x = renderOrigin.x;
    m_renderOrigin.y = renderOrigin.y;
    m_drawList = ImGui::GetWindowDrawList();

    glm::vec3 pos = getActiveEntityPtr()->component<Transform>()->getPosition();
    float rightLen = m_camera->getSegmentLengthClipSpace(
        pos, pos + m_camera->component<Transform>()->getRight());
    m_screenFactor = 1.0f / rightLen;

    m_frameBuffer->activate();
    Primitive::instance().setDrawingView(m_camera);
}

void RenderContext::unloadContext() { m_frameBuffer->deactivate(); }

void RenderContext::setActiveEntityId(int id) { m_activeEntityId = id; }

EntityBase* RenderContext::getActiveEntityPtr() const {
    return m_entities->getPtr(m_activeEntityId);
}

void RenderContext::setCamrea(Camera* camera) { m_camera = camera; }

Camera* RenderContext::getCamera() const { return m_camera; }

void RenderContext::setWindow(RenderWindow* window) { m_window = window; }

RenderWindow* RenderContext::getWindow() const { return m_window; }

void RenderContext::setFrameBuffer(FrameBuffer* frameBuffer) {
    m_frameBuffer = frameBuffer;
}

FrameBuffer* RenderContext::getFrameBuffer() const { return m_frameBuffer; }

void RenderContext::setEntityManager(EntityManager<EntityBase>* entites) {
    m_entities = entites;
}

glm::vec2 RenderContext::getContextScreenPos() const {
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

void RenderContext::addLineEx(const glm::vec3& start, const glm::vec3& end,
                              const glm::vec4& color, float thickness) {
    Primitive::instance().drawLine(start, end, color, thickness);
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
