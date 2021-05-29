#include <Entity/Camera.hpp>
#include <Component/Transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

ActionMap<Movement> Camera::s_cameraMovement;

Camera::Camera(EntityManager<EntityBase> *manager, uint32_t id, int x, int y,
               int width, int height, const glm::vec3 &position)
    : CameraBase(x, y, width, height),
      EntityBase(manager, id),
      ActionTarget(s_cameraMovement),
      m_yaw(0),
      m_pitch(0) {
    component<Transform>()->setPosition(position);
    component<Transform>()->setEulerAngle(
        glm::radians(glm::vec3(m_pitch, m_yaw, 0)));
}

glm::mat4 Camera::getView() const {
    auto trans = component<Transform>();
    const glm::vec3 &up = trans->getUp();
    const glm::vec3 &front = trans->getFront();
    const glm::vec3 &pos = trans->getPosition();
    return glm::lookAt(pos, pos - front, up);
}

void Camera::draw(RenderTarget &, RenderStates) const {}

void Camera::move(Movement dir, float val) {
    auto trans = component<Transform>();
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
    auto trans = component<Transform>();
    trans->setEulerAngle(glm::radians(glm::vec3(m_pitch, m_yaw, 0)));
}

ControlCamera::ControlCamera(EntityManager<EntityBase> *manager, uint32_t id,
                             int x, int y, int width, int height,
                             const glm::vec3 &position)
    : Camera(manager, id, x, y, width, height, position), m_isFirstMouse(true) {
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
    bind(Action(Event::EventType::MOUSE_MOVED), [this](const Event &event) {
        if (!isActive()) return;
        glm::vec2 currentMousePos =
            glm::vec2(event.mouseMove.x, event.mouseMove.y);
        if (m_isFirstMouse) {
            m_isFirstMouse = false;
        } else {
            // glm::vec2 offset = currentMousePos - m_lastMousePos;
            // rotate(-offset.x * MOUSE_SENSITIVITY,
            //        -offset.y * MOUSE_SENSITIVITY);
        }
        m_lastMousePos = currentMousePos;
    });
    bind(Action(Event::EventType::MOUSE_WHEEL_SCROLLED),
         [this](const Event &event) { zoom(event.mouseWheel.yOffset); });
}
