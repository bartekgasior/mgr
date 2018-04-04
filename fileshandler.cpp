#include "fileshandler.h"
#include "mainwindow.h"
#include <stdio.h>

#include <QFileDialog>

FilesHandler::FilesHandler()
{

}

void FilesHandler::writeLine(){
    printf("Hello");
    emit this->lineWritten();
}

void FilesHandler::loadFiles(){
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Img folder"));
    if(!folderPath.isEmpty()){
        QDir dir(folderPath);
        QStringList filter;
        filter << QLatin1String("*.png");
        filter << QLatin1String("*.jpg");
        dir.setNameFilters(filter);
        list = dir.entryInfoList();

        emit this->pathSelected();
    }
}
