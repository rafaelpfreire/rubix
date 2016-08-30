#include "mainwindow.h"
#include <QApplication>
#include <QSurface>

int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    format.setVersion(3, 3);

#if __APPLE__
    format.setProfile(QSurfaceFormat::CoreProfile);
#else
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
#endif
    // depth of the buffer size: to better depth test
    format.setDepthBufferSize(32);
    // antialiasing sampling
    format.setSamples(4);

    QSurfaceFormat::setDefaultFormat(format);

    QApplication a(argc, argv);
    MainWindow w;

    return a.exec();
}
