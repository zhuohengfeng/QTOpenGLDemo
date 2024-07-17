//
// Created by Rokid on 2024/7/17.
//

#ifndef QTOPENGLDEMO_SHADER_H
#define QTOPENGLDEMO_SHADER_H

#include "base.h"

class Shader final {

public:
    void createShader();

    void initShader(const QByteArray& vertexSource, const QByteArray& fragmentSource);

    void releaseShader();

    void start();

    void end();

    void setUniformMatrix(QOpenGLExtraFunctions* f, const char* _name , glm::mat4 _matrix)const;
    void setUniformVec3(QOpenGLExtraFunctions* f, const char* _name, glm::vec3 _vec3)const;
    void setUniformFloat(QOpenGLExtraFunctions* f, const char* _name, float _f)const;

private:
    QOpenGLShaderProgram* m_program;
};


#endif //QTOPENGLDEMO_SHADER_H
