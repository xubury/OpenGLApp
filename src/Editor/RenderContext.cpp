#include <Editor/RenderContext.hpp>

RenderContext::RenderContext() : m_activeEntityId(2), m_screenFactor(1.0f) {}

void RenderContext::prepareContext() {
    ImVec2 renderOrigin = ImGui::GetWindowPos();
    m_renderOrigin.x = renderOrigin.x;
    m_renderOrigin.y = renderOrigin.y;
    m_drawList = ImGui::GetWindowDrawList();

    glm::vec3 pos = getActiveEntityPtr()->component<Transform>()->getPosition();
    float rightLen = camera->getSegmentLengthClipSpace(
        pos, pos + camera->component<Transform>()->getRight());
    m_screenFactor = 1.0f / rightLen;
}

EntityBase* RenderContext::getActiveEntityPtr() {
    return entities->getPtr(m_activeEntityId);
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
