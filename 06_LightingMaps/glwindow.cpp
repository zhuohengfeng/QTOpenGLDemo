//
// Created by Rokid on 2024/7/16.
//

#include "glwindow.h"
#include "shaderSrc.h"

GLWindow::GLWindow() {
    resize(WIDTH, HEIGHT);

    m_camera.lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_camera.setSpeed(0.05f);

    m_projMatrix = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

    light_pos = glm::vec3(0.0f, 1.0f, 5.0f);
    light_color = glm::vec3(1.0, 1.0f, 1.0f);
}

GLWindow::~GLWindow() {
    makeCurrent();
    m_cubeShader.releaseShader();
    m_sunShader.releaseShader();
}

void GLWindow::initializeGL() {
    QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();

    // 边框金属反光光照贴图
    m_boardImage = ffImage::readFromFile("../../resource/specular.png");
    f->glGenTextures(1, &m_boardTexture);
    f->glBindTexture(GL_TEXTURE_2D, m_boardTexture);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_boardImage->getWidth(), m_boardImage->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_boardImage->getData());
    f->glBindTexture(GL_TEXTURE_2D, 0);
    qDebug() << "m_boardImage w=" << m_boardImage->getWidth() << ", h=" << m_boardImage->getHeight();

    // 箱体的贴图绑定纹理
    m_boxImage = ffImage::readFromFile("../../resource/box.png");
    f->glGenTextures(1, &m_boxTexture);
    f->glBindTexture(GL_TEXTURE_2D, m_boxTexture);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_boxImage->getWidth(), m_boxImage->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_boxImage->getData());
    f->glBindTexture(GL_TEXTURE_2D, 0);
    qDebug() << "m_boxImage w=" << m_boxImage->getWidth() << ", h=" << m_boxImage->getHeight();

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

    // 创建shader Program
    m_cubeShader.createShader();
    m_cubeShader.initShader(cubeVertexShaderStr, cubeFragmentShaderStr);
    m_sunShader.createShader();
    m_sunShader.initShader(sunVertexShaderStr, sunFragmentShaderStr);
}

void GLWindow::resizeGL(int w, int h) {
    QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();
    f->glViewport(0, 0, w, h);
}

void GLWindow::paintGL() {
    QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();

    f->glEnable(GL_DEPTH_TEST);
    f->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_camera.update();
    glm::mat4 _modelMatrix(1.0f);
    _modelMatrix = glm::translate(_modelMatrix, glm::vec3(0.0f, 0.0f, -3.0f));

    // ---------------Draw Call start---------------


    // ---------------绘制有光照效果的立方体---------------
    m_cubeShader.start();
    m_cubeShader.setUniformVec3(f, "view_pos", m_camera.getPosition());

    //传入光照属性
//    light_color = glm::vec3((float)timer.elapsed() * 0.8f, (float)timer.elapsed() * 0.5f, (float)timer.elapsed() * 0.7f);
    m_cubeShader.setUniformVec3(f, "myLight.m_ambient" , light_color * glm::vec3(0.1f));
    m_cubeShader.setUniformVec3(f, "myLight.m_diffuse", light_color * glm::vec3(0.7f)); // 0.7f
    m_cubeShader.setUniformVec3(f, "myLight.m_specular", light_color * glm::vec3(1.0f)); // 0.5f
    m_cubeShader.setUniformVec3(f, "myLight.m_pos", light_pos);

    //传入物体材质属性, 这里不指定物体材质的环境光和漫反射的颜色
    m_cubeShader.setUniformInt(f, "myMaterial.m_diffuse", 0); // 环境光和漫反射使用光照贴图texture0
    m_cubeShader.setUniformInt(f, "myMaterial.m_specular", 1); // 使用光照贴图texture1
    m_cubeShader.setUniformFloat(f, "myMaterial.m_shiness" , 256); // 32

    m_cubeShader.setUniformMatrix(f, "_modelMatrix", _modelMatrix);
    m_cubeShader.setUniformMatrix(f, "_viewMatrix", m_camera.getMatrix());
    m_cubeShader.setUniformMatrix(f, "_projMatrix", m_projMatrix);
    // 绑定纹理
    f->glBindVertexArray(m_vao);
    f->glActiveTexture(GL_TEXTURE0);
    f->glBindTexture(GL_TEXTURE_2D, m_boxTexture);
    f->glActiveTexture(GL_TEXTURE1);
    f->glBindTexture(GL_TEXTURE_2D, m_boardTexture);
    f->glDrawArrays(GL_TRIANGLES, 0, 36);
    f->glBindTexture(GL_TEXTURE_2D, 0);
    f->glBindVertexArray(0);

    m_cubeShader.end();

    // --------------绘制一个太阳-------------------
    m_sunShader.start();
    _modelMatrix = glm::mat4(1.0f);
    _modelMatrix = glm::translate(_modelMatrix, light_pos);   // 设置光源(太阳位置)
    _modelMatrix = glm::scale(_modelMatrix, glm::vec3(0.2f)); // 缩放太阳大小
    m_sunShader.setUniformMatrix(f, "_modelMatrix", _modelMatrix);
    m_sunShader.setUniformMatrix(f, "_viewMatrix", m_camera.getMatrix());
    m_sunShader.setUniformMatrix(f, "_projMatrix", m_projMatrix);

    f->glBindVertexArray(m_vao);
    f->glDrawArrays(GL_TRIANGLES, 0, 36);
    f->glBindVertexArray(0);

    m_sunShader.end();
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
