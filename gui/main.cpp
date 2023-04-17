#include "picamera.h"
#include <QApplication>
#include <QtWidgets/QApplication>
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    camerashow1 w;
    w.show();
    return a.exec();
}

