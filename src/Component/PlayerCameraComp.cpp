#include "Component/PlayerCameraComp.hpp"

namespace te {

PlayerCameraComp::PlayerCameraComp(int x, int y, int width, int height)
    : CameraComp(x, y, width, height), m_dist(10.0f) {}

void PlayerCameraComp::rotate(float pitch, float yaw) {
    m_yaw += yaw;
    m_pitch += pitch;
    if (m_pitch > 0.f) {
        m_pitch = 0.f;
    } else if (m_pitch < -89.f) {
        m_pitch = -89.f;
    }
    setEulerAngle(glm::radians(glm::vec3(m_pitch, m_yaw, 0)));
}

}  // namespace te
