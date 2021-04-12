#include "Camera.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>

Camera::Camera(float width, float height, const glm::vec3 &position,
               const glm::vec3 &worldUp, float yaw, float pitch)
    : m_width(width),
      m_height(height),
      m_position(position),
      m_front(glm::vec3(0.f, 0.f, -1.0f)),
      m_worldUp(worldUp),
      m_yaw(yaw),
      m_pitch(pitch),
      m_zoom(ZOOM),
      m_nearZ(0.1f),
      m_farZ(100.f) {
    update();
    m_projection = glm::perspective(glm::radians(getFOV()), getAspect(),
                                    getNearZ(), getFarZ());
}

glm::mat4 Camera::getProjection() const { return m_projection; }

glm::mat4 Camera::getView() const { return m_view; }

float Camera::getFOV() const { return m_zoom; }

float Camera::getNearZ() const { return m_nearZ; }

float Camera::getFarZ() const { return m_farZ; }

float Camera::getAspect() const { return m_width / m_height; }

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

void Camera::update() {
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
    m_view = glm::lookAt(m_position, m_position + m_front, m_up);
}
