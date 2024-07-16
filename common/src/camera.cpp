//
// Created by Rokid on 2024/7/16.
//

#include "camera.h"

Camera::Camera() {
    m_position = glm::vec3(1.0f);
    m_front = glm::vec3(1.0f);
    m_up = glm::vec3(1.0f);

    m_speed = 0.01f;

    m_pitch = 0.0f;
    m_yaw = -90.0f;
    m_sensitivity = 0.1f;

    m_xpos = 0;
    m_ypos = 0;
    m_firstMove = true;

    m_vMatrix = glm::mat4(1.0f);
}

Camera::~Camera() {
}

void Camera::lookAt(glm::vec3 _pos, glm::vec3 _front, glm::vec3 _up) {
    m_position = _pos;
    m_front = glm::normalize(_front);
    m_up = _up;
    m_vMatrix = glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::update() {
    m_vMatrix = glm::lookAt(m_position, m_position + m_front, m_up);
}

// 更新位置
void Camera::move(CAMERA_MOVE _mode) {
    switch (_mode)
    {
        case CAMERA_MOVE::MOVE_LEFT:
            m_position -= glm::normalize(glm::cross(m_front, m_up)) * m_speed;
            break;
        case CAMERA_MOVE::MOVE_RIGHT:
            m_position += glm::normalize(
                    glm::cross(m_front, m_up)) * m_speed;
            break;
        case CAMERA_MOVE::MOVE_FRONT:
            m_position += m_speed * m_front;
            break;
        case CAMERA_MOVE::MOVE_BACK:
            m_position -= m_speed * m_front;
            break;
        default:
            break;
    }
}

// 更新上下朝向
void Camera::pitch(float _yOffset) {
    m_pitch += _yOffset * m_sensitivity;

    if (m_pitch >= 89.0f)
    {
        m_pitch = 89.0f;
    }

    if (m_pitch <= -89.0f)
    {
        m_pitch = -89.0f;
    }

    m_front.y = sin(glm::radians(m_pitch));
    m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_front = glm::normalize(m_front);
    update();
}

// 更新左右朝向
void Camera::yaw(float _xOffset) {
    m_yaw += _xOffset * m_sensitivity;

    m_front.y = sin(glm::radians(m_pitch));
    m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_front = glm::normalize(m_front);
    update();
}

void Camera::onMouseMove(double _xpos, double _ypos) {
    if (m_firstMove)
    {
        m_xpos = _xpos;
        m_ypos = _ypos;
        m_firstMove = false;
        return;
    }

    float _xOffset = _xpos - m_xpos;
    float _yOffset = -(_ypos - m_ypos);

    m_xpos = _xpos;
    m_ypos = _ypos;

    pitch(_yOffset);
    yaw(_xOffset);
}
