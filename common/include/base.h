#ifndef QTOPENGLDEMO_BASE_H
#define QTOPENGLDEMO_BASE_H

#include <QOpenGLTexture>
#include <QOpenGLContext>
#include <QDebug>

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

#endif //QTOPENGLDEMO_BASE_H
