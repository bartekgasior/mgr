#ifndef OPENGLOPERATIONS_H
#define OPENGLOPERATIONS_H

#include <QOpenGLWidget>

class OpenGLOperations: public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit OpenGLOperations(QWidget *parent = 0);
    void initializeGL();
    void paintGL();
    void draw();
};

#endif // OPENGLOPERATIONS_H
