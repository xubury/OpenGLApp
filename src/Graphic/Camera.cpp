#include <Graphic/Camera.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

const Camera Camera::Default = Camera(0, 0, 1, 1);

ActionMap<Movement> Camera::s_cameraMovement;

Camera::Camera(int x, int y, int width, int height, const glm::vec3 &position,
               float yaw, float pitch)
    : ActionTarget(s_cameraMovement),
      m_x(x),
      m_y(y),
      m_width(width),
      m_height(height),
      m_position(position),
      m_front(glm::vec3(0.f, 0.f, -1.0f)),
      m_yaw(yaw),
      m_pitch(pitch),
      m_roll(0),
      m_zoom(ZOOM),
      m_nearZ(0.1f),
      m_farZ(100.f) {
    update();
    m_projection = glm::perspective(glm::radians(getFOV()), getAspect(),
                                    getNearZ(), getFarZ());
}

glm::vec3 Camera::getPosition() const { return m_position; }

void Camera::setPosition(float x, float y, float z) {
    m_position[0] = x;
    m_position[1] = y;
    m_position[2] = z;
    update();
}

glm::vec3 Camera::getPitchYawRoll() const {
    return glm::vec3(m_pitch, m_yaw, m_roll);
}

void Camera::setPitchYawRoll(float pitch, float yaw, float roll) {
    m_pitch = pitch;
    m_yaw = yaw;
    m_roll = roll;
    update();
}

glm::mat4 Camera::getProjection() const { return m_projection; }

glm::mat4 Camera::getView() const { return m_view; }

int Camera::getX() const { return m_x; }

int Camera::getY() const { return m_y; }

int Camera::getWidth() const { return m_width; }

int Camera::getHeight() const { return m_height; }

float Camera::getFOV() const { return m_zoom; }

float Camera::getNearZ() const { return m_nearZ; }

float Camera::getFarZ() const { return m_farZ; }

float Camera::getAspect() const { return (float)m_width / m_height; }

void Camera::move(Movement dir, float val) {
    if (dir == Movement::FORWARD) {
        m_position -= m_front * val;
    } else if (dir == Movement::BACKWRAD) {
        m_position += m_front * val;
    } else if (dir == Movement::LEFT) {
        m_position -= m_right * val;
    } else if (dir == Movement::RIGHT) {
        m_position += m_right * val;
    } else if (dir == Movement::UPWARD) {
        m_position += m_up * val;
    } else if (dir == Movement::DOWNWARD) {
        m_position -= m_up * val;
    }
    update();
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
    update();
}

void Camera::zoom(float zoom) {
    m_zoom -= zoom;
    if (m_zoom < 1.f)
        m_zoom = 1.f;
    else if (m_zoom > 45.f)
        m_zoom = 45.f;
    m_projection = glm::perspective(glm::radians(getFOV()), getAspect(),
                                    getNearZ(), getFarZ());
}

void Camera::setSize(float width, float height) {
    m_width = width;
    m_height = height;
    m_projection = glm::perspective(glm::radians(getFOV()), getAspect(),
                                    getNearZ(), getFarZ());
}

void Camera::setNearFar(float near, float far) {
    m_nearZ = near;
    m_farZ = far;
    m_projection = glm::perspective(glm::radians(getFOV()), getAspect(),
                                    getNearZ(), getFarZ());
}

bool Camera::processEvent(const Event &) const { return false; }

void Camera::processEvents() const {}

void Camera::update() {
    glm::vec3 right;
    right.x = cos(glm::radians(m_roll)) * cos(glm::radians(m_yaw));
    right.y = sin(glm::radians(m_roll)) * cos(glm::radians(m_yaw));
    right.z = -sin(glm::radians(m_yaw));
    glm::vec3 up;
    up.x = cos(glm::radians(m_roll)) * sin(glm::radians(m_yaw)) *
               sin(glm::radians(m_pitch)) -
           sin(glm::radians(m_roll)) * cos(glm::radians(m_pitch));
    up.y = sin(glm::radians(m_roll)) * sin(glm::radians(m_yaw)) *
               sin(glm::radians(m_pitch)) +
           cos(glm::radians(m_roll)) * cos(glm::radians(m_pitch));
    up.z = cos(glm::radians(m_roll)) * sin(glm::radians(m_pitch));
    m_right = glm::normalize(right);
    m_up = glm::normalize(up);
    m_front = glm::normalize(glm::cross(m_right, m_up));
    m_view = glm::lookAt(m_position, m_position - m_front, m_up);
}

ControlCamera::ControlCamera(int x, int y, int width, int height,
                             const glm::vec3 &position, float yaw, float pitch)
    : Camera(x, y, width, height, position, yaw, pitch), m_isFirstMouse(true) {
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
        glm::vec2 currentMousePos =
            glm::vec2(event.mouseMove.x, event.mouseMove.y);
        if (m_isFirstMouse) {
            m_isFirstMouse = false;
        } else {
            glm::vec2 offset = currentMousePos - m_lastMousePos;
            rotate(-offset.x * MOUSE_SENSITIVITY, -offset.y * MOUSE_SENSITIVITY);
        }
        m_lastMousePos = currentMousePos;
    });
    bind(Action(Event::EventType::MOUSE_WHEEL_SCROLLED),
         [this](const Event &event) { zoom(event.mouseWheel.yOffset); });
}

bool ControlCamera::processEvent(const Event &event) const {
    return ActionTarget::processEvent(event);
}

void ControlCamera::processEvents() const { ActionTarget::processEvents(); }
