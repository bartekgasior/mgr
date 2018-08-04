#include "mainwindow.h"
#include "fileshandler.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    //qputenv("QT_DEBUG_PLUGINS", QByteArray("1"));
    //QApplication::addLibraryPath("C:/Qt/5.9.1/mingw53_32/plugins/imageformats");
    QApplication a(argc, argv);
	QDesktopWidget dw;
    MainWindow w;

	//int x = dw.primaryScreen.width()*0.9;
	//int y = dw.primaryScreen.height()*0.9;
	w.setFixedSize(1366, 768);

    w.show();

    return a.exec();
}