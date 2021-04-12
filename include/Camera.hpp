#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

inline const float YAW = -90.f;
inline const float PITCH = 0.f;
inline const float SPEED = 2.5f;
inline const float SENSITIVITY = 0.1f;
inline const float ZOOM = 45.f;

class Camera {
   public:
    enum class Movement { FORWARD, BACKWRAD, LEFT, RIGHT };

   public:
    Camera(const glm::vec3 &position = glm::vec3(0.f),
           const glm::vec3 &worldUp = glm::vec3(0.f, 1.f, 0.f), float yaw = YAW,
           float pitch = PITCH);

    glm::mat4 getView();

    void move(Movement dir, float val);

    void rotate(float yaw, float pitch, bool constraintPitch = true);

    void zoom(float zoom);

   private:
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;
    float m_yaw;
    float m_pitch;
    float m_zoom;

    glm::mat4 m_view;
    void update();
    bool m_needUpdate;
};

#endif
