#ifndef OPENGLOPERATIONS_H
#define OPENGLOPERATIONS_H

#include <QObject>
#include <QGLWidget>

class openGLOperations: public QGLWidget
{
    Q_OBJECT
public:
    openGLOperations();

    void initializeGL();
};

#endif // OPENGLOPERATIONS_H
