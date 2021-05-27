#ifndef RENDER_CONTEXT_HPP
#define RENDER_CONTEXT_HPP

#include <imgui.h>
#include <glm/glm.hpp>
#include <Component/Transform.hpp>
#include <Graphic/FrameBuffer.hpp>
#include <Entity/Camera.hpp>
#include <Graphic/RenderWindow.hpp>
#include <Component/Transform.hpp>

class RenderContext {
   public:
    RenderContext();

    void setActiveEntityId(int id);

    uint32_t getActiveEntityId() const;

    EntityBase *getActiveEntityPtr();

    void setCamrea(Camera *camera);

    Camera *getCamera();

    void setWindow(RenderWindow *window);

    RenderWindow *getWindow();

    void setFrameBuffer(FrameBuffer *frameBuffer);

    FrameBuffer *getFrameBuffer();

    EntityManager<EntityBase> *getEntityManager();

    void setEntityManager(EntityManager<EntityBase> *entites);

    void prepareContext();

    void unloadContext();

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

    // return the size of clipSize in world space
    float getClipSizeInWorld(float clipSize) const;

   private:
    RenderWindow *m_window;

    FrameBuffer *m_frameBuffer;

    Camera *m_camera;

    EntityManager<EntityBase> *m_entities;

    uint32_t m_activeEntityId;

    float m_screenFactor;

    ImDrawList *m_drawList;

    glm::vec2 m_renderOrigin;
};

#endif /* RENDER_CONTEXT_HPP */
