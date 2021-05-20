#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <memory>
#include <glm/glm.hpp>
#include <Window/ActionTarget.hpp>

inline const float YAW = 0.f;
inline const float PITCH = 0.f;
inline const float SPEED = 2.5f;
inline const float ZOOM = 45.f;
inline const float MOUSE_SENSITIVITY = 0.1f;

enum Movement { FORWARD, BACKWRAD, UPWARD, DOWNWARD, LEFT, RIGHT };

class Camera : protected ActionTarget<Movement> {
   public:
    static const Camera Default;

    using ActionTarget::setActive;

   public:
    Camera(int x, int y, int width, int height,
           const glm::vec3 &position = glm::vec3(0.f), float yaw = YAW,
           float pitch = PITCH);

    virtual ~Camera() = default;

    glm::vec3 getPosition() const;

    void setPosition(float x, float y, float z);

    glm::vec3 getPitchYawRoll() const;

    void setPitchYawRoll(float pitch, float yaw, float roll);

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

   protected:
    static ActionMap<Movement> s_cameraMovement;

   private:
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;

    float m_yaw;
    float m_pitch;
    float m_roll;

    float m_zoom;
    float m_nearZ;
    float m_farZ;

    glm::mat4 m_projection;
    glm::mat4 m_view;

    void update();
};

class ControlCamera : public Camera {
   public:
    ControlCamera(int x, int y, int width, int height,
                  const glm::vec3 &position = glm::vec3(0.f), float yaw = YAW,
                  float pitch = PITCH);

    virtual bool processEvent(const Event &event) const override;

    virtual void processEvents() const override;

   private:
    glm::vec2 m_lastMousePos;
    bool m_isFirstMouse;
};

#endif
