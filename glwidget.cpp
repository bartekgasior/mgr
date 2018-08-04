#include "glwidget.h"
#include "asf.h"
#include "Model3D.h"
#include "gl/glu.h"
#include "FileHandler.h"
using namespace std;
using namespace cv;

GLWidget::GLWidget(QWidget *parent) :
	QOpenGLWidget(parent) {
    yRot = 0;
    p.x = p.y = p.z = 0.0;
	setAcceptDrops(true);
	try {
		//cylinder model, default configuration files
		model = new pf::Model3D(pf::Model3D::Cylinder, ASF_TEMPLATE_PATH, DAT_TEMPLATE_PATH);
		//modelState = fileHandler->getAMCTemplate(model);
		//model->setModelState(modelState[0]);
	}
	catch (std::exception &e) {
		QMessageBox::critical(this, "Error", e.what(), QMessageBox::Ok);
		exit(-1);
	}

	pf::MotionData::loadASF(ASF_TEMPLATE_PATH, idxBonesMap, asfBones);
	//model->loadConfig(DAT_TEMPLATE_PATH, bonesConf, bonesGeometry);
}

GLWidget::~GLWidget() {

}

void GLWidget::initializeGL(){
    glClearColor(255,255,255,0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_COLOR_MATERIAL);
	glEnableClientState(GL_VERTEX_ARRAY);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // czarne tlo

    //static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    //glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GLWidget::paintGL(){
    glRotatef(yRot, 0.0, 1.0, 0.0);
	//glColor3f(0.0f, 0.0f, 0.8f);

	if (drawBckg) {
		if (!isAvi)
			drawBackgroud(imgPath);
		else
			drawBackground(aviFrame);
	}
	
	glPushMatrix();

	// draw skeleton model
	glLineWidth(5);
	if (mode == 1) {
		this->drawSkeletonModel(vertices);
	}
	else if (mode == 2) {
		//cout << radiusVec.size() << endl;
		this->drawCylinderModel(vertices, radiusVec);
	}
	//cout << endl;
	glLineWidth(1);

	glPopMatrix();

	glFinish();
}

