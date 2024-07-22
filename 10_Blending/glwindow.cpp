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
    GLuint texture[] {m_boxTexture, m_windowTexture};
    f->glDeleteTextures(2, texture);
    GLuint vao[] {m_cube_vao, m_plane_vao, m_window_vao};
    f->glDeleteVertexArrays(3, vao);
}

GLuint createTexture(QOpenGLExtraFunctions* f, const char* imageFile) {
    GLuint texture;
    ffImage* image = ffImage::readFromFile(imageFile);
    f->glGenTextures(1, &texture);
    f->glBindTexture(GL_TEXTURE_2D, texture);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->getWidth(), image->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->getData());
    f->glBindTexture(GL_TEXTURE_2D, 0);
    qDebug() << "m_windowTexture w=" << image->getWidth() << ", h=" << image->getHeight();
    delete image;
    return texture;
}


GLuint createCubeVAO(QOpenGLExtraFunctions* f) {
    GLuint vao;
    GLuint vbo;
    // 绑定VAO VBO
    f->glGenVertexArrays(1, &vao);
    f->glBindVertexArray(vao);

    f->glGenBuffers(1, &vbo);
    f->glBindBuffer(GL_ARRAY_BUFFER, vbo);
    f->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void *>(0));
    f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void *>(3*sizeof(GLfloat)));
    f->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void *>(5*sizeof(GLfloat)));
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glEnableVertexAttribArray(2);

    f->glBindVertexArray(0);
    f->glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vao;
}

GLuint createPlaneVAO(QOpenGLExtraFunctions* f) {
    float planeVertices[] = {
            5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
            -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
            -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

            5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
            -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
            5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };

    GLuint vao;
    GLuint vbo;
    // 绑定VAO VBO
    f->glGenVertexArrays(1, &vao);
    f->glBindVertexArray(vao);

    f->glGenBuffers(1, &vbo);
    f->glBindBuffer(GL_ARRAY_BUFFER, vbo);
    f->glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void *>(0));
    f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void *>(3*sizeof(GLfloat)));
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);

    f->glBindVertexArray(0);
    f->glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vao;
}

GLuint createWindowVAO(QOpenGLExtraFunctions* f) {
    float transparentVertices[] = {
            0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
            0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
            1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

            0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
            1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
            1.0f,  0.5f,  0.0f,  1.0f,  0.0f
    };

    GLuint vao;
    GLuint vbo;
    // 绑定VAO VBO
    f->glGenVertexArrays(1, &vao);
    f->glBindVertexArray(vao);

    f->glGenBuffers(1, &vbo);
    f->glBindBuffer(GL_ARRAY_BUFFER, vbo);
    f->glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);

    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void *>(0));
    f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void *>(3*sizeof(GLfloat)));
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);

    f->glBindVertexArray(0);
    f->glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vao;
}

void GLWindow::initializeGL() {
    QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();

    // 创建shader Program
    m_shader.createShader();
    m_shader.initShader("../../10_Blending/shader/vertexShader.glsl", "../../10_Blending/shader/fragmentShader.glsl");

    // 边框金属反光光照贴图
    m_windowTexture = createTexture(f, "../../resource/blend_window.png");

    // 箱体的贴图绑定纹理
    m_boxTexture = createTexture(f, "../../resource/box.png");

    m_cube_vao = createCubeVAO(f);
    m_plane_vao = createPlaneVAO(f);
    m_window_vao = createWindowVAO(f);
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

    //数据
    std::vector<glm::vec3> _window_pos
    {
            glm::vec3(-1.5f, 0.0f, -0.48f),
            glm::vec3(1.5f, 0.0f, 0.51f),
            glm::vec3(0.0f, 0.0f, 0.7f),
            glm::vec3(-0.3f, 0.0f, -2.3f),
            glm::vec3(0.5f, 0.0f, -0.6f)
    };

    m_camera.update();
    m_projMatrix = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
    glm::mat4 _modelMatrix(1.0f);
    _modelMatrix = glm::translate(_modelMatrix, glm::vec3(0.0f, 0.0f, -3.0f));

    // 先绘制不透明物体，因为透明物体也会影响深度值
    // 透明物体也要进行排序
    // map会根据key自动进行排序
    std::map<float, glm::vec3> _window_sort;
    for (int i = 0; i < _window_pos.size(); i++)
    {
        float _dist = glm::length(m_camera.getPosition() - _window_pos[i]);
        _window_sort[_dist] = _window_pos[i];
    }

    f->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    f->glEnable(GL_DEPTH_TEST);

    // 设置混合模式
    f->glEnable(GL_BLEND);
    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // ---------------Draw Call start---------------
    f->glActiveTexture(GL_TEXTURE0);
    f->glBindTexture(GL_TEXTURE_2D, m_boxTexture);

    m_shader.start();
    m_shader.setUniformMatrix(f, "_modelMatrix", _modelMatrix);
    m_shader.setUniformMatrix(f, "_viewMatrix", m_camera.getMatrix());
    m_shader.setUniformMatrix(f, "_projMatrix", m_projMatrix);

    //绘制地面
    f->glBindVertexArray(m_plane_vao);
    f->glDrawArrays(GL_TRIANGLES, 0, 6);

    //绘制方盒
    f->glBindVertexArray(m_cube_vao);
    f->glDrawArrays(GL_TRIANGLES, 0, 36);

    //绘制窗体
    for (std::map<float, glm::vec3>::reverse_iterator _it = _window_sort.rbegin();_it != _window_sort.rend();_it++)
    {
        _modelMatrix = glm::mat4(1.0f);
        _modelMatrix = glm::translate(_modelMatrix, _it->second);
        m_shader.setUniformMatrix(f, "_modelMatrix", _modelMatrix);
        f->glBindTexture(GL_TEXTURE_2D, m_windowTexture);
        f->glBindVertexArray(m_window_vao);
        f->glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    // ---------------Draw Call end---------------

    m_shader.end();
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
