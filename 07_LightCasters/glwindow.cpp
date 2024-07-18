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
    _shader_cube.releaseShader();
    _shader_sun.releaseShader();
    _shader_dir.releaseShader();
    _shader_point.releaseShader();
    _shader_spot.releaseShader();
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
    _shader_cube.createShader();
    _shader_cube.initShader("../../07_LightCasters/shader/vertexShader.glsl", "../../07_LightCasters/shader/fragmentShader.glsl");

    _shader_sun.createShader();
    _shader_sun.initShader("../../07_LightCasters/shader/vsunShader.glsl", "../../07_LightCasters/shader/fsunShader.glsl");

    _shader_dir.createShader();
    _shader_dir.initShader("../../07_LightCasters/shader/dirShaderv.glsl", "../../07_LightCasters/shader/dirShaderf.glsl");

    _shader_point.createShader();
    _shader_point.initShader("../../07_LightCasters/shader/pointShaderv.glsl", "../../07_LightCasters/shader/pointShaderf.glsl");

    _shader_spot.createShader();
    _shader_spot.initShader("../../07_LightCasters/shader/spotShaderv.glsl", "../../07_LightCasters/shader/spotShaderf.glsl");
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
    _shader_spot.start();
    _shader_spot.setUniformVec3(f, "view_pos", m_camera.getPosition());

    //传入光照属性
//    light_color = glm::vec3((float)timer.elapsed() * 0.8f, (float)timer.elapsed() * 0.5f, (float)timer.elapsed() * 0.7f);
    _shader_spot.setUniformVec3(f, "myLight.m_ambient" , light_color * glm::vec3(0.1f));
    _shader_spot.setUniformVec3(f, "myLight.m_diffuse", light_color * glm::vec3(0.9f)); // 0.7f
    _shader_spot.setUniformVec3(f, "myLight.m_specular", light_color * glm::vec3(0.9f)); // 0.5f

    _shader_spot.setUniformVec3(f,"myLight.m_pos", m_camera.getPosition());
    _shader_spot.setUniformVec3(f,"myLight.m_direction", m_camera.getDirection());
    _shader_spot.setUniformFloat(f,"myLight.m_cutOff", glm::cos(glm::radians(12.5f)));
    _shader_spot.setUniformFloat(f,"myLight.m_outCutOff", glm::cos(glm::radians(20.5f)));

    _shader_spot.setUniformFloat(f,"myLight.m_c", 1.0f);
    _shader_spot.setUniformFloat(f,"myLight.m_l", 0.07f);
    _shader_spot.setUniformFloat(f,"myLight.m_q", 0.017f);


    //传入物体材质属性, 这里不指定物体材质的环境光和漫反射的颜色
    _shader_spot.setUniformInt(f, "myMaterial.m_diffuse", 0); // 环境光和漫反射使用光照贴图texture0
    _shader_spot.setUniformInt(f, "myMaterial.m_specular", 1); // 使用光照贴图texture1
    _shader_spot.setUniformFloat(f, "myMaterial.m_shiness" , 32); // 32

    _shader_spot.setUniformMatrix(f, "_viewMatrix", m_camera.getMatrix());
    _shader_spot.setUniformMatrix(f, "_projMatrix", m_projMatrix);
    // 绑定纹理
    f->glBindVertexArray(m_vao);
    f->glActiveTexture(GL_TEXTURE0);
    f->glBindTexture(GL_TEXTURE_2D, m_boxTexture);
    f->glActiveTexture(GL_TEXTURE1);
    f->glBindTexture(GL_TEXTURE_2D, m_boardTexture);

    for (int i = 0; i < 10; i++)
    {
        _modelMatrix = glm::mat4(1.0f);
        _modelMatrix = glm::translate(_modelMatrix, cubePositions[i]);
        _modelMatrix = glm::rotate(_modelMatrix, glm::radians(i * 20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        _shader_spot.setUniformMatrix(f, "_modelMatrix", _modelMatrix);
        f->glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    f->glBindTexture(GL_TEXTURE_2D, 0);
    f->glBindVertexArray(0);
    _shader_spot.end();

    // --------------绘制一个太阳-------------------
    _shader_sun.start();
    _modelMatrix = glm::mat4(1.0f);
    _modelMatrix = glm::translate(_modelMatrix, light_pos);   // 设置光源(太阳位置)
    _modelMatrix = glm::scale(_modelMatrix, glm::vec3(0.2f)); // 缩放太阳大小
    _shader_sun.setUniformMatrix(f, "_modelMatrix", _modelMatrix);
    _shader_sun.setUniformMatrix(f, "_viewMatrix", m_camera.getMatrix());
    _shader_sun.setUniformMatrix(f, "_projMatrix", m_projMatrix);

    f->glBindVertexArray(m_vao);
    f->glDrawArrays(GL_TRIANGLES, 0, 36);
    f->glBindVertexArray(0);

    _shader_sun.end();
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
