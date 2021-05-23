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

struct EditorContext {
    RenderWindow *window;
    FrameBuffer *frameBuffer;
    Camera *camera;
    EntityManager<EntityBase> *entities;
};

class Transform;

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

    EditorContext context;

   private:
    Editor();

    // build the model's axes coordinate and color
    void buildModelAxes(float len);

    // build model's X-Y X-Z Y-Z plane
    void buildModelPlane();

    void renderFps();

    void renderAxis(const glm::vec2 &origin, const glm::vec2 &axis, ImU32 color,
                    float thickness = 5.f);

    void renderModelAxes();

    void renderCameraAxes(float len);

    int m_activeEntityId;

    ImDrawList *m_drawList;

    glm::vec2 m_renderOrigin;

    glm::vec3 m_camRayOrigin;

    glm::vec3 m_camRayDir;

    Axes m_modelScreenAxes;
    int m_axesDrawingOrder[3];

    glm::vec4 m_planeXY;

    glm::vec4 m_planeXZ;

    glm::vec4 m_planeYZ;
};

#endif /* EDITOR_HPP */