void GLWidget::resizeGL(int width, int height){
	GLfloat aspectRatio;

	if (height == 0) height = 1;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	w = width;
	h = height;
	//cout << width << " " << height << endl;
	aspectRatio = (GLfloat)width / (GLfloat)height;
	ar = aspectRatio;
	if (width <= height) 
		glOrtho(-1000.0, 1000.0, -1000 / aspectRatio, 1000.0 / aspectRatio, 2000.0, -2000.0);
	else 
		glOrtho(-1000.0 * aspectRatio, 1000.0 * aspectRatio, -1000.0, 1000.0, 2000.0, -2000.0);

	cout << width << " " << height << endl;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLWidget::drawSkeletonModel(vector<vector<pf::Vec3f> > modelVertices)
{
	GLUquadricObj *sphere = gluNewQuadric();
	gluQuadricOrientation(sphere, GLU_INSIDE);

	for (int i = 0; i<(int)modelVertices.size(); ++i)
	{
		
		pf::Vec3 ver(modelVertices[i][0].x(), modelVertices[i][0].y(), -modelVertices[i][0].z());
		pf::Vec3 ver1(modelVertices[i][1].x(), modelVertices[i][1].y(), -modelVertices[i][1].z());

		if (cast == true) {
			ver = camera->cast(ver);
			//cout << ver.x() << endl;
			ver1 = camera->cast(ver1);
		}
		if (drawJoints) {
			glPushMatrix();
			glColor3f(255.0f, 0.0f, 0.0f);
			glTranslatef(ver.x(), ver.y(), -ver.z());
			gluSphere(sphere, jointRadius, 100, 100);
			glPopMatrix();

			glPushMatrix();
			glColor3f(255.0f, 0.0f, 0.0f);
			glTranslatef(ver1.x(), ver1.y(), -ver1.z());
			gluSphere(sphere, jointRadius, 100, 100);
			glPopMatrix();
		}
		glBegin(GL_LINES);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(ver.x(), ver.y(), -ver.z());
			glVertex3f(ver1.x(), ver1.y(), -ver1.z());
		glEnd();
	}
	gluDeleteQuadric(sphere);
}

void GLWidget::drawCylinderModel(vector<vector<pf::Vec3> > vertex, vector<vector<float> > radius) {
	pf::Vec3 eye(0, 0, 10000);
	vector<vector<pf::Vec3> > cylinderVertices(vertex.size());
	float c = 0.01f;
	computeCylinder8Vertices(eye, cylinderVertices, vertex, radius);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1.0f, 1.0f, 1.0f);
	// for each model parts
	for (int i = 0; i<(int)vertex.size(); ++i)
	{
		pf::Vec3 v;
		//glColor3f(0.1f, 0.2f + c, 0.8f);

		if (cast == true) {
			glBegin(GL_QUADS);
			v = cylinderVertices[i][0];
			v = camera->cast(v);
			glVertex3f(v.x(), v.y(), -v.z());
			v = cylinderVertices[i][2];
			v = camera->cast(v);
			glVertex3f(v.x(), v.y(), -v.z());
			v = cylinderVertices[i][6];
			v = camera->cast(v);
			glVertex3f(v.x(), v.y(), -v.z());
			v = cylinderVertices[i][4];
			v = camera->cast(v);
			glVertex3f(v.x(), v.y(), -v.z());
			glEnd();

			// paint cylinder top
			glBegin(GL_QUADS);
			v = cylinderVertices[i][0];
			v = camera->cast(v);
			glVertex3f(v.x(), v.y(), v.z());
			v = cylinderVertices[i][1];
			v = camera->cast(v);
			glVertex3f(v.x(), v.y(), v.z());
			v = cylinderVertices[i][2];
			v = camera->cast(v);
			glVertex3f(v.x(), v.y(), v.z());
			v = cylinderVertices[i][3];
			v = camera->cast(v);
			glVertex3f(v.x(), v.y(), v.z());
			glEnd();

			// paint cylinder bottom
			glBegin(GL_QUADS);
			v = cylinderVertices[i][7];
			v = camera->cast(v);
			glVertex3f(v.x(), v.y(), v.z());
			v = cylinderVertices[i][6];
			v = camera->cast(v);
			glVertex3f(v.x(), v.y(), v.z());
			v = cylinderVertices[i][5];
			v = camera->cast(v);
			glVertex3f(v.x(), v.y(), v.z());
			v = cylinderVertices[i][4];
			v = camera->cast(v);
			glVertex3f(v.x(), v.y(), v.z());
			glEnd();
		}
		else {
			// paint cylinder side
			glBegin(GL_QUADS);
			v = cylinderVertices[i][0];
			glVertex3f(v.x(), v.y(), -v.z());
			v = cylinderVertices[i][2];
			glVertex3f(v.x(), v.y(), -v.z());
			v = cylinderVertices[i][6];
			glVertex3f(v.x(), v.y(), -v.z());
			v = cylinderVertices[i][4];
			glVertex3f(v.x(), v.y(), -v.z());
			glEnd();

			// paint cylinder top
			glBegin(GL_QUADS);
			v = cylinderVertices[i][0];
			glVertex3f(v.x(), v.y(), v.z());
			v = cylinderVertices[i][1];
			glVertex3f(v.x(), v.y(), v.z());
			v = cylinderVertices[i][2];
			glVertex3f(v.x(), v.y(), v.z());
			v = cylinderVertices[i][3];
			glVertex3f(v.x(), v.y(), v.z());
			glEnd();

			// paint cylinder bottom
			glBegin(GL_QUADS);
			v = cylinderVertices[i][7];
			glVertex3f(v.x(), v.y(), v.z());
			v = cylinderVertices[i][6];
			glVertex3f(v.x(), v.y(), v.z());
			v = cylinderVertices[i][5];
			glVertex3f(v.x(), v.y(), v.z());
			v = cylinderVertices[i][4];
			glVertex3f(v.x(), v.y(), v.z());
			glEnd();
		}
		c += 0.03f;

	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void GLWidget::computeCylinder8Vertices(pf::Vec3 eye, vector<vector<pf::Vec3f> >& cylinderVertices, const vector<vector<pf::Vec3f> >& vertices, const vector<vector<float> >& radius) {
	// compute cyliner vertices for each model part
	for (int i = 0; i<(int)cylinderVertices.size(); ++i){
		vector<pf::Vec3f> verticesTmp(8);

		pf::Vec3 lookVec = eye - vertices[i][0];
		// cylinder height
		pf::Vec3 heightVec = vertices[i][1] - vertices[i][0];
		pf::Vec3 rVec = lookVec^heightVec;
		rVec.normalize();

		// bottom
		verticesTmp[0] = vertices[i][0] + rVec*radius[i][0];
		verticesTmp[2] = vertices[i][0] + rVec*(-radius[i][0]);
		// top
		verticesTmp[4] = vertices[i][1] + rVec*radius[i][1];
		verticesTmp[6] = vertices[i][1] + rVec*(-radius[i][1]);

		// additional vertices
		pf::Vec3 fVec = (vertices[i][1] - verticesTmp[0]) ^ heightVec;
		fVec.normalize();

		// bottom
		verticesTmp[1] = vertices[i][0] + fVec*radius[i][0];
		verticesTmp[3] = vertices[i][0] + fVec*(-radius[i][0]);
		// top
		verticesTmp[5] = vertices[i][1] + fVec*radius[i][1];
		verticesTmp[7] = vertices[i][1] + fVec*(-radius[i][1]);

		cylinderVertices[i] = verticesTmp;
	}
}

void GLWidget::drawBone(Bone *bone, cv::Point3f p){
    Bone *childTmp, *sibilingTmp;
    cv::Point3f pTmp;

    pTmp = p;
    pTmp.x +=(bone->length)*(bone->directions[0])/50;
    pTmp.y +=(bone->length)*(bone->directions[1])/50;
    pTmp.z +=(bone->length)*(bone->directions[2])/50;

    glLineWidth(2);

    glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex3f(p.x, p.y, p.z);
        glVertex3f(pTmp.x, pTmp.y, pTmp.z);
    glEnd();

    childTmp = bone->child;
    if(childTmp != NULL){
        GLWidget::drawBone(childTmp , pTmp);
        sibilingTmp = childTmp->sibiling;

        while(sibilingTmp != NULL){
            GLWidget::drawBone(sibilingTmp, pTmp);
            sibilingTmp = sibilingTmp->sibiling;
        }
    }
}

void GLWidget::drawSkeleton(){
    Bone *root;
    Bone *child, *sibiling;

    root = &skeletonBones[0];
    child = root->child;

    if(child !=NULL){
        GLWidget::drawBone(child , p);

        sibiling = child->sibiling;

        while(sibiling != NULL){
            GLWidget::drawBone(sibiling, p);
            sibiling = sibiling->sibiling;
        }
    }else{
        //std::cout << bone->child->name << " " << bone->id <<std::endl;
        std::cout << "no child " << std::endl;
    }
}

void GLWidget::rotate(string boneName, float direction, pf::Vec3 vect, int rotVal, vector<pf::boneConfig> bConf, vector<pf::range2> limitsVector, int frame, vector<vector<float>> &modelState) {

	int actualBoneNameIDtmp, limitID; 

	//id kosci wsrod wykorzystywanych w modelu
	for (int i = 0; i < bConf.size(); i++) {
		if (bConf[i].name == boneName)
			actualBoneNameIDtmp = i;
	}
	//limit kazdej z wykorzystanych kosci opisany jest przez 3 kolejne el. wektora 'limits'
	for (int i = 3; i < limitsVector.size(); i++) {
		if (actualBoneNameIDtmp+1 == i / 3) {
			limitID = i;
			break;
		}
	}

	float angle;
	//float rotVal = 5.0f;
	if (vect == pf::Model3D::axisX) {
		angle = modelState[frame][limitID] + rotVal*direction;
		//cout << "min: " << limits[limitID].min << " max: " << limits[limitID].max << endl;
		if (limitsVector[limitID].min <= angle && angle <= limitsVector[limitID].max)
			modelState[frame][limitID] += rotVal*direction;

		//cout << "angle: " << angle << " min: " << limits[limitID].min << " max: " << limits[limitID].max << endl;
	}
	if (vect == pf::Model3D::axisY) {

		angle = modelState[frame][limitID+1] + rotVal*direction;
		if (limitsVector[limitID+1].min <= angle && angle <= limitsVector[limitID+1].max)
			modelState[frame][limitID+1] += rotVal*direction;

		//cout << "angle: " << angle << " min: " << limits[limitID+1].min << " max: " << limits[limitID+1].max << endl;
	}
	if (vect == pf::Model3D::axisZ) {

		angle = modelState[frame][limitID+2] + rotVal*direction;
		if (limitsVector[limitID+2].min <= angle && angle <= limitsVector[limitID+2].max)
			modelState[frame][limitID+2] += rotVal*direction;

		//cout << "angle: " << angle << " min: " << limits[limitID+2].min << " max: " << limits[limitID+2].max << endl;
	}

	//cout << modelState[0][limitID] << " " << modelState[0][limitID+1] << " " << modelState[0][limitID + 2] << endl;
	//cout << endl;

	//model->setModelState(modelState[frame]);

	//update();
}

void GLWidget::rotate(string boneName, pf::Vec3 vect, int rotVal, vector<pf::boneConfig> bConf, vector<pf::range2> limitsVector, int frame, vector<vector<float>> &modelState) {
	int actualBoneNameIDtmp, limitID;

	//id kosci wsrod wykorzystywanych w modelu
	for (int i = 0; i < bConf.size(); i++) {
		if (bConf[i].name == boneName)
			actualBoneNameIDtmp = i;
	}
	//limit kazdej z wykorzystanych kosci opisany jest przez 3 kolejne el. wektora 'limits'
	for (int i = 3; i < limitsVector.size(); i++) {
		if (actualBoneNameIDtmp + 1 == i / 3) {
			limitID = i;
			break;
		}
	}

	float angle;
	if (vect == pf::Model3D::axisX) {
		angle = rotVal;
		if (limitsVector[limitID].min <= angle && angle <= limitsVector[limitID].max)
			modelState[frame][limitID] = rotVal;

	}
	if (vect == pf::Model3D::axisY) {
		angle =rotVal;
		if (limitsVector[limitID + 1].min <= angle && angle <= limitsVector[limitID + 1].max)
			modelState[frame][limitID + 1] = rotVal;

	}
	if (vect == pf::Model3D::axisZ) {
		angle = rotVal;
		if (limitsVector[limitID + 2].min <= angle && angle <= limitsVector[limitID + 2].max)
			modelState[frame][limitID + 2] = rotVal;
	}
}

void GLWidget::translate(float direction, pf::Vec3 vect, int value, int frame, vector<vector<float>> &modelTranslation, vector<vector<float>> &modelState) {
	if (vect == pf::Model3D::axisX){
		modelState[frame][0] += value*direction;
		modelTranslation[frame][0] = modelState[0][0];
	}

	if (vect == pf::Model3D::axisY) {
		modelState[frame][1] += value*direction;
		modelTranslation[frame][1] = modelState[0][1];
	}

	if (vect == pf::Model3D::axisZ) {
		modelState[frame][2] += value*direction;
		modelTranslation[frame][2] = modelState[0][2];
	}

	//cout << modelState[0][0] << " " << modelState[0][1] << " " << modelState[0][2] << endl;

	model->setModelState(modelState[frame]);
	update();
}

void GLWidget::translate(pf::Vec3 vect, int value, int frame, vector<vector<float>> &modelTranslation, vector<vector<float>> &modelState) {
	if (vect == pf::Model3D::axisX) {
		modelState[frame][0] = value;
		modelTranslation[frame][0] = modelState[0][0];
	}

	if (vect == pf::Model3D::axisY) {
		modelState[frame][1] = value;
		modelTranslation[frame][1] = modelState[0][1];
	}

	if (vect == pf::Model3D::axisZ) {
		modelState[frame][2] = value;
		modelTranslation[frame][2] = modelState[0][2];
	}

	//cout << modelState[0][0] << " " << modelState[0][1] << " " << modelState[0][2] << endl;

	model->setModelState(modelState[frame]);
	update();
}

void GLWidget::scale(float direction, int value) {}

void GLWidget::drawBackgroud(string img) {
	cv::Mat mat = cv::imread(img, CV_LOAD_IMAGE_COLOR);
	if (mat.empty()) {
		cerr << "img empty" << endl;
	}
	else {
		if (scaleFrame)
			cv::resize(mat, mat, cv::Size(frameWidth, frameHeight), 0, 0, CV_INTER_LINEAR);

		else {
			cv::Mat tmp;
			mat.copyTo(tmp);
			GLuint tex;
			cv::flip(tmp, tmp, 0);
			glGenTextures(1, &tex);
			glBindTexture(GL_TEXTURE_2D, tex);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Set texture clamping method
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


			glTexImage2D(GL_TEXTURE_2D,     // Type of texture
				0,                 // Pyramid level (for mip-mapping) - 0 is the top level
				GL_RGB,            // Internal colour format to convert to
				tmp.cols,          // Image width  i.e. 640 for Kinect in standard mode
				tmp.rows,          // Image height i.e. 480 for Kinect in standard mode
				0,                 // Border width in pixels (can either be 1 or 0)
				GL_BGR, // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
				GL_UNSIGNED_BYTE,  // Image data type
				tmp.ptr());        // The actual image data itself

			glDisable(GL_BLEND);
			glEnable(GL_TEXTURE_2D);

			glBegin(GL_QUADS);
			//glTexCoord2f(0.0f, 0.0f); glVertex3f(-w - (700 * ar), -h - (700 * ar), 1000);
			//glTexCoord2f(1.0f, 0.0f); glVertex3f(w + (700 * ar), -h - (700 * ar), 1000);
			//glTexCoord2f(1.0f, 1.0f); glVertex3f(w + (700 * ar), h + (700 * ar), 1000);
			//glTexCoord2f(0.0f, 1.0f); glVertex3f(-w - (700 * ar), h + (700 * ar), 1000);

			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1350, -1000, 1000);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(1350, -1000, 1000);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(1350, 1000, 1000);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1350, 1000, 1000);
			glEnd();

			glDisable(GL_TEXTURE_2D);
			glFinish();

			tmp.release();
			glDeleteTextures(1, &tex);
		}
	}
	mat.release();
}

