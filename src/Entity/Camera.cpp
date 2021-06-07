#include <Entity/Camera.hpp>
#include <Component/Transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace te {

ActionMap<Movement> Camera::s_cameraMovement;

Camera::Camera(EntityManager<EntityBase> *manager, uint32_t id, int x, int y,
               int width, int height)
    : CameraBase(x, y, width, height),
      EntityBase(manager, id),
      ActionTarget(s_cameraMovement),
      m_zoom(45.f),
      m_yaw(0),
      m_pitch(0) {
    setEulerAngle(glm::radians(glm::vec3(m_pitch, m_yaw, 0)));
    s_cameraMovement.map(Movement::FORWARD, Keyboard::Key::W);
    s_cameraMovement.map(Movement::BACKWRAD, Keyboard::Key::S);
    s_cameraMovement.map(Movement::LEFT, Keyboard::Key::A);
    s_cameraMovement.map(Movement::RIGHT, Keyboard::Key::D);
    s_cameraMovement.map(Movement::UPWARD, Keyboard::Key::SPACE);
    s_cameraMovement.map(Movement::DOWNWARD, Keyboard::Key::LCONTROL);

    bind(Movement::FORWARD,
         [this](const Event &) { this->move(Movement::FORWARD, 0.1f); });
    bind(Movement::BACKWRAD,
         [this](const Event &) { this->move(Movement::BACKWRAD, 0.1f); });
    bind(Movement::LEFT,
         [this](const Event &) { this->move(Movement::LEFT, 0.1f); });
    bind(Movement::RIGHT,
         [this](const Event &) { this->move(Movement::RIGHT, 0.1f); });
    bind(Movement::UPWARD,
         [this](const Event &) { this->move(Movement::UPWARD, 0.1f); });
    bind(Movement::DOWNWARD,
         [this](const Event &) { this->move(Movement::DOWNWARD, 0.1f); });
    bind(Action(Event::EventType::MOUSE_WHEEL_SCROLLED),
         [this](const Event &event) { zoom(event.mouseWheel.yOffset); });
}

glm::mat4 Camera::getView() const {
    Transform::Handle trans = component<Transform>();
    const glm::vec3 &up = trans->getUp();
    const glm::vec3 &front = trans->getFront();
    const glm::vec3 &pos = trans->getPosition();
    return glm::lookAt(pos, pos - front, up);
}

glm::mat4 Camera::getProjection() const {
    return glm::perspective(glm::radians(getFOV()), getAspect(), getNearZ(),
                            getFarZ());
}

void Camera::move(Movement dir, float val) {
    Transform::Handle trans = component<Transform>();
    if (dir == Movement::FORWARD) {
        trans->translateLocal(glm::vec3(0, 0, -1) * val);
    } else if (dir == Movement::BACKWRAD) {
        trans->translateLocal(glm::vec3(0, 0, 1) * val);
    } else if (dir == Movement::LEFT) {
        trans->translateLocal(glm::vec3(-1, 0, 0) * val);
    } else if (dir == Movement::RIGHT) {
        trans->translateLocal(glm::vec3(1, 0, 0) * val);
    } else if (dir == Movement::UPWARD) {
        trans->translateLocal(glm::vec3(0, 1, 0) * val);
    } else if (dir == Movement::DOWNWARD) {
        trans->translateLocal(glm::vec3(0, -1, 0) * val);
    }
}

void Camera::rotate(float yaw, float pitch, bool constraintPitch) {
    m_yaw += yaw;
    m_pitch += pitch;
    if (constraintPitch) {
        if (m_pitch > 89.f) {
            m_pitch = 89.f;
        } else if (m_pitch < -89.f) {
            m_pitch = -89.f;
        }
    }
    setEulerAngle(glm::radians(glm::vec3(m_pitch, m_yaw, 0)));
}

void Camera::zoom(float zoom) {
    m_zoom -= zoom;
    if (m_zoom < 1.f)
        m_zoom = 1.f;
    else if (m_zoom > 45.f)
        m_zoom = 45.f;
}

float Camera::getFOV() const { return m_zoom; }

}  // namespace te
