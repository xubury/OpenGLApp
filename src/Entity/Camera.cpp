#include <Entity/Camera.hpp>
#include <Component/Transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

ActionMap<Movement> Camera::s_cameraMovement;

ES_INIT_ENTITY(Camera)

Camera::Camera(EntityManager<EntityBase> *manager, uint32_t id, int x, int y,
               int width, int height, const glm::vec3 &position)
    : EntityBase(manager, id),
      ActionTarget(s_cameraMovement),
      m_x(x),
      m_y(y),
      m_width(width),
      m_height(height),
      m_yaw(0),
      m_pitch(0),
      m_zoom(ZOOM),
      m_nearZ(0.1f),
      m_farZ(100.f) {
    component<Transform>()->setPosition(position);
    component<Transform>()->setEulerAngle(
        glm::radians(glm::vec3(m_pitch, m_yaw, 0)));
    m_projection = glm::perspective(glm::radians(getFOV()), getAspect(),
                                    getNearZ(), getFarZ());
}

void Camera::draw(RenderTarget &, RenderStates) const {}

glm::vec3 Camera::getPosition() const {
    return component<Transform>()->getPosition();
}

glm::mat4 Camera::getProjection() const { return m_projection; }

glm::mat4 Camera::getView() const {
    // TODO: view == glm::inverse(model)
    // we can get view directly from camera model matrix
    auto trans = component<Transform>();
    glm::vec3 up = trans->getUp();
    glm::vec3 pos = trans->getPosition();
    glm::vec3 front = trans->getFront();
    return glm::lookAt(pos, pos - front, up);
}

int Camera::getX() const { return m_x; }

int Camera::getY() const { return m_y; }

glm::vec2 Camera::getViewportPos() const { return glm::vec2(m_x, m_y); }

int Camera::getWidth() const { return m_width; }

int Camera::getHeight() const { return m_height; }

glm::vec2 Camera::getViewportSize() const {
    return glm::vec2(m_width, m_height);
}

float Camera::getFOV() const { return m_zoom; }

float Camera::getNearZ() const { return m_nearZ; }

float Camera::getFarZ() const { return m_farZ; }

float Camera::getAspect() const { return (float)m_width / m_height; }

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
    component<Transform>()->setEulerAngle(
        glm::radians(glm::vec3(m_pitch, m_yaw, 0)));
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

void Camera::computeCameraRay(glm::vec3 &rayOrigin, glm::vec3 &rayDir,
                              const glm::vec2 &screenPos) const {
    glm::vec2 mouseClipPos((screenPos.x - m_x) / m_width,
                           (screenPos.y - m_y) / m_height);
    mouseClipPos.x = mouseClipPos.x * 2.f - 1.f;
    mouseClipPos.y = (1.f - mouseClipPos.y) * 2.f - 1.f;
    const float zNear = 0.f;
    const float zFar = 1.f - std::numeric_limits<float>::epsilon();

    glm::mat4 inversePV = glm::inverse(getProjection() * getView());
    glm::vec4 rayOriginH = inversePV * glm::vec4(mouseClipPos, zNear, 1.0f);
    rayOriginH /= rayOriginH.w;
    rayOrigin = rayOriginH;

    glm::vec4 rayEnd = inversePV * glm::vec4(mouseClipPos, zFar, 1.0f);
    rayEnd /= rayEnd.w;

    rayDir = glm::normalize(glm::vec3(rayEnd) - rayOrigin);
}

glm::vec3 Camera::computeWorldToSrceen(const glm::vec3 &worldPos) const {
    glm::mat4 projectionView = getProjection() * getView();
    glm::vec4 clipPos = projectionView * glm::vec4(worldPos, 1.0f);
    clipPos /= clipPos.w;

    glm::vec3 screenPos;
    screenPos.x = (clipPos.x + 1) * 0.5 * m_width + m_x;
    screenPos.y = (1 - clipPos.y) * 0.5 * m_height + m_y;
    screenPos.z = clipPos.z;
    return screenPos;
}

float Camera::getSegmentLengthClipSpace(const glm::vec3 &start,
                                        const glm::vec3 &end) const {
    glm::mat4 projectionView = getProjection() * getView();
    glm::vec4 segStart = projectionView * glm::vec4(start, 1.0f);
    if (std::fabs(segStart.w) > std::numeric_limits<float>::epsilon()) {
        segStart /= segStart.w;
    }

    glm::vec4 segEnd = projectionView * glm::vec4(end, 1.0f);
    if (std::fabs(segEnd.w) > std::numeric_limits<float>::epsilon()) {
        segEnd /= segEnd.w;
    }

    glm::vec2 clipSpaceAxis = segEnd - segStart;
    clipSpaceAxis.y /= getAspect();
    return glm::length(clipSpaceAxis);
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
            glm::vec2 offset = currentMousePos - m_lastMousePos;
            rotate(-offset.x * MOUSE_SENSITIVITY,
                   -offset.y * MOUSE_SENSITIVITY);
        }
        m_lastMousePos = currentMousePos;
    });
    bind(Action(Event::EventType::MOUSE_WHEEL_SCROLLED),
         [this](const Event &event) { zoom(event.mouseWheel.yOffset); });
}
