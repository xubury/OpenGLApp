#include "Entity/PlayerCamera.hpp"
#include "Entity/Player.hpp"

namespace te {

inline const float MOUSE_SENSITIVITY = 0.1f;

PlayerCamera::PlayerCamera(EntityManager<EntityBase> *manager, uint32_t id,
                           int x, int y, int width, int height, Player *player)
    : EntityBase(manager, id),
      Camera(x, y, width, height),
      ActionTarget(m_cameraActionMap),
      m_isFirstMouse(true),
      m_player(player),
      m_dist(10.0f) {
    setName("Player Camera");
    setPosition(m_player->getPosition() + getFront() * 10.0f);
    bind(Action(Event::EventType::MOUSE_MOVED), [this](const Event &event) {
        glm::vec2 currentMousePos =
            glm::vec2(event.mouseMove.x, event.mouseMove.y);
        if (m_isFirstMouse) {
            m_isFirstMouse = false;
        } else {
            glm::vec2 offset = currentMousePos - m_lastMousePos;
            rotate(-offset.y * MOUSE_SENSITIVITY,
                   -offset.x * MOUSE_SENSITIVITY);
        }
        m_lastMousePos = currentMousePos;
    });
}

void PlayerCamera::update(const Time &) {
    setPosition(m_player->getPosition() + getFront() * m_dist);
}

void PlayerCamera::rotate(float pitch, float yaw) {
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
