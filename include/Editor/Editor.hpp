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

    void computeProjectionView();

    glm::vec3 computeWorldToSrceen(const glm::vec3 &localPos);

    void computeCameraRay();

    void computeModelAxes(float len);

    void renderFps();

    void renderAxis(const glm::vec2 &origin, const glm::vec2 &axis, ImU32 color,
                    float thickness = 5.f);

    void renderModelAxes();

    void renderCameraAxes(float len);

    int m_activeEntityId;

    glm::mat4 m_projectionView;

    ImDrawList *m_drawList;

    glm::vec2 m_renderOrigin;

    glm::vec2 m_renderSize;

    glm::vec4 m_camRayOrigin;

    glm::vec4 m_camRayEnd;

    glm::vec3 m_modelScreenPos;

    Axis m_modelAxes[3];
};

#endif /* EDITOR_HPP */
