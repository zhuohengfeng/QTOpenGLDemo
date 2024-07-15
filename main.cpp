#include <QApplication>
#include "glwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    GLWindow glWindow;
    glWindow.resize(600, 400);
    glWindow.show();

    return QApplication::exec();
}
