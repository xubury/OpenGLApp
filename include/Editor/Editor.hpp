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
    glm::vec3 screenPos;
    uint32_t color;
};

class Editor {
   public:
    static Editor &instance();

    void render();

    void close();

    EditorContext context;

   private:
    Editor();

    // compute the model's axes coordinate and color
    void computeModelAxes(float len);

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

    glm::vec3 m_modelScreenPos;

    Axis m_modelAxes[3];
};

#endif /* EDITOR_HPP */
