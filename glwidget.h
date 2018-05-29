#ifndef GLWIDGET_H
#define GLWIDGET_H
#include "bone.h"
#include "asf.h"
#include "Model3D.h"


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <QOpenGLWidget>
#include <QGLWidget>
#include <QDir>
#include <QFileDialog>

#include <QMessageBox>
#include <iostream>
#include <fstream>
#include <cstring>

#include "avihzdialog.h"

#define ASF_TEMPLATE_PATH "ETC/modelTemplate.asf"
#define AMC_TEMPLATE_PATH "ETC/modelTemplate.amc"
#define DAT_TEMPLATE_PATH "ETC/modelTemplate.dat"

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);

	int mode = 1;

	QVector<Bone> skeletonBones;
	cv::Point3f p;
	int yRot, w, h;
	GLfloat ar;

	//sciezka do folderu ze zdjeciami
	QString *loadedImagesFolderPath;
	//sciezka do pliku avi
	QString aviFilePath;
	QVector<cv::Mat> aviFrames;

	/*pliki wybranego folderu*/
	QFileInfoList list;

	//sciezka do obrazu tla
	string imgPath;

	/* klatka z pliku avi */
	cv::Mat aviFrame;

	// Human 3D Model
	pf::Model3D* model;

	/*flaga sprawdzajaca czy obiekt znajduje sie w glownym oknie aplikacji*/
	bool isInMainWindow = true;

	/*czy wczytane tlo jest zdjeciem czy klatka pliku avi*/
	bool isAvi = false;

	/*czy dodac tlo*/
	bool drawBckg = false;
	
	/*czestotliwosc wczytywania klatek z avi - wykorzystywana do zapisu i odczytu z pliku*/
	int hz = 0;

	// vector zawieraj¹cy limity rotacji poszczególnych koœci
	vector<pf::range2> limits;

	vector<float> velocity;

	//rotacje
	vector<pf::Matrixf> rotations;

	//nazwy wykorzystanych kosci
	vector<string> usedBones;

	//nazwy wszystkich kosci
	vector<string> allBones;

	//stany modelu
	vector<vector<float>> modelState;

	vector<vector<float>> radiusVec;
	//kosci
	vector<pf::ASFBone> asfBones;
	map<string, int> idxBonesMap;

	//
	vector<pf::boneConfig> bonesConf;
	vector<pf::boneGeometry> bonesGeometry;

	/*obroty wszystkich kosci wraz z nazwa*/
	map<string, pf::Vec3f> bonesRotations;

	/*dlugosci kosci oraz ich nazwa*/
	map<string, float> bonesLength;

	/*przesuniecie modelu*/
	vector<float> modelTranslation;

	/*limity obrotow danej kosci
		np. LeftLeg 50, 50, 50, 50, 50, 50 -> odpowiednio limity w osi X, Y, Z
	*/
	map<string, vector<int>> modelLimits;

	/* velocity -> LeftLeg 0, 10, 15*/
	map<string, vector<float>> modelVelocity;

	/* dof -> np. LeftLeg true, true, false */
	map<string, vector<bool>> modelDOF;

	/*###############################################*/
	/* metody */

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

	/*tworzenie tla dla wczytanego folderu ze zdjeciami*/
	void drawBackgroud(string img);

	/* tworzenie tla dla wczytanego pliku .avi */
	void drawBackground(cv::Mat mat);
	/*BG, funkcja nie uzywana*/
    void drawBone(Bone *bone, cv::Point3f);

	/*BG,funkcja nie uzywana*/
    void drawSkeleton();

	//obrot modelu
	void rotate(string boneName, float direction, pf::Vec3 vect, int rotVal);

	/*zmiana dlugosci kosci*/
	void updateLength(string boneName, float direction, float value);

	/*przesuniecie modelu*/
	void translate(float direction, pf::Vec3 vect, int value);

	/*skalowanie wszystkich kosci modelu*/
	void scale(float direction, int value);

	/*pobranie dlugosci kosci*/
	float getBoneLength(string boneName);

	/*funkcja zwraca id wybranej kosci w wektorze modelState*/
	int getModelStateID(pf::Model3D *model, string boneName, pf::Vec3 vect);

	/*funkcja aktualizuje limity obrotow wybranych kosci*/
	void setLimitsVector(vector<pf::range2> &limits, vector<pf::boneConfig> bones, vector<pf::boneGeometry> bonesGeometry);

	void setVelocityVector(vector<float> &velocity, vector<pf::boneConfig> bones, vector<pf::boneGeometry> bonesGeometry);

	/*zapis rotacji do mapy bonesRotations*/
	void saveModelStateToMap(map<string, pf::Vec3f> &bonesRotations, vector<vector<float>> mState, vector<pf::boneConfig> bonesConfig);

	void saveBonesLengthToMap(map<string, float> &bonesLength, vector<pf::boneGeometry> boneGeometry);

	void saveDOFToMap(map<string, vector<bool>> &bonesDOF, vector<pf::boneConfig> boneConfig);

	void saveLimitsToMap(map<string, vector<int>> modelLimits, vector<pf::boneConfig> boneConfig);

	void saveVelocityToMap(map<string, vector<float>> modelVelocity, vector<pf::boneConfig> boneConfig);

	/*aktualizacja wektora modelState po zmianie ilosci kosci modelu*/
	void updateModelStateFromMap(vector<vector<float>> &mState, map<string, pf::Vec3f> bonesRotations, vector<pf::boneConfig> bonesConfig);

	void updateBonesLengthFromMap(map<string, float> bonesLength);

	void updateBonesDOFMap(map<string, vector<bool>> &bonesDOF, string boneName, vector<bool> dofs);

	void updateDOFFromMap(map<string, vector<bool>> bonesDOF, vector<pf::boneConfig> &bonesConfig);
	
	void updateLimitsFromMap(map<string, vector<int>> modelLimits, vector<pf::boneConfig> &boneConfig);

	void updateVelocityFromMap(map<string, vector<float>> modelVelocity, vector<pf::boneConfig> &bonesConfig);

	void updateUsedBones(vector<string> &usedBones, vector<pf::boneConfig> bonesConfig);

	// draw skeleton model 
	void drawSkeletonModel(vector<vector<pf::Vec3f> > modelVertices);

	void drawCylinderModel(vector<vector<pf::Vec3> > vertex, vector<vector<float> > radius);

	void computeCylinder8Vertices(pf::Vec3 eye, vector<vector<pf::Vec3f> >& cylinderVertices, const vector<vector<pf::Vec3f> >& vertices, const vector<vector<float> >& radius);

	/*wczytanie zdjêæ z wybranej œcie¿ki*/
	void loadFiles();

	/*Wczytanie pliku .avi*/
	void loadAviFile();

	void loadAviFile(string path, int hz);

private:
	void initializeBonesRotationsMap();
	void initializeBonesLengthMap();
	void initializeBonesLimitsMap();
	void initializeBonesVelocityMap();
	void initializeBonesDOFMap();
};

#endif // GLWIDGET_H
