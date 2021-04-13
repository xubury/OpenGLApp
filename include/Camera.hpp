#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include "ActionTarget.hpp"

inline const float YAW = -90.f;
inline const float PITCH = 0.f;
inline const float SPEED = 2.5f;
inline const float ZOOM = 45.f;
inline const float MOUSE_SENSITIVITY = 0.1f;

class Camera : public ActionTarget<int> {
   public:
    enum Movement { FORWARD, BACKWRAD, LEFT, RIGHT };
    static ActionMap<int> cameraMovement;

   public:
    Camera(float width, float height,
           const glm::vec3 &position = glm::vec3(0.f),
           const glm::vec3 &worldUp = glm::vec3(0.f, 1.f, 0.f), float yaw = YAW,
           float pitch = PITCH);

    glm::mat4 getProjection() const;

    glm::mat4 getView() const;

    float getFOV() const;

    float getNearZ() const;

    float getFarZ() const;

    float getAspect() const;

    void move(Movement dir, float val);

    void rotate(float yaw, float pitch, bool constraintPitch = true);

    void zoom(float zoom);

    void setSize(float width, float height);

    void setNearFar(float near, float far);

   private:
    float m_width;
    float m_height;
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;
    float m_yaw;
    float m_pitch;
    float m_zoom;
    float m_nearZ;
    float m_farZ;

    glm::mat4 m_projection;
    glm::mat4 m_view;
    void update();

    glm::vec2 m_lastMousePos;
    bool m_isFirstMouse;
};

#endif
