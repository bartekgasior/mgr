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
#include <QMouseEvent>
#include <QMessageBox>

#include <iostream>
#include <fstream>
#include <cstring>

#include "qmimedata.h"
#include "avihzdialog.h"
#include "Camera.h"
#include "ModelHandler.h"

#define ASF_TEMPLATE_PATH "ETC/modelTemplate.asf"
#define AMC_TEMPLATE_PATH "ETC/modelTemplate.amc"
#define DAT_TEMPLATE_PATH "ETC/modelTemplate.dat"

#define GOLDEN_RATIO 1.61803398875

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
	~GLWidget();

	int mode = 1;
	/*promien rysowanego stawu*/
	int jointRadius = 30;
	/*czy rysowac staw*/
	bool drawJoints = true;
	/*zmienne do skalowania klatki*/
	bool scaleFrame = false;
	int frameWidth = 0;
	int frameHeight = 0;

	QVector<Bone> skeletonBones;
	cv::Point3f p;
	int yRot, w, h;
	GLfloat ar;

	//sciezka do folderu ze zdjeciami
	QString loadedImagesFolderPath;
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
	//int hz = 0;

	/*czy wykorzystywac kamere*/
	bool cast = false;
	int cameraID = 0;
	pf::Camera *camera;

	//vector<vector<float>> modelState;//stany modelu
	//vector<pf::range2> limits;// vector zawieraj¹cy limity rotacji poszczególnych koœci
	//vector<float> velocity;
	//vector<pf::Matrixf> rotations;//rotacje
	vector<string> usedBones;//nazwy wykorzystanych kosci
	//vector<string> allBones;//nazwy wszystkich kosci
	vector<pf::ASFBone> asfBones;//kosci
	map<string, int> idxBonesMap;
	//vector<pf::boneConfig> bonesConf;//parametry kosci
	//vector<pf::boneGeometry> bonesGeometry;
	vector<vector<pf::Vec3f>> vertices;
	vector<vector<float>> radiusVec;

	/*wektor o rozmiarze modelState, gdy true zapisuje konfiguracje*/
	//vector<bool> saveModelState;

	/*obroty wszystkich kosci wraz z nazwa*/
	//vector<map<string, pf::Vec3f>> bonesRotations;

	/*przesuniecie modelu*/
	//vector<vector<float>> modelTranslation;

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
	void rotate(string boneName, float direction, pf::Vec3 vect, int rotVal, vector<pf::boneConfig> bConf, vector<pf::range2> limitsVector, int frame, vector<vector<float>> &modelState);

	void rotate(string boneName, pf::Vec3 vect, int rotVal, vector<pf::boneConfig> bConf, vector<pf::range2> limitsVector, int frame, vector<vector<float>> &modelState);

	/*przesuniecie modelu*/
	void translate(float direction, pf::Vec3 vect, int value, int frame, vector<vector<float>> &modelTranslation, vector<vector<float>> &modelState);

	void translate(pf::Vec3 vect, int value, int frame, vector<vector<float>> &modelTranslation, vector<vector<float>> &modelState);

	/*skalowanie wszystkich kosci modelu*/
	void scale(float direction, int value);	

	void updateUsedBones(vector<string> &usedBones, vector<pf::boneConfig> bonesConfig);

	// draw skeleton model 
	void drawSkeletonModel(vector<vector<pf::Vec3f> > modelVertices);

	void drawCylinderModel(vector<vector<pf::Vec3> > vertex, vector<vector<float> > radius);

	void computeCylinder8Vertices(pf::Vec3 eye, vector<vector<pf::Vec3f> >& cylinderVertices, const vector<vector<pf::Vec3f> >& vertices, const vector<vector<float> >& radius);

	/*wczytanie zdjêæ z wybranej œcie¿ki*/
	void loadFiles();

	void loadFiles(QString fileName);

	/*Wczytanie pliku .avi*/
	void loadAviFile();

	void loadAviFile(QString path);

	/*zapisanie konfiguracji z jednego modelu do drugiego*/
	void copyConfigToGlWidget(GLWidget *&dest, GLWidget *source);

	void setGLWidgetCamera(pf::Camera *cam);

	/*po wczytaniu tla funkcja tworzy wektor model state o rozmiarze rownym ilosci zaladowanych klatek*/
	//void updateModelStateVector(int frames);

	/*ustawienie aktualnego modelState modelu*/
	void setGLWidgetModelState(int i, vector<vector<float>> modelState);

	/*ustawienie wektorow verticies oraz radiusVec*/
	void setRadiusVertices(vector<vector<pf::Vec3f>> ver, vector<vector<float>> rad);

private:
	void checkImagesList(QString fileName);

	//void dragEnterEvent(QDragEnterEvent *e);
	//void dragLeaveEvent(QDragLeaveEvent *e);
	//void dropEvent(QDropEvent *e);
};

#endif // GLWIDGET_H
