#ifndef GLWIDGET_H
#define GLWIDGET_H
#include "bone.h"
#include "asf.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <QOpenGLWidget>
#include <iostream>
#include <fstream>
#include <cstring>

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void drawBone(Bone *bone, cv::Point3f);
    void drawSkeleton();

    QVector<Bone> skeletonBones;
    cv::Point3f p;
    int yRot;

public slots:
    void setRootXPosition(int posX);
    //void setRootYPosition(int posY);

signals:
    void rootXPositionChanged(double posX);
    //void rootYPositionChanged(int posY);

};

#endif // GLWIDGET_H