void GLWidget::drawBackground(cv::Mat mat) {
	if (mat.empty()) {
		cerr << "img empty" << endl;
	}
	else {
		cv::Mat tmp;
		mat.copyTo(tmp);
		GLuint tex;
		cv::flip(tmp, tmp, 0);
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Set texture clamping method
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


		glTexImage2D(GL_TEXTURE_2D,     // Type of texture
			0,                 // Pyramid level (for mip-mapping) - 0 is the top level
			GL_RGB,            // Internal colour format to convert to
			tmp.cols,          // Image width  i.e. 640 for Kinect in standard mode
			tmp.rows,          // Image height i.e. 480 for Kinect in standard mode
			0,                 // Border width in pixels (can either be 1 or 0)
			GL_BGR, // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
			GL_UNSIGNED_BYTE,  // Image data type
			tmp.ptr());        // The actual image data itself

		glDisable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1350, -1000, 1000);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1350, -1000, 1000);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1350, 1000, 1000);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1350, 1000, 1000);
		glEnd();

		glDisable(GL_TEXTURE_2D);
		glFinish();

		tmp.release();
		glDeleteTextures(1, &tex);
	}
	mat.release();
}

void GLWidget::updateUsedBones(vector<string> &usedBones, vector<pf::boneConfig> bonesConfig) {
	usedBones.clear();
	for (int i = 0; i < bonesConfig.size(); i++) {
		usedBones.push_back(bonesConfig[i].name);
	}
}

