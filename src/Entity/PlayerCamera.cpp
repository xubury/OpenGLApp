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
      m_player(player) {
    setName("Player Camera");
    float dist = 10.f;
    float angle = glm::radians(0.0f);
    glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f),
                                      glm::vec3(0.f, 1.0f, 0.f));
    transform = glm::rotate(transform, -angle, glm::vec3(1.f, 0.f, 0.f));
    setTransform(transform);
    setPosition(player->getPosition() +
                glm::vec3(0, dist * sin(angle), dist * cos(angle)));
    bind(Action(Event::EventType::MOUSE_MOVED), [this](const Event &event) {
        glm::vec2 currentMousePos =
            glm::vec2(event.mouseMove.x, event.mouseMove.y);
        if (m_isFirstMouse) {
            m_isFirstMouse = false;
        } else {
            glm::vec2 offset = currentMousePos - m_lastMousePos;
            glm::mat4 transform(1.0f);
            const glm::vec3 &modelWorldPos = m_player->getPosition();
            transform = glm::translate(transform, modelWorldPos);
            transform = glm::rotate(transform,
                                    glm::radians(-offset.y * MOUSE_SENSITIVITY),
                                    getLeft());
            transform = glm::rotate(transform,
                                    glm::radians(-offset.x * MOUSE_SENSITIVITY),
                                    glm::vec3(0, 1, 0));
            transform = glm::translate(transform, -modelWorldPos);
            setTransform(transform * getTransform());
        }
        m_lastMousePos = currentMousePos;
    });
}

void PlayerCamera::update(const Time &) {
    setPosition(m_player->getPosition() + getFront() * 10.0f);
}

}  // namespace te
