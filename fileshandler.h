#ifndef FILESHANDLER_H
#define FILESHANDLER_H

#include <QObject>
#include <QMainWindow>
#include <QFileInfoList>

class FilesHandler : public QWidget
{
    Q_OBJECT

public:
    FilesHandler();
    QFileInfoList list;

signals:
    void lineWritten();
    void pathSelected();

public slots:
    void writeLine();
    void loadFiles();
};


#endif // FILESHANDLER_H
