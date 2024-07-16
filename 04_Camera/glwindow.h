//
// Created by Rokid on 2024/7/16.
//

#ifndef QTOPENGLDEMO_GLWINDOW_H
#define QTOPENGLDEMO_GLWINDOW_H

#include "base.h"

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

    QOpenGLShaderProgram* m_Program{nullptr};
    QElapsedTimer timer;

    GLuint m_texture;
    ffImage* m_Image{nullptr};

    GLuint m_vbo;
    GLuint m_vao;

    GLint m_modelMatrixHandle;
    GLint m_viewMatrixHandle;
    GLint m_projMatrixHandle;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projMatrix;

    Camera m_camera;
};


#endif //QTOPENGLDEMO_GLWINDOW_H
