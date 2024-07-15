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

    QOpenGLShaderProgram* m_Program{nullptr};

    GLuint m_vbo;
    GLuint m_vao;
};


#endif //QTOPENGLDEMO_GLWINDOW_H
