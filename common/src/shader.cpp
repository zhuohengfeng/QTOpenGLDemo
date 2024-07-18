//
// Created by Rokid on 2024/7/17.
//

#include "shader.h"

void Shader::createShader() {
    m_program = new QOpenGLShaderProgram();
}

void Shader::initShader(const QByteArray& vertexSource, const QByteArray& fragmentSource) {
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, versionedShaderCode(vertexSource));
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, versionedShaderCode(fragmentSource));
    m_program->link();
}

void Shader::initShader(const char* vertexFile, const char* fragmentFile) {
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, readShaderFile(vertexFile));
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, readShaderFile(fragmentFile));
    m_program->link();
}

void Shader::releaseShader() {
    delete m_program;
}

void Shader::start() {
    m_program->bind();
}

void Shader::end() {
    m_program->release();
}

void Shader::setUniformMatrix(QOpenGLExtraFunctions* f, const char* _name, glm::mat4 _matrix) const {
    f->glUniformMatrix4fv(m_program->uniformLocation(_name), 1 , GL_FALSE , glm::value_ptr(_matrix));
}

void Shader::setUniformVec3(QOpenGLExtraFunctions* f, const char* _name, glm::vec3 _vec3) const {
    f->glUniform3fv(m_program->uniformLocation(_name), 1, glm::value_ptr(_vec3));
}

void Shader::setUniformFloat(QOpenGLExtraFunctions* f, const char* _name, float _f) const {
    f->glUniform1f(m_program->uniformLocation(_name), _f);
}

void Shader::setUniformInt(QOpenGLExtraFunctions* f, const char* _name, int _i) const {
    f->glUniform1i(m_program->uniformLocation(_name), _i);
}
