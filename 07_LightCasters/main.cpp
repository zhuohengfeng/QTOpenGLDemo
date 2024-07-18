//
// Created by Rokid on 2024/7/16.
//

#include <QApplication>
#include "glwindow.h"

int main(int argc, char* argv[]) {
    QApplication application(argc, argv);
    GLWindow glWindow;
    glWindow.show();
    return QApplication::exec();
}