void GLWidget::loadFiles() {
	
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Load image"), "",
		tr("Image files(*.png *.jpg *.jpeg *.bmp) ;; PNG (*.png);; JPG (*.jpg);; JPEG (*.jpeg) ;; BMP (*.bmp) ;; All Files(*)"));

	if (!fileName.isEmpty()) {
		loadedImagesFolderPath = QFileInfo(fileName).absolutePath();

		if (!loadedImagesFolderPath.isEmpty()) {
			aviFrames.clear();
			list.clear();
			QDir dir(loadedImagesFolderPath);
			QStringList filter;
			filter << QLatin1String("*.png");
			filter << QLatin1String("*.jpg");
			filter << QLatin1String("*.jpeg");
			filter << QLatin1String("*.bmp");
			dir.setNameFilters(filter);
			list = dir.entryInfoList();

			checkImagesList(fileName);

			isAvi = false;
		}
	}
}

void GLWidget::loadFiles(QString fileName) {
	if (!fileName.isEmpty()) {
		
		loadedImagesFolderPath = QFileInfo(fileName).absolutePath();
			
		if (!loadedImagesFolderPath.isEmpty()) {
			aviFrames.clear();
			list.clear();
			QDir dir(loadedImagesFolderPath);
			QStringList filter;
			filter << QLatin1String("*.png");
			filter << QLatin1String("*.jpg");
			filter << QLatin1String("*.jpeg");
			filter << QLatin1String("*.bmp");
			dir.setNameFilters(filter);
			list = dir.entryInfoList();
				
			checkImagesList(fileName);

			isAvi = false;
		}
	}
}

