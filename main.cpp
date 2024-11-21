#include <QApplication>
#include "initialscreen.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    InitialScreen window;
    window.show();

    return app.exec();
}
