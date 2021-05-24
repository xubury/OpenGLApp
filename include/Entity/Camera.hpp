#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <Entity/EntityBase.hpp>
#include <Window/ActionTarget.hpp>
#include <Graphic/RenderTarget.hpp>

inline const float SPEED = 2.5f;
inline const float ZOOM = 45.f;
inline const float MOUSE_SENSITIVITY = 0.1f;

enum Movement { FORWARD, BACKWRAD, UPWARD, DOWNWARD, LEFT, RIGHT };

class Camera : public EntityBase, protected ActionTarget<Movement> {
   public:
    using ActionTarget::processEvent;
    using ActionTarget::processEvents;
    using ActionTarget::setActive;

   public:
    Camera(EntityManager<EntityBase> *manager, uint32_t id, int x, int y,
           int width, int height, const glm::vec3 &position = glm::vec3(0.f));

    virtual ~Camera() = default;

    virtual void draw(RenderTarget &target, RenderStates states) const override;

    glm::vec3 getPosition() const;

    glm::mat4 getProjection() const;

    glm::mat4 getView() const;

    int getX() const;

    int getY() const;

    glm::vec2 getViewportPos() const;

    glm::vec2 getViewportSize() const;

    int getWidth() const;

    int getHeight() const;

    float getFOV() const;

    float getNearZ() const;

    float getFarZ() const;

    float getAspect() const;

    void setSize(float width, float height);

    void setNearFar(float near, float far);

    void move(Movement dir, float val);

    void rotate(float yaw, float pitch, bool constraintPitch = true);

    void zoom(float zoom);

    void computeCameraRay(glm::vec3 &rayOrigin, glm::vec3 &rayDir,
                          const glm::vec2 &screenPos) const;

    glm::vec3 computeWorldToSrceen(const glm::vec3 &worldPos) const;

    float getSegmentLengthClipSpace(const glm::vec3 &start,
                                    const glm::vec3 &end) const;

   protected:
    static ActionMap<Movement> s_cameraMovement;

   private:
    int m_x;
    int m_y;
    int m_width;
    int m_height;

    float m_yaw;
    float m_pitch;

    float m_zoom;
    float m_nearZ;
    float m_farZ;

    glm::mat4 m_projection;
};

class ControlCamera : public Camera {
   public:
    ControlCamera(EntityManager<EntityBase> *manager, uint32_t id, int x, int y,
                  int width, int height,
                  const glm::vec3 &position = glm::vec3(0.f));

   private:
    glm::vec2 m_lastMousePos;
    bool m_isFirstMouse;
};

#endif
