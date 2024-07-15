#include "glwindow.h"

static const char* vertexShaderSource = R"(
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    layout (location = 2) in vec2 aUV;
    out vec4 outColor;
    out vec2 outUV;
    void main()
    {
       gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
       outColor = vec4(aColor , 1.0f);
       outUV = aUV;
    };
)";

static const char *fragmentShaderSource = R"(
    out vec4 FragColor;
    in vec4 outColor;
    in vec2 outUV;
    uniform sampler2D  ourTexture;
    void main()
    {
        FragColor = texture(ourTexture , outUV);
    };
)";


float vertices[] =
        {
                1.0f,  1.0f, 0.0f,    1.0f , 0.0f , 0.0f,   1.0f , 1.0f,
                1.0f, -1.0f, 0.0f,    0.0f , 1.0f , 0.0f,   1.0f , 0.0f,
                -1.0f,  -1.0f, 0.0f,  0.0f , 0.0f , 1.0f,   0.0f , 0.0f,
                -1.0f, 1.0f, 0.0f,    0.0f , 1.0f , 0.0f,   0.0f , 1.0f,
        };

unsigned int indices[] =
        {
                0 , 1 , 3 ,
                1 , 2 , 3
        };


GLWindow::GLWindow() {
}

GLWindow::~GLWindow() {
    makeCurrent();
    delete m_program;
}

void GLWindow::initializeGL() {
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

    m_Image = ffImage::readFromFile("../../resource/marker1.png");
    f->glGenTextures(1, &m_texture);
    f->glBindTexture(GL_TEXTURE_2D, m_texture);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Image->getWidth(), m_Image->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Image->getData());
    qDebug() << "Image w=" << m_Image->getWidth() << ", h=" << m_Image->getHeight();

    delete m_program;
    m_program = new QOpenGLShaderProgram;
    // Prepend the correct version directive to the sources. The rest is the
    // same, thanks to the common GLSL syntax.
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, versionedShaderCode(vertexShaderSource));
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, versionedShaderCode(fragmentShaderSource));
    m_program->link();

    // Create a VAO. Not strictly required for ES 3, but it is for plain OpenGL.
    f->glGenVertexArrays(1, &m_vao);
    f->glBindVertexArray(m_vao);

    f->glGenBuffers(1, &m_ebo);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    f->glGenBuffers(1, &m_vbo);
    f->glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    f->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                             reinterpret_cast<void *>(0));
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                             reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    f->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                             reinterpret_cast<void *>(6 * sizeof(GLfloat)));

    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glEnableVertexAttribArray(2);

    f->glBindVertexArray(0);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    f->glBindBuffer(GL_ARRAY_BUFFER, 0);


}

void GLWindow::resizeGL(int w, int h) {
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();
    qDebug() << "resizeGL w=" << w << ", h=" << h;
    f->glViewport(0, 0, w, h);
}

void GLWindow::paintGL() {
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

    f->glEnable(GL_DEPTH_TEST);
//    f->glEnable(GL_CULL_FACE);

    f->glClearColor(0, 0, 0, 1);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();

    f->glBindTexture(GL_TEXTURE_2D, m_texture);

    f->glBindVertexArray(m_vao);
    f->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    f->glBindVertexArray(0);

    m_program->release();
}
