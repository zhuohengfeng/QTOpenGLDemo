//
// Created by Rokid on 2024/7/16.
//

#ifndef QTOPENGLDEMO_GLWINDOW_H
#define QTOPENGLDEMO_GLWINDOW_H

#include "base.h"

// 立方体顶点: position, uv, normal
const float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,           0.0f,  0.0f, -1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,           0.0f,  0.0f, -1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,           0.0f,  0.0f, -1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,           0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,           0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,           0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,           0.0f,  0.0f,  1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,           0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,           0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,           0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,           0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,           0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,           -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,           -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,           -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,           -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,           -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,           -1.0f,  0.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,           1.0f,  0.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,           1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,           1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,           1.0f,  0.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,           1.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,           1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,           0.0f, -1.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,           0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,           0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,           0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,           0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,           0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,           0.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,           0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,           0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,           0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,           0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,           0.0f,  1.0f,  0.0f,
};

const glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
};


class GLWindow final : public QOpenGLWindow {
Q_OBJECT

public:
    const int WIDTH = 800;
    const int HEIGHT = 600;

    GLWindow();
    ~GLWindow();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    // 监听按键和鼠标事件
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    QElapsedTimer timer;

    Shader          _shader_cube;
    Shader          _shader_sun;
    Shader          _shader_dir;
    Shader          _shader_point;
    Shader          _shader_spot;

    glm::vec3 light_pos;
    glm::vec3 light_color;

    // 箱体的贴图
    GLuint            m_boardTexture = 0;
    GLuint            m_boxTexture = 0;

    GLuint m_vbo;
    GLuint m_vao;

    glm::mat4 m_projMatrix;
    Camera m_camera;
};


#endif //QTOPENGLDEMO_GLWINDOW_H
