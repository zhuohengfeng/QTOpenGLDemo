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
    m_shader.releaseShader();
    m_boardShader.releaseShader();
    GLuint texture[] {m_boardTexture, m_boxTexture};
    f->glDeleteTextures(2, texture);
    GLuint vao[] {m_vao};
    f->glDeleteVertexArrays(1, vao);
}

void GLWindow::initializeGL() {
    QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();

    // 边框金属反光光照贴图
    ffImage* image = ffImage::readFromFile("../../resource/specular.png");
    f->glGenTextures(1, &m_boardTexture);
    f->glBindTexture(GL_TEXTURE_2D, m_boardTexture);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->getWidth(), image->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->getData());
    f->glBindTexture(GL_TEXTURE_2D, 0);
    qDebug() << "m_boardImage w=" << image->getWidth() << ", h=" << image->getHeight();
    delete image;

    // 箱体的贴图绑定纹理
    image = ffImage::readFromFile("../../resource/box.png");
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
    m_shader.createShader();
    m_shader.initShader("../../09_StencilTesting/shader/vertexShader.glsl", "../../09_StencilTesting/shader/fragmentShader.glsl");

    m_boardShader.createShader();
    m_boardShader.initShader("../../09_StencilTesting/shader/vertexBoard.glsl", "../../09_StencilTesting/shader/fragmentBoard.glsl");
}

void GLWindow::resizeGL(int w, int h) {
    QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();
    f->glViewport(0, 0, w, h);
}

