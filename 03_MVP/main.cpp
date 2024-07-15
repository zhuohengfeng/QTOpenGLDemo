#include <QApplication>
#include "glwindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    GLWindow glWindow;
    glWindow.show();

    return app.exec();
}

