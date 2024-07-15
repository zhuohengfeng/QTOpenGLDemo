#ifndef QTOPENGLDEMO_GLWINDOW_H
#define QTOPENGLDEMO_GLWINDOW_H

#include "base.h"
#include <QOpenGLWindow>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWindow final : public QOpenGLWindow
{
    Q_OBJECT

public:
    GLWindow();
    ~GLWindow();

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    QOpenGLShaderProgram *m_program = nullptr;

    GLuint m_vbo;
    GLuint m_vao;
};



#endif //QTOPENGLDEMO_GLWINDOW_H
