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

    Shader m_shader;

    glm::vec3 light_pos;
    glm::vec3 light_color;

    // 箱体的贴图
    GLuint            m_boxTexture = 0;
    GLuint            m_windowTexture = 0;

    GLuint m_cube_vao;
    GLuint m_plane_vao;
    GLuint m_window_vao;

    glm::mat4 m_projMatrix;
    Camera m_camera;

};


#endif //QTOPENGLDEMO_GLWINDOW_H
