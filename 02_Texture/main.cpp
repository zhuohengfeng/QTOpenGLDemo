#include <QApplication>
#include "glwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    GLWindow glWindow;
    glWindow.resize(800, 600);
    glWindow.show();

    return QApplication::exec();
}
