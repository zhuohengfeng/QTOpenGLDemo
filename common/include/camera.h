//
// Created by Rokid on 2024/7/16.
//

#ifndef QTOPENGLDEMO_CAMERA_H
#define QTOPENGLDEMO_CAMERA_H

#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include"glm/gtc/type_ptr.hpp"

enum class CAMERA_MOVE
{
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_FRONT,
    MOVE_BACK
};

class Camera
{

public:
    Camera();
    ~Camera();

    void lookAt(glm::vec3 _pos, glm::vec3 _front, glm::vec3 _up);
    void update();

    glm::mat4 getMatrix() const {
        return m_vMatrix;
    }

    glm::vec3 getPosition() const {
        return m_position;
    }

    void setSpeed(float speed) {
        m_speed = speed;
    }

    void setSentitivity(float _s) {
        m_sensitivity = _s;
    }

    void move(CAMERA_MOVE _mode);

    void pitch(float _yOffset);
    void yaw(float _xOffset);

    void onMouseMove(double _xpos , double _ypos);

private:
    glm::vec3	m_position;
    glm::vec3	m_front;
    glm::vec3	m_up;

    float		m_speed;

    float		m_pitch;
    float		m_yaw;
    float		m_sensitivity;

    float		m_xpos;
    float       m_ypos;
    bool		m_firstMove;

    glm::mat4	m_vMatrix;

};


#endif //QTOPENGLDEMO_CAMERA_H
