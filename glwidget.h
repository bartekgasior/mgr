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

#define ASF_TEMPLATE_PATH "ETC/modelTemplate.asf"
#define AMC_TEMPLATE_PATH "ETC/modelTemplate.amc"
#define DAT_TEMPLATE_PATH "ETC/modelTemplate.dat"

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
	void rotate(string boneName, float direction, pf::Vec3 vect, int rotVal);

	/*funkcja zwraca id wybranej kosci w wektorze modelState*/
	int getModelStateID(pf::Model3D *model, string boneName, pf::Vec3 vect);

	/*funkcja aktualizuje limity obrotu wybranych kosci*/
	void setLimitsVector(vector<pf::range2> &limits, vector<pf::boneConfig> bones);

	/*zapis rotacji do mapy bonesRotations*/
	void saveModelStateToMap(map<string, pf::Vec3f> &bonesRotations, vector<vector<float>> mState, vector<pf::boneConfig> bonesConfig);

	/*aktualizacja wektora modelState po zmianie ilosci kosci modelu*/
	void updateModelStateFromMap(vector<vector<float>> &mState, map<string, pf::Vec3f> bonesRotations, vector<pf::boneConfig> bonesConfig);

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
	//rotacje
	vector<pf::Matrixf> rotations; 
	//nazwy wykorzystanych kosci
	vector<string> usedBones;
	//nazwy wszystkich kosci
	vector<string> allBones;
	//stany modelu
	vector<vector<float>> modelState;
	//kosci
	vector<pf::ASFBone> asfBones;
	map<string, int> idxBonesMap;
	//
	vector<pf::boneConfig> bonesConf;
	vector<pf::boneGeometry> bonesGeometry;

	/*obroty wszystkich kosci wraz z nazwa*/
	map<string, pf::Vec3f> bonesRotations;

private:
	void initializeBonesRotationsMap();

};

#endif // GLWIDGET_H
