#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <memory>
#include <glm/glm.hpp>
#include <Window/ActionTarget.hpp>

inline const float YAW = -90.f;
inline const float PITCH = 0.f;
inline const float SPEED = 2.5f;
inline const float ZOOM = 45.f;
inline const float MOUSE_SENSITIVITY = 0.1f;

class Camera {
   public:
    static const Camera Default;

    enum Movement { FORWARD, BACKWRAD, UPWARD, DOWNWARD, LEFT, RIGHT };

   public:
    Camera(int x, int y, int width, int height,
           const glm::vec3 &position = glm::vec3(0.f),
           const glm::vec3 &worldUp = glm::vec3(0.f, 1.f, 0.f), float yaw = YAW,
           float pitch = PITCH);

    virtual ~Camera() = default;

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

    virtual bool processEvent(const Event &event) const;

    virtual void processEvents() const;

   private:
    int m_x;
    int m_y;
    int m_width;
    int m_height;
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
};

class ControlCamera : public Camera, protected ActionTarget<Camera::Movement> {
   public:
    ControlCamera(int x, int y, int width, int height,
                  const glm::vec3 &position = glm::vec3(0.f),
                  const glm::vec3 &worldUp = glm::vec3(0.f, 1.f, 0.f),
                  float yaw = YAW, float pitch = PITCH);

    virtual bool processEvent(const Event &event) const override;

    virtual void processEvents() const override;

   private:
    static ActionMap<Camera::Movement> s_cameraMovement;
    glm::vec2 m_lastMousePos;
    bool m_isFirstMouse;
};

#endif