void GLWidget::loadAviFile() {
	//isAvi = false;
	aviFilePath = QFileDialog::getOpenFileName(this,
		tr("Load .avi"), "",
		tr("AVI (*.avi);;All Files(*)"));

	if (!aviFilePath.isEmpty()) {

		VideoCapture cap(aviFilePath.toUtf8().constData());
		AviHzDialog *aviDialog = new AviHzDialog(this);

		int framesCount = cap.get(CV_CAP_PROP_FRAME_COUNT);
		aviDialog->setFramesCounter(framesCount);
		aviDialog->setLowerHigherSpinBoxes(framesCount);

		aviDialog->resize(200, 100);
		aviDialog->exec();

		int mode = aviDialog->mode;
		if (aviDialog->clicked) {
			if (!cap.isOpened()) {
				QMessageBox::warning(NULL, QObject::tr("app"),
					QObject::tr("Error while opening avi file"),
					QMessageBox::Cancel,
					QMessageBox::Cancel);
			}
			else {
				if (mode == 0) {
					list.clear();
					aviFrames.clear();
					Mat frame;

					bool stop = false;
					int i = 0;
					while (!stop) {

						cap >> frame;

						if (frame.empty()) {
							stop = true;
							continue;
						}
						if (scaleFrame)
							cv::resize(frame, frame, cv::Size(frameWidth, frameHeight), 0, 0, CV_INTER_LINEAR);
						std::cout << "." << endl;
						aviFrames.push_back(frame);
						i++;

					}
				}
				if (mode == 1) {
					int hz = aviDialog->getHz();
					list.clear();
					aviFrames.clear();
					Mat frame;

					bool stop = false;
					int i = 0;
					while (!stop) {

						cap >> frame;

						if (frame.empty()) {
							stop = true;
							continue;
						}
						if (i%hz == 0) {
							if (scaleFrame)
								cv::resize(frame, frame, cv::Size(frameWidth, frameHeight), 0, 0, CV_INTER_LINEAR);
							std::cout << "." << endl;
							aviFrames.push_back(frame);
						}
						i++;

					}
				}
				if (mode == 2) {
					int higher = aviDialog->getSpinBox3Value();
					int lower = aviDialog->getSpinBox2Value();

					cout << lower << " " << higher << endl;
					list.clear();
					aviFrames.clear();
					Mat frame;

					bool stop = false;
					int i = 0;
					while (!stop) {

						cap >> frame;

						if (frame.empty()) {
							stop = true;
							continue;
						}
						if (i >= lower && i <= higher) {
							if (scaleFrame)
								cv::resize(frame, frame, cv::Size(frameWidth, frameHeight), 0, 0, CV_INTER_LINEAR);
							std::cout << "." << endl;
							aviFrames.push_back(frame);
						}
						i++;

					}
				}
				isAvi = true;
				cap.release();
			}
		}
		delete aviDialog;
	}
	cout << aviFilePath.toUtf8().constData() << endl;
}

