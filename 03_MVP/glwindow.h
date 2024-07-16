//
// Created by Rokid on 2024/7/15.
//

#ifndef QTOPENGLDEMO_GLWINDOW_H
#define QTOPENGLDEMO_GLWINDOW_H

#include "base.h"

class GLWindow final : public QOpenGLWindow{

public:
    GLWindow();
    ~GLWindow();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    const int _width = 800;
    const int _height = 600;

    QOpenGLShaderProgram* m_Program{nullptr};
    QElapsedTimer timer;

    GLuint m_texture;
    ffImage* m_Image{nullptr};

    GLuint m_vbo;
    GLuint m_vao;

    GLint modelMatrixHandle = 0;
    GLint viewMatrixHandle = 0;
    GLint projMatrixHandle = 0;
};


#endif //QTOPENGLDEMO_GLWINDOW_H
