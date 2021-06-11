#ifndef EDITOR_CONTEXT_HPP
#define EDITOR_CONTEXT_HPP

#include <imgui.h>
#include <glm/glm.hpp>
#include "Graphic/FrameBuffer.hpp"
#include "Window/RenderWindow.hpp"

namespace te {

class EditorContext {
   public:
    EditorContext();

    void prepareContext();

    // get window pos relateive to current context window
    glm::vec2 getCursorPos() const;

    void addText(const glm::vec2 &pos, uint32_t col, const char *textBegin,
                 const char *textEnd = nullptr);

    void addLine(const glm::vec2 &start, const glm::vec2 &end, uint32_t color,
                 float thickness = 5.f);

    void addCircleFilled(const glm::vec2 &center, float radius, uint32_t color,
                         int numSegments = 0);

    void addRectFilled(
        const glm::vec2 &tl, const glm::vec2 &br, uint32_t color,
        float rounding = 0.f,
        ImDrawCornerFlags roundingCorners = ImDrawCornerFlags_All);

   private:
    ImDrawList *m_drawList;

    glm::vec2 m_renderOrigin;
};

}  // namespace te

#endif /* RENDER_CONTEXT_HPP */
