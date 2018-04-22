#ifndef GLWIDGET_H
#define GLWIDGET_H
#include "bone.h"
#include "asf.h"
#include "Model3D.h"


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <QOpenGLWidget>
#include <QGLWidget>

#include <QMessageBox>
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
    void resizeGL(int width, int height);
	void drawBackgroud(string img);
	/*BG, funkcja nie uzywana*/
    void drawBone(Bone *bone, cv::Point3f);
	/*BG,funkcja nie uzywana*/
    void drawSkeleton();
	//obrot modelu
	void rotate(string boneName, float direction, pf::Vec3 vect);
	int getModelStateID(pf::Model3D *model, string boneName, pf::Vec3 vect);

    QVector<Bone> skeletonBones;
    cv::Point3f p;
    int yRot, w, h;
	GLfloat ar;
	//sciezka do obrazu tla
	string imgPath;
	// Human 3D Model
	pf::Model3D* model;
	

	// draw skeleton model 
	void drawSkeletonModel(vector<vector<pf::Vec3f> > modelVertices);
	// vector zawieraj¹cy limity rotacji poszczególnych koœci
	vector<pf::range2> limits;
	vector<pf::Matrixf> rotations;
	vector<string> usedBones;
	vector<vector<float>> modelState;

//public slots:
	//void rotate(string boneName, float direction, pf::Vec3 vect);
};

#endif // GLWIDGET_H