void GLWidget::loadAviFile(QString path) {
	if (!path.isEmpty()) {
		VideoCapture cap(path.toUtf8().constData());
		AviHzDialog *aviDialog = new AviHzDialog(this);

		int framesCount = cap.get(CV_CAP_PROP_FRAME_COUNT);
		aviDialog->setFramesCounter(framesCount);
		aviDialog->setLowerHigherSpinBoxes(framesCount);

		aviDialog->resize(200, 100);
		aviDialog->exec();

		int mode = aviDialog->mode;
		if (aviDialog->clicked) {
			if (!cap.isOpened()) {
				QMessageBox::warning(NULL, QObject::tr("app"),
					QObject::tr("Error while opening avi file"),
					QMessageBox::Cancel,
					QMessageBox::Cancel);
			}
			else {
				aviFilePath = path;
				if (mode == 0) {
					list.clear();
					aviFrames.clear();
					Mat frame;

					bool stop = false;
					int i = 0;
					while (!stop) {

						cap >> frame;

						if (frame.empty()) {
							stop = true;
							continue;
						}
						if(scaleFrame)
							cv::resize(frame, frame, cv::Size(frameWidth, frameHeight), 0, 0, CV_INTER_LINEAR);
						std::cout << "." << endl;
						aviFrames.push_back(frame);
						i++;

					}
				}
				if (mode == 1) {
					int hz = aviDialog->getHz();
					list.clear();
					aviFrames.clear();
					Mat frame;

					bool stop = false;
					int i = 0;
					while (!stop) {

						cap >> frame;

						if (frame.empty()) {
							stop = true;
							continue;
						}
						if (i%hz == 0) {
							if (scaleFrame)
								cv::resize(frame, frame, cv::Size(frameWidth, frameHeight), 0, 0, CV_INTER_LINEAR);
							std::cout << "." << endl;
							aviFrames.push_back(frame);
						}
						i++;

					}
				}
				if (mode == 2) {
					int higher = aviDialog->getSpinBox3Value();
					int lower = aviDialog->getSpinBox2Value();

					cout << lower << " " << higher << endl;
					list.clear();
					aviFrames.clear();
					Mat frame;

					bool stop = false;
					int i = 0;
					while (!stop) {

						cap >> frame;

						if (frame.empty()) {
							stop = true;
							continue;
						}
						if (i >= lower && i <= higher) {
							if (scaleFrame)
								cv::resize(frame, frame, cv::Size(frameWidth, frameHeight), 0, 0, CV_INTER_LINEAR);
							std::cout << "." << endl;
							aviFrames.push_back(frame);
						}
						i++;

					}
				}
				isAvi = true;
				cap.release();
			}
		}
		delete aviDialog;
	}
	cout << aviFilePath.toUtf8().constData() << endl;
}

