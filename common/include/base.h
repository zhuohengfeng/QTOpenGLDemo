#ifndef QTOPENGLDEMO_BASE_H
#define QTOPENGLDEMO_BASE_H

#include <iostream>
#include <string>

#include <QOpenGLTexture>
#include <QOpenGLContext>
#include <QDebug>
#include <QOpenGLWindow>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>
#include <QElapsedTimer>
#include <QKeyEvent>
#include <QMouseEvent>

#include "ffImage.h"
#include "camera.h"

#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include"glm/gtc/type_ptr.hpp"

/**
 * Get the shader string
 * @param src
 * @return
 */
static QByteArray versionedShaderCode(const char *src)
{
    QByteArray versionedSrc;
    if (QOpenGLContext::currentContext()->isOpenGLES())
        versionedSrc.append(QByteArrayLiteral("#version 300 es\n"));
    else
        versionedSrc.append(QByteArrayLiteral("#version 330\n"));
    versionedSrc.append(src);
    return versionedSrc;
}

static QMatrix4x4 glmMatToQMat(glm::mat4 glmMat) {
//    return QMatrix4x4(
//            glmMat[0][0], glmMat[1][0], glmMat[2][0], glmMat[3][0],
//            glmMat[0][1], glmMat[1][1], glmMat[2][1], glmMat[3][1],
//            glmMat[0][2], glmMat[1][2], glmMat[2][2], glmMat[3][2],
//            glmMat[0][3], glmMat[1][3], glmMat[2][3], glmMat[3][3]
//    );

    return QMatrix4x4(
            glmMat[0][0], glmMat[0][1], glmMat[0][2], glmMat[0][3],
            glmMat[1][0], glmMat[1][1], glmMat[1][2], glmMat[2][3],
            glmMat[2][0], glmMat[2][1], glmMat[2][2], glmMat[2][3],
            glmMat[3][0], glmMat[3][1], glmMat[3][2], glmMat[3][3]
    );
}

#endif //QTOPENGLDEMO_BASE_H
