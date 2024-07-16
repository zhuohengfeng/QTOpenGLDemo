//
// Created by Rokid on 2024/7/16.
//

#include "glwindow.h"

const static char* vertexShaderStr = R"(
    layout(location=0) in vec3 aPos;
    layout(location=1) in vec2 aUV;
    out vec2 outUV;
    uniform mat4 _projMatrix;
    uniform mat4 _viewMatrix;
    uniform mat4 _modelMatrix;
    void main()
    {
        gl_Position = _projMatrix * _viewMatrix * _modelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
        outUV = aUV;
    };
)";

const static char* fragmentShaderStr = R"(
    in vec2 outUV;
    out vec4 FragColor;
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

GLWindow::GLWindow() {
    resize(WIDTH, HEIGHT);

    m_camera.lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_camera.setSpeed(0.05f);
    m_projMatrix = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
}

GLWindow::~GLWindow() {
    makeCurrent();
    m_Program->release();
    delete m_Program;
}

void GLWindow::initializeGL() {
    QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();

    // 创建Program
    m_Program = new QOpenGLShaderProgram();
    m_Program->addShaderFromSourceCode(QOpenGLShader::Vertex, versionedShaderCode(vertexShaderStr));
    m_Program->addShaderFromSourceCode(QOpenGLShader::Fragment, versionedShaderCode(fragmentShaderStr));
    m_Program->link();
    m_Program->bind();
    m_modelMatrixHandle = m_Program->uniformLocation("_modelMatrix");
    m_viewMatrixHandle = m_Program->uniformLocation("_viewMatrix");
    m_projMatrixHandle = m_Program->uniformLocation("_projMatrix");
    m_Program->release();

    // 绑定纹理
    m_Image = ffImage::readFromFile("../../resource/wall.jpg");
    f->glGenTextures(1, &m_texture);
    f->glBindTexture(GL_TEXTURE_2D, m_texture);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Image->getWidth(), m_Image->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Image->getData());
    f->glBindTexture(GL_TEXTURE_2D, 0);
    qDebug() << "Image w=" << m_Image->getWidth() << ", h=" << m_Image->getHeight();

    // 绑定VAO VBO
    f->glGenVertexArrays(1, &m_vao);
    f->glBindVertexArray(m_vao);

    f->glGenBuffers(1, &m_vbo);
    f->glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    f->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void *>(0));
    f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void *>(3*sizeof(GLfloat)));
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);

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

    f->glBindVertexArray(m_vao);
    f->glBindTexture(GL_TEXTURE_2D, m_texture);

    m_camera.update();
    for (int i = 0; i < 10; i++) {
        glm::mat4 modelMatrix = glm::mat4(1.0);
        modelMatrix = glm::translate(modelMatrix, modelVecs[i]);
        modelMatrix = glm::rotate(modelMatrix, glm::radians((float)timer.elapsed() * (i+1) * 10), glm::vec3(0.0f, 1.0f, 0.0f));

        f->glUniformMatrix4fv(m_modelMatrixHandle, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        f->glUniformMatrix4fv(m_viewMatrixHandle, 1, GL_FALSE, glm::value_ptr(m_camera.getMatrix()));
        f->glUniformMatrix4fv(m_projMatrixHandle, 1, GL_FALSE, glm::value_ptr(m_projMatrix));

        f->glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    f->glBindTexture(GL_TEXTURE_2D, 0);
    f->glBindVertexArray(0);
    m_Program->release();
}

void GLWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_W) {
        m_camera.move(CAMERA_MOVE::MOVE_FRONT);
    } else if (event->key() == Qt::Key_S) {
        m_camera.move(CAMERA_MOVE::MOVE_BACK);
    } else if (event->key() == Qt::Key_A) {
        m_camera.move(CAMERA_MOVE::MOVE_LEFT);
    } else if (event->key() == Qt::Key_D) {
        m_camera.move(CAMERA_MOVE::MOVE_RIGHT);
    }
    update(); // 触发渲染paintGL
}

void GLWindow::mouseMoveEvent(QMouseEvent *event) {
    m_camera.onMouseMove(event->pos().x(), event->pos().y());
    update(); // 触发渲染paintGL
}
