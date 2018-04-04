#ifndef FILESOPERATIONS_H
#define FILESOPERATIONS_H

#include <QObject>

class filesOperations : public QObject
{
    Q_OBJECT
public:

    explicit filesOperations(QObject *parent = nullptr);

signals:
    void pathSelected(bool);
public slots:
    void loadFiles();
};

#endif // FILESOPERATIONS_H
