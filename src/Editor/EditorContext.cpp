#include <Graphic/Primitive.hpp>
#include <Editor/EditorContext.hpp>

namespace te {

EditorContext::EditorContext() : m_activeEntityId(0) {}

void EditorContext::prepareContext() {
    ImVec2 renderOrigin = ImGui::GetWindowPos();
    m_renderOrigin.x = renderOrigin.x;
    m_renderOrigin.y = renderOrigin.y;
    m_drawList = ImGui::GetWindowDrawList();
}

void EditorContext::setActiveEntityId(int id) { m_activeEntityId = id; }

uint32_t EditorContext::getActiveEntityId() const { return m_activeEntityId; }

EntityBase* EditorContext::getActiveEntityPtr() {
    return m_entities->get(m_activeEntityId);
}

void EditorContext::setWindow(RenderWindow* window) { m_window = window; }

RenderWindow* EditorContext::getWindow() { return m_window; }

EntityManager<EntityBase>* EditorContext::getEntityManager() {
    return m_entities;
}

void EditorContext::setEntityManager(EntityManager<EntityBase>* entites) {
    m_entities = entites;
}

glm::vec2 EditorContext::getCursorPos() const {
    ImGuiIO& io = ImGui::GetIO();
    return glm::vec2(io.MousePos.x, io.MousePos.y) - m_renderOrigin;
}

void EditorContext::addText(const glm::vec2& pos, uint32_t col,
                            const char* textBegin, const char* textEnd) {
    m_drawList->AddText(
        ImVec2(m_renderOrigin.x + pos.x, m_renderOrigin.y + pos.y), col,
        textBegin, textEnd);
}

void EditorContext::addLine(const glm::vec2& start, const glm::vec2& end,
                            uint32_t color, float thickness) {
    m_drawList->AddLine(
        ImVec2(m_renderOrigin.x + start.x, m_renderOrigin.y + start.y),
        ImVec2(m_renderOrigin.x + end.x, m_renderOrigin.y + end.y), color,
        thickness);
}

void EditorContext::addCircleFilled(const glm::vec2& center, float radius,
                                    uint32_t color, int numSegments) {
    m_drawList->AddCircleFilled(
        ImVec2(m_renderOrigin.x + center.x, m_renderOrigin.y + center.y),
        radius, color, numSegments);
}

void EditorContext::addRectFilled(const glm::vec2& tl, const glm::vec2& br,
                                  uint32_t color, float rounding,
                                  ImDrawCornerFlags roundingCorners) {
    m_drawList->AddRectFilled(
        ImVec2(m_renderOrigin.x + tl.x, m_renderOrigin.y + tl.y),
        ImVec2(m_renderOrigin.x + br.x, m_renderOrigin.y + br.y), color,
        rounding, roundingCorners);
}

}  // namespace te
