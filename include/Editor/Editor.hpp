#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <glm/glm.hpp>
#include <imgui.h>

class RenderWindow;

class FrameBuffer;

class Camera;

template <class ENTITY>
class EntityManager;

class EntityBase;

class RenderContext {
   public:
    RenderWindow *window;

    FrameBuffer *frameBuffer;

    Camera *camera;

    EntityManager<EntityBase> *entities;

    void prepareContext();

    // get window pos relateive to current context window
    glm::vec2 getContextScreenPos();

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

struct Axis {
    glm::vec3 pos;
    uint32_t color;
};

struct Axes {
    glm::vec3 origin;
    Axis axes[3];
};

class Editor {
   public:
    static Editor &instance();

    void render();

    void close();

    RenderContext context;

   private:
    Editor();

    // build the model's axes coordinate and color
    void buildModelAxes(float len);

    // build model's X-Y X-Z Y-Z plane
    void buildModelPlane();

    void renderFps();

    void renderModelAxes();

    void renderCameraAxes(float len);

    int m_activeEntityId;

    glm::vec3 m_camRayOrigin;

    glm::vec3 m_camRayDir;

    Axes m_modelScreenAxes;

    int m_axesDrawingOrder[3];

    glm::vec4 m_planeXY;

    glm::vec4 m_planeXZ;

    glm::vec4 m_planeYZ;
};

#endif /* EDITOR_HPP */
