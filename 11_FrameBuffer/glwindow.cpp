//
// Created by Rokid on 2024/7/16.
//

#include "glwindow.h"

GLWindow::GLWindow() {
    resize(WIDTH, HEIGHT);

    m_camera.lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_camera.setSpeed(0.05f);

    m_projMatrix = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

    light_pos = glm::vec3(8.0f, 8.0f, 5.0f);
    light_color = glm::vec3(1.0, 1.0f, 1.0f);
}

GLWindow::~GLWindow() {
    makeCurrent();
    QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();
    m_shaderBox.releaseShader();
    m_shaderBlur.releaseShader();
    GLuint texture[] {m_boxTexture};
    f->glDeleteTextures(1, texture);
    GLuint vao[] {m_vao};
    f->glDeleteVertexArrays(1, vao);
}

void GLWindow::initializeGL() {
    QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();

    // 箱体的贴图绑定纹理
    ffImage* image = ffImage::readFromFile("../../resource/box.png");
    f->glGenTextures(1, &m_boxTexture);
    f->glBindTexture(GL_TEXTURE_2D, m_boxTexture);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->getWidth(), image->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->getData());
    f->glBindTexture(GL_TEXTURE_2D, 0);
    qDebug() << "m_boxImage w=" << image->getWidth() << ", h=" << image->getHeight();
    delete image;

    createFrameBuffer();

    // 绑定VAO VBO
    f->glGenVertexArrays(1, &m_vao);
    f->glBindVertexArray(m_vao);

    f->glGenBuffers(1, &m_vbo);
    f->glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    f->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void *>(0));
    f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void *>(3*sizeof(GLfloat)));
    f->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void *>(5*sizeof(GLfloat)));
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glEnableVertexAttribArray(2);

    f->glBindVertexArray(0);
    f->glBindBuffer(GL_ARRAY_BUFFER, 0);


    // 绑定屏幕VAO VBO
    f->glGenVertexArrays(1, &m_blurvao);
    f->glBindVertexArray(m_blurvao);

    f->glGenBuffers(1, &m_blurvao);
    f->glBindBuffer(GL_ARRAY_BUFFER, m_blurvao);
    f->glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    f->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), reinterpret_cast<void *>(0));
    f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), reinterpret_cast<void *>(2*sizeof(GLfloat)));
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);

    f->glBindVertexArray(0);
    f->glBindBuffer(GL_ARRAY_BUFFER, 0);


    // 创建shader Program
    m_shaderBox.createShader();
    m_shaderBox.initShader("../../11_FrameBuffer/shader/vertexBox.glsl", "../../11_FrameBuffer/shader/fragmentBox.glsl");

    m_shaderBlur.createShader();
    m_shaderBlur.initShader("../../11_FrameBuffer/shader/vertexBlur.glsl", "../../11_FrameBuffer/shader/fragmentBlur.glsl");
}

void GLWindow::resizeGL(int w, int h) {
    QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();
    f->glViewport(0, 0, w, h);
}

void GLWindow::createFrameBuffer() {
    QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();

    f->glGenFramebuffers(1, &m_fbo);
    f->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    // color buffer
    f->glGenTextures(1, &m_fboTextColorBuffer);
    f->glBindTexture(GL_TEXTURE_2D, m_fboTextColorBuffer);
    f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fboTextColorBuffer, 0);

    // depth/sential buffer
    f->glGenRenderbuffers(1, &m_rbo);
    f->glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    f->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
    f->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

    f->glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void GLWindow::paintGL() {
    QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();


    f->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    f->glEnable(GL_DEPTH_TEST);
    f->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_camera.update();

    // ---------------绘制有光照效果的立方体---------------
    m_shaderBox.start();

    //传入物体材质属性, 这里不指定物体材质的环境光和漫反射的颜色
    m_shaderBox.setUniformMatrix(f, "_viewMatrix", m_camera.getMatrix());
    m_shaderBox.setUniformMatrix(f, "_projMatrix", m_projMatrix);

    // 绑定纹理
    f->glActiveTexture(GL_TEXTURE0);
    f->glBindTexture(GL_TEXTURE_2D, m_boxTexture);

    // 绑定VAO
    f->glBindVertexArray(m_vao);
    glm::mat4 _modelMatrix(1.0f);
    _modelMatrix = glm::mat4(1.0f);
    _modelMatrix = glm::rotate(_modelMatrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_shaderBox.setUniformMatrix(f, "_modelMatrix", _modelMatrix);

    f->glDrawArrays(GL_TRIANGLES, 0, 36);

    f->glBindTexture(GL_TEXTURE_2D, 0);
    f->glBindVertexArray(0);
    m_shaderBox.end();

    f->glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // ---------------Draw Call end---------------
    f->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shaderBlur.start();
    f->glBindVertexArray(m_blurvao);
    f->glBindTexture(GL_TEXTURE_2D, m_fboTextColorBuffer);
    f->glDrawArrays(GL_TRIANGLES, 0, 6);

    f->glBindVertexArray(0);
    m_shaderBlur.end();
    // ---------------Draw Call end---------------

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
