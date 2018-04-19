#include "mainwindow.h"
#include "fileshandler.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //qputenv("QT_DEBUG_PLUGINS", QByteArray("1"));
    //QApplication::addLibraryPath("C:/Qt/5.9.1/mingw53_32/plugins/imageformats");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}