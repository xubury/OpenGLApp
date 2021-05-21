#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <memory>
#include <glm/glm.hpp>
#include <Window/ActionTarget.hpp>
#include <Component/Transform.hpp>

inline const float SPEED = 2.5f;
inline const float ZOOM = 45.f;
inline const float MOUSE_SENSITIVITY = 0.1f;

enum Movement { FORWARD, BACKWRAD, UPWARD, DOWNWARD, LEFT, RIGHT };

class Camera : public Entity, protected ActionTarget<Movement> {
   public:
    using ActionTarget::processEvent;
    using ActionTarget::processEvents;
    using ActionTarget::setActive;

   public:
    Camera(EntityManager<Entity> *manager, uint32_t id, int x, int y,
           int width, int height, const glm::vec3 &position = glm::vec3(0.f));

    virtual ~Camera() = default;

    virtual void draw(RenderTarget &target, RenderStates states) const override;

    glm::vec3 getPosition() const;

    glm::mat4 getProjection() const;

    glm::mat4 getView() const;

    int getX() const;

    int getY() const;

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

    void updateView();

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
    glm::mat4 m_view;
};

class ControlCamera : public Camera {
   public:
    ControlCamera(EntityManager<Entity> *manager, uint32_t id, int x, int y,
                  int width, int height,
                  const glm::vec3 &position = glm::vec3(0.f));

   private:
    glm::vec2 m_lastMousePos;
    bool m_isFirstMouse;
};

#endif