void GLWindow::paintGL() {
    QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();

    f->glEnable(GL_DEPTH_TEST);
    f->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_camera.update();


    // ---------------Draw Call start---------------


    // ---------------绘制有光照效果的立方体---------------
    m_shader.start();
    m_shader.setUniformVec3(f, "view_pos", m_camera.getPosition());

    //传入物体材质属性, 这里不指定物体材质的环境光和漫反射的颜色
    m_shader.setUniformInt(f, "myMaterial.m_diffuse", 0); // 环境光和漫反射使用光照贴图texture0
    m_shader.setUniformInt(f, "myMaterial.m_specular", 1); // 使用光照贴图texture1
    m_shader.setUniformFloat(f, "myMaterial.m_shiness" , 32); // 32

    m_shader.setUniformMatrix(f, "_viewMatrix", m_camera.getMatrix());
    m_shader.setUniformMatrix(f, "_projMatrix", m_projMatrix);

    //传入光照属性
    // directional light
    m_shader.setUniformVec3(f, "_dirLight.m_direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    m_shader.setUniformVec3(f, "_dirLight.m_ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    m_shader.setUniformVec3(f, "_dirLight.m_diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
    m_shader.setUniformVec3(f, "_dirLight.m_specular", glm::vec3(0.5f, 0.5f, 0.5f));
    // point light 1
    m_shader.setUniformVec3(f, "_pointLight[0].m_pos", pointLightPositions[0]);
    m_shader.setUniformVec3(f, "_pointLight[0].m_ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    m_shader.setUniformVec3(f, "_pointLight[0].m_diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    m_shader.setUniformVec3(f, "_pointLight[0].m_specular", glm::vec3(1.0f, 1.0f, 1.0f));
    m_shader.setUniformFloat(f, "_pointLight[0].m_c", 1.0f);
    m_shader.setUniformFloat(f, "_pointLight[0].m_l", 0.09);
    m_shader.setUniformFloat(f, "_pointLight[0].m_q", 0.032);
    // point light 2
    m_shader.setUniformVec3(f, "_pointLight[1].m_pos", pointLightPositions[1]);
    m_shader.setUniformVec3(f, "_pointLight[1].m_ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    m_shader.setUniformVec3(f, "_pointLight[1].m_diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    m_shader.setUniformVec3(f, "_pointLight[1].m_specular", glm::vec3(1.0f, 1.0f, 1.0f));
    m_shader.setUniformFloat(f, "_pointLight[1].m_c", 1.0f);
    m_shader.setUniformFloat(f, "_pointLight[1].m_l", 0.09);
    m_shader.setUniformFloat(f, "_pointLight[1].m_q", 0.032);
    // point light 3
    m_shader.setUniformVec3(f, "_pointLight[2].m_pos", pointLightPositions[2]);
    m_shader.setUniformVec3(f, "_pointLight[2].m_ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    m_shader.setUniformVec3(f, "_pointLight[2].m_diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    m_shader.setUniformVec3(f, "_pointLight[2].m_specular", glm::vec3(1.0f, 1.0f, 1.0f));
    m_shader.setUniformFloat(f, "_pointLight[2].m_c", 1.0f);
    m_shader.setUniformFloat(f, "_pointLight[2].m_l", 0.09);
    m_shader.setUniformFloat(f, "_pointLight[2].m_q", 0.032);
    // point light 4
    m_shader.setUniformVec3(f, "_pointLight[3].m_pos", pointLightPositions[3]);
    m_shader.setUniformVec3(f, "_pointLight[3].m_ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    m_shader.setUniformVec3(f, "_pointLight[3].m_diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    m_shader.setUniformVec3(f, "_pointLight[3].m_specular", glm::vec3(1.0f, 1.0f, 1.0f));
    m_shader.setUniformFloat(f, "_pointLight[3].m_c", 1.0f);
    m_shader.setUniformFloat(f, "_pointLight[3].m_l", 0.09);
    m_shader.setUniformFloat(f, "_pointLight[3].m_q", 0.032);
    // spotLight
    m_shader.setUniformVec3(f, "_spotLight.m_pos", m_camera.getPosition());
    m_shader.setUniformVec3(f, "_spotLight.m_direction", m_camera.getDirection());
    m_shader.setUniformVec3(f, "_spotLight.m_ambient", glm::vec3(0.0f, 0.0f, 0.0f));
    m_shader.setUniformVec3(f, "_spotLight.m_diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
    m_shader.setUniformVec3(f, "_spotLight.m_specular", glm::vec3(1.0f, 1.0f, 1.0f));
    m_shader.setUniformFloat(f, "_spotLight.m_c", 1.0f);
    m_shader.setUniformFloat(f, "_spotLight.m_l", 0.09);
    m_shader.setUniformFloat(f, "_spotLight.m_q", 0.032);
    m_shader.setUniformFloat(f, "_spotLight.m_cutOff", glm::cos(glm::radians(12.5f)));
    m_shader.setUniformFloat(f, "_spotLight.m_outCutOff", glm::cos(glm::radians(15.0f)));

    // 绑定纹理
    f->glActiveTexture(GL_TEXTURE0);
    f->glBindTexture(GL_TEXTURE_2D, m_boxTexture);
    f->glActiveTexture(GL_TEXTURE1);
    f->glBindTexture(GL_TEXTURE_2D, m_boardTexture);

    // 绑定VAO
    f->glBindVertexArray(m_vao);
    glm::mat4 _modelMatrix(1.0f);
    _modelMatrix = glm::mat4(1.0f);
    _modelMatrix = glm::rotate(_modelMatrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_shader.setUniformMatrix(f, "_modelMatrix", _modelMatrix);

    f->glEnable(GL_STENCIL_TEST);
    f->glStencilFunc(GL_ALWAYS, 0x1,  0xff);
    f->glStencilMask(0xff);
    f->glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//    f->glStencilOp();

    f->glDrawArrays(GL_TRIANGLES, 0, 36);

    f->glBindTexture(GL_TEXTURE_2D, 0);
    f->glBindVertexArray(0);
    m_shader.end();



    // ---------------Draw Call end---------------
    m_boardShader.start();
    m_boardShader.setUniformMatrix(f, "_viewMatrix", m_camera.getMatrix());
    m_boardShader.setUniformMatrix(f, "_projMatrix", m_projMatrix);
    f->glBindVertexArray(m_vao);
    _modelMatrix = glm::mat4(1.0f);
    _modelMatrix = glm::rotate(_modelMatrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    _modelMatrix = glm::scale(_modelMatrix, glm::vec3(1.2f));

    f->glStencilFunc(GL_NOTEQUAL, 0x1,  0xff);
    f->glStencilMask(0x00);

    m_boardShader.setUniformMatrix(f, "_modelMatrix", _modelMatrix);
    f->glDrawArrays(GL_TRIANGLES, 0, 36);

    f->glBindVertexArray(0);
    m_boardShader.end();
    // ---------------Draw Call end---------------
    f->glStencilMask(0xFF);
    f->glStencilFunc(GL_ALWAYS, 0, 0xFF);
    f->glEnable(GL_DEPTH_TEST);

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
