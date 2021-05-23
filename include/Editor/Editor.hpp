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

class Editor {
   public:
    static Editor &instance();

    void render();

    void close();

    EditorContext context;

   private:
    Editor();

    void computeProjectionView();

    void computeCameraRay();

    glm::vec3 computeWorldToSrceen(const glm::vec3 &localPos);

    void renderFps();

    void renderAxis(const glm::vec2 &origin, const glm::vec2 &axis, ImU32 color,
                    float thickness = 5.f);

    void renderModelAxes();

    void renderCameraAxes();

    int m_activeEntityId;

    glm::mat4 m_projectionView;

    ImDrawList *m_drawList;

    glm::vec2 m_renderOrigin;

    glm::vec4 m_camRayOrigin;

    glm::vec4 m_camRayEnd;
};

#endif /* EDITOR_HPP */