void GLWidget::copyConfigToGlWidget(GLWidget *&dest, GLWidget *source) {
	/*dest->limits = source->limits;
	dest->rotations = source->rotations;
	dest->usedBones = source->usedBones;
	dest->allBones = source->allBones;
	dest->modelState = source->modelState;
	dest->asfBones = source->asfBones;
	dest->idxBonesMap = source->idxBonesMap;
	dest->bonesConf = source->bonesConf;
	dest->bonesGeometry = source->bonesGeometry;
	dest->bonesRotations = source->bonesRotations;
	dest->bonesLength = source->bonesLength;
	dest->modelTranslation = source->modelTranslation;
	dest->modelLimits = source->modelLimits;
	dest->modelVelocity = source->modelVelocity;
	dest->modelDOF = source->modelDOF;

	saveModelStateToMap(dest->bonesRotations, dest->modelState, dest->bonesConf);
	saveBonesLengthToMap(dest->bonesLength, dest->bonesGeometry);
	saveDOFToMap(dest->modelDOF, dest->bonesConf);
	saveLimitsToMap(dest->modelLimits, dest->bonesConf);
	saveVelocityToMap(dest->modelVelocity, dest->bonesConf);

	FileHandler *fileHandler = new FileHandler();
	fileHandler->reloadParams(dest);

	delete fileHandler;*/
}

void GLWidget::setGLWidgetCamera(pf::Camera *cam) {
	if (cam->getType() == pf::Camera::TSAI1) {
		pf::TSAI1Camera tsai1Camera;
		tsai1Camera.trans.x = cam->getTSAI1Param().trans.x;
		tsai1Camera.trans.y = cam->getTSAI1Param().trans.y;
		tsai1Camera.trans.z = cam->getTSAI1Param().trans.z;
		tsai1Camera.rot.x = cam->getTSAI1Param().rot.x;
		tsai1Camera.rot.y = cam->getTSAI1Param().rot.y;
		tsai1Camera.rot.z = cam->getTSAI1Param().rot.z;
		tsai1Camera.focal = cam->getTSAI1Param().focal;
		tsai1Camera.kappa1 = cam->getTSAI1Param().kappa1;
		tsai1Camera.c.x = cam->getTSAI1Param().c.x;
		tsai1Camera.c.y = cam->getTSAI1Param().c.y;
		tsai1Camera.sx = cam->getTSAI1Param().sx;
		tsai1Camera.ncx = cam->getTSAI1Param().ncx;
		tsai1Camera.nfx = cam->getTSAI1Param().nfx;
		tsai1Camera.d.x = cam->getTSAI1Param().d.x;
		tsai1Camera.d.y = cam->getTSAI1Param().d.y;
		tsai1Camera.dp.x = cam->getTSAI1Param().dp.x;
		tsai1Camera.dp.y = cam->getTSAI1Param().dp.y;

		this->camera = new pf::Camera(tsai1Camera, cam->getWidth(), cam->getHeight());
	}
	if (cam->getType() == pf::Camera::TSAI2) {
		pf::TSAI2Camera tsai2Camera;
		tsai2Camera.trans.x = cam->getTSAI2Param().trans.x;
		tsai2Camera.trans.y = cam->getTSAI2Param().trans.y;
		tsai2Camera.trans.z = cam->getTSAI2Param().trans.z;
		tsai2Camera.rot.x = cam->getTSAI2Param().rot.x;
		tsai2Camera.rot.y = cam->getTSAI2Param().rot.y;
		tsai2Camera.rot.z = cam->getTSAI2Param().rot.z;
		tsai2Camera.focal1 = cam->getTSAI2Param().focal1;
		tsai2Camera.focal2 = cam->getTSAI2Param().focal2;
		tsai2Camera.c.x = cam->getTSAI2Param().c.x;
		tsai2Camera.c.y = cam->getTSAI2Param().c.y;
		tsai2Camera.K1 = cam->getTSAI2Param().K1;
		tsai2Camera.K2 = cam->getTSAI2Param().K2;
		tsai2Camera.P1 = cam->getTSAI2Param().P1;
		tsai2Camera.P2 = cam->getTSAI2Param().P2;

		this->camera = new pf::Camera(tsai2Camera, cam->getWidth(), cam->getHeight());
	}
	if (cam->getType() == pf::Camera::Perspective) {
		pf::Camera camTmp;
		float pos_x = cam->getEye().x();
		float pos_y = cam->getEye().y();
		float pos_z = cam->getEye().z();
		bool useQuat = false;
		float quat_x = cam->getOrientation().x();
		float quat_y = cam->getOrientation().y();
		float quat_z = cam->getOrientation().z();
		float quat_w = cam->getOrientation().w();
		float rot_x = RAD_TO_DEG*cam->getAlpha();
		float rot_y = RAD_TO_DEG* cam->getBeta();
		float rot_z = RAD_TO_DEG*cam->getGama();
		float fovy = cam->getFovy();

		if (useQuat) 
			this->camera = new pf::Camera(pf::Vec3f(pos_x, pos_y, pos_z), pf::Quat(quat_x, quat_y, quat_z, quat_w), fovy, cam->getWidth(), cam->getHeight());
		else
			this->camera = new pf::Camera(pf::Vec3f(pos_x, pos_y, pos_z), rot_x, rot_y, rot_z, fovy, cam->getWidth(), cam->getHeight());
	}
	if (cam->getType() == pf::Camera::PerspectiveDisortion) {
		pf::Camera camTmp;
		float pos_x = cam->getEye().x();
		float pos_y = cam->getEye().y();
		float pos_z = cam->getEye().z();
		bool useQuat = false;
		float quat_x = cam->getOrientation().x();
		float quat_y = cam->getOrientation().y();
		float quat_z = cam->getOrientation().z();
		float quat_w = cam->getOrientation().w();
		float rot_x = RAD_TO_DEG*cam->getAlpha();
		float rot_y = RAD_TO_DEG* cam->getBeta();
		float rot_z = RAD_TO_DEG*cam->getGama();
		float fovy = cam->getFovy();
		pf::disortions dis;
		dis.K1 = cam->getDisortionParams().K1;
		dis.K2 = cam->getDisortionParams().K2;
		dis.P1 = cam->getDisortionParams().P1;
		dis.P2 = cam->getDisortionParams().P2;

		if (useQuat)
			this->camera = new pf::Camera(pf::Vec3f(pos_x, pos_y, pos_z), pf::Quat(quat_x, quat_y, quat_z, quat_w), fovy, cam->getWidth(), cam->getHeight(), dis);
		else
			this->camera = new pf::Camera(pf::Vec3f(pos_x, pos_y, pos_z), rot_x, rot_y, rot_z, fovy, cam->getWidth(), cam->getHeight(), dis);
	}
}

