//
// Created by Rokid on 2024/7/15.
//

#include "glwindow.h"

static const char* vertexShaderSource = R"(
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aUV;
    out vec2 outUV;

    uniform mat4 _modelMatrix;
    uniform mat4 _viewMatrix;
    uniform mat4 _projMatrix;

    void main()
    {
       gl_Position = _projMatrix * _viewMatrix * _modelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
       outUV = aUV;
    };
)";

static const char *fragmentShaderSource = R"(
    out vec4 FragColor;
    in vec2 outUV;
    uniform sampler2D ourTexture;
    void main()
    {
        FragColor = texture(ourTexture, outUV);
    };
)";

// 立方体顶点
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};


GLWindow::GLWindow() {
    resize(800, 600);
}
GLWindow::~GLWindow() {
    makeCurrent();
    m_Program->release();
    delete m_Program;
}

void GLWindow::initializeGL() {
    QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();

    m_Image = ffImage::readFromFile("../../resource/wall.jpg");
    f->glGenTextures(1, &m_texture);
    f->glBindTexture(GL_TEXTURE_2D, m_texture);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Image->getWidth(), m_Image->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Image->getData());
    qDebug() << "Image w=" << m_Image->getWidth() << ", h=" << m_Image->getHeight();

    delete m_Program;
    m_Program = new QOpenGLShaderProgram();
    m_Program->addShaderFromSourceCode(QOpenGLShader::Vertex, versionedShaderCode(vertexShaderSource));
    m_Program->addShaderFromSourceCode(QOpenGLShader::Fragment, versionedShaderCode(fragmentShaderSource));
    m_Program->link();

    m_Program->bind();
    modelMatrixHandle = m_Program->uniformLocation("_modelMatrix");
    viewMatrixHandle = m_Program->uniformLocation("_viewMatrix");
    projMatrixHandle = m_Program->uniformLocation("_projMatrix");

    f->glGenVertexArrays(1, &m_vao);
    f->glBindVertexArray(m_vao);

    f->glGenBuffers(1, &m_vbo);
    f->glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    f->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,5 * sizeof(GLfloat), reinterpret_cast<void *>(0));
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,5 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));

    f->glBindVertexArray(0);
    f->glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLWindow::resizeGL(int w, int h) {
    QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();
    f->glViewport(0, 0, w, h);
}

void GLWindow::paintGL() {
    QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();

    m_Program->bind();

    f->glEnable(GL_DEPTH_TEST);

    f->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    f->glBindTexture(GL_TEXTURE_2D, m_texture);

    glm::vec3 modelVecs[] = {
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
    glm::mat4 _viewMatrix(1.0f);
    glm::mat4 _projMatrix(1.0f);


    _viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    _projMatrix = glm::perspective(glm::radians(45.0f), (float)_width / (float)_height, 0.1f, 100.0f);

    f->glBindVertexArray(m_vao);
    for (int i = 0; i <10; i++) {
        glm::mat4 _modelMatrix(1.0);
        _modelMatrix = glm::translate(_modelMatrix, modelVecs[i]);
        _modelMatrix = glm::rotate(_modelMatrix, glm::radians((float)timer.elapsed() * (i+1) * 10), glm::vec3(0.0f, 1.0f, 0.0f));

        f->glUniformMatrix4fv(modelMatrixHandle, 1, GL_FALSE , glm::value_ptr(_modelMatrix));
        f->glUniformMatrix4fv(viewMatrixHandle, 1, GL_FALSE , glm::value_ptr(_viewMatrix));
        f->glUniformMatrix4fv(projMatrixHandle, 1, GL_FALSE , glm::value_ptr(_projMatrix));

        f->glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    f->glBindVertexArray(0);


    m_Program->release();
}
