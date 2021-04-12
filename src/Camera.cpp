#include "Camera.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <iostream>

Camera::Camera(const glm::vec3 &position, const glm::vec3 &worldUp, float yaw,
               float pitch)
    : m_position(position),
      m_front(glm::vec3(0.f, 0.f, -1.0f)),
      m_worldUp(worldUp),
      m_yaw(yaw),
      m_pitch(pitch),
      m_zoom(ZOOM),
      m_view(1.0f),
      m_needUpdate(true) {
    update();
}

glm::mat4 Camera::getView() {
    if (m_needUpdate) {
        m_view = glm::lookAt(m_position, m_position + m_front, m_up);
        m_needUpdate = false;
    }
    return m_view;
}

void Camera::move(Movement dir, float val) {
    if (dir == Movement::FORWARD) {
        m_position += m_front * val;
    } else if (dir == Movement::BACKWRAD) {
        m_position -= m_front * val;
    } else if (dir == Movement::LEFT) {
        m_position -= m_right * val;
    } else if (dir == Movement::RIGHT) {
        m_position += m_right * val;
    }
    update();
}

void Camera::rotate(float yaw, float pitch, bool constraintPitch) {
    m_yaw += yaw;
    m_pitch += pitch;
    if (constraintPitch) {
        if (pitch > 89.f) {
            pitch = 89.f;
        } else if (pitch < -89.f) {
            pitch = -89.f;
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
    update();
}

void Camera::update() {
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
    m_needUpdate = true;
}
