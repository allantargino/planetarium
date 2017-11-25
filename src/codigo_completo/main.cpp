#include <QApplication>
#include <QSurfaceFormat>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QSurfaceFormat format;

#ifdef Q_OS_MAC
    format.setVersion(4, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);
#endif

    format.setDepthBufferSize(24);
    format.setSamples(4);
    QSurfaceFormat::setDefaultFormat(format);
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