void GLWidget::setGLWidgetModelState(int i, vector<vector<float>> modelState) {
	model->setModelState(modelState[i]);
}

void GLWidget::checkImagesList(QString fileName) {
	QString prefix, sufix;
	QFileInfoList listTMP;
	QString name = QFileInfo(fileName).baseName();
	for (int j = name.length() - 1; j >= 0; j--) {
		if (!name[j].isDigit()) {
			prefix = name.mid(0, j + 1);
			sufix = name.right(j + 1);
			break;
		}
	}

	for (int i = 0; i < list.size(); i++) {
		if (list[i].baseName().mid(0, prefix.length()) == prefix) {
			if (list[i].baseName().right(prefix.length()).toInt() >= sufix.toInt()) {
				cout << list[i].baseName().toUtf8().constData() << endl;
				listTMP.push_back(list[i]);
			}
		}
	}
	cout << endl;
	list.clear();
	list = listTMP;
}

void GLWidget::setRadiusVertices(vector<vector<pf::Vec3f>> ver, vector<vector<float>> rad) {
	vertices.clear();
	radiusVec.clear();
	vertices = ver;
	radiusVec = rad;

	update();
}
/*
void GLWidget::dropEvent(QDropEvent *e) {
	if (e->mimeData()->hasUrls()){
			
		if (e->mimeData()->urls().size() > 1) {
			cout << "to many files loaded" << endl;
		}
		else {
			QUrl url = e->mimeData()->urls()[0];
			QString fileName = url.toLocalFile();
			loadFiles(fileName);
			droppedInside = true;
		}

	}
}

void GLWidget::dragEnterEvent(QDragEnterEvent *e) {
	if (e->mimeData()->hasUrls()) {
		e->acceptProposedAction();
	}
}

void GLWidget::dragLeaveEvent(QDragLeaveEvent *e) {

}*/