
#include "glwindow.h"

#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLExtraFunctions>

static const char* vertexShaderSource = R"(
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    out vec4 outColor;
    void main()
    {
       gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
       outColor = vec4(aColor , 1.0);
    };
)";

static const char *fragmentShaderSource = R"(
    out vec4 FragColor;
    uniform vec4 ourColor;
    in vec4 outColor;
    void main()
    {
        FragColor = outColor;
    };
)";

float vertices[] =
    {
        -0.5f , -0.5f , 0.0f,   1.0f , 0.0f , 0.0f,
        0.5f , -0.5f , 0.0f,    0.0f , 1.0f , 0.0f,
        0.0f , 0.5f , 0.0f,     0.0f , 0.0f , 1.0f,
    };


QByteArray versionedShaderCode(const char *src)
{
    QByteArray versionedSrc;
    if (QOpenGLContext::currentContext()->isOpenGLES())
        versionedSrc.append(QByteArrayLiteral("#version 300 es\n"));
    else
        versionedSrc.append(QByteArrayLiteral("#version 330\n"));
    versionedSrc.append(src);
    return versionedSrc;
}

GLWindow::GLWindow() {
}

GLWindow::~GLWindow() {
    makeCurrent();
    delete m_program;
    delete m_vbo;
    delete m_vao;
}

void GLWindow::initializeGL() {
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

    delete m_program;
    m_program = new QOpenGLShaderProgram;
    // Prepend the correct version directive to the sources. The rest is the
    // same, thanks to the common GLSL syntax.
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, versionedShaderCode(vertexShaderSource));
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, versionedShaderCode(fragmentShaderSource));
    m_program->link();

    // Create a VAO. Not strictly required for ES 3, but it is for plain OpenGL.
    delete m_vao;
    m_vao = new QOpenGLVertexArrayObject;
    if (m_vao->create())
        m_vao->bind();


    delete m_vbo;
    m_vbo = new QOpenGLBuffer;
    m_vbo->create();
    m_vbo->bind();
    m_vbo->allocate(vertices, sizeof(vertices));
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                             nullptr);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                             reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    m_vbo->release();
    m_vao->release();

    f->glEnable(GL_DEPTH_TEST);
    f->glEnable(GL_CULL_FACE);
}

void GLWindow::resizeGL(int w, int h) {
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();
    f->glViewport(0, 0, w, h);
}

void GLWindow::paintGL() {
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

    f->glClearColor(0, 0, 0, 1);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();

    m_vao->bind();
    f->glDrawArrays(GL_TRIANGLES, 0, 3);
}
