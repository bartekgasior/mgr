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
    //skeletonBones = ASF::getASF();
	FileHandler * fileHandler;
	try {
		//cylinder model, default configuration files
		model = new pf::Model3D(pf::Model3D::Cylinder, ASF_TEMPLATE_PATH, DAT_TEMPLATE_PATH);
		modelState = fileHandler->getAMCTemplate(model);
		model->setModelState(modelState[0]);
	}
	catch (std::exception &e) {
		QMessageBox::critical(this, "Error", e.what(), QMessageBox::Ok);
		exit(-1);
	}
	limits = model->getLimits();
	velocity = model->getVelocity();
	usedBones = model->getNamesMovingBones();
	allBones = model->getNamesBones();
	

	pf::MotionData::loadASF(ASF_TEMPLATE_PATH, idxBonesMap, asfBones);
	model->loadConfig(DAT_TEMPLATE_PATH, bonesConf, bonesGeometry);

	initializeBonesRotationsMap();
	initializeBonesLengthMap();
	initializeBonesLimitsMap();
	initializeBonesVelocityMap();
	initializeBonesDOFMap();
	initializeBonesRadiusMap();

	modelTranslation.push_back(0);
	modelTranslation.push_back(0);
	modelTranslation.push_back(0);
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
	
	vector<vector<pf::Vec3f> > vertices = this->model->getRotatedVertices();
	//glColor3f(0.0f, 0.0f, 0.8f);

	if (isInMainWindow) {
		if (drawBckg) {
			if (!isAvi)
				drawBackgroud(imgPath);
			else
				drawBackground(aviFrame);
		}
	}
	
	glPushMatrix();

	// draw skeleton model
	glLineWidth(5);
	if (mode == 1) {
		this->drawSkeletonModel(vertices);
	}
	else if (mode == 2) {
		vector<vector<float>> radiusVec = model->getRadiusVec();
		this->drawCylinderModel(vertices, radiusVec);
	}
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
	aspectRatio = (GLfloat)width / (GLfloat)height;
	ar = aspectRatio;
	if (width <= height) glOrtho(-1000.0, 1000.0, -1000 / aspectRatio, 1000.0 / aspectRatio, 2000.0, -2000.0);
	else glOrtho(-1000.0 * aspectRatio, 1000.0 * aspectRatio, -1000.0, 1000.0, 2000.0, -2000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLWidget::drawSkeletonModel(vector<vector<pf::Vec3f> > modelVertices)
{
	for (int i = 0; i<(int)modelVertices.size(); ++i)
	{
		glBegin(GL_LINES);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(modelVertices[i][0].x(), modelVertices[i][0].y(), -modelVertices[i][0].z());
			glVertex3f(modelVertices[i][1].x(), modelVertices[i][1].y(), -modelVertices[i][1].z());
		glEnd();

	}
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

void GLWidget::rotate(string boneName, float direction, pf::Vec3 vect, int rotVal) {

	int actualBoneNameIDtmp, limitID; 

	//id kosci wsrod wykorzystywanych w modelu
	for (int i = 0; i < bonesConf.size(); i++) {
		if (bonesConf[i].name == boneName)
			actualBoneNameIDtmp = i;
	}
	//limit kazdej z wykorzystanych kosci opisany jest przez 3 kolejne el. wektora 'limits'
	for (int i = 3; i < limits.size(); i++) {
		if (actualBoneNameIDtmp+1 == i / 3) {
			limitID = i;
			break;
		}
	}

	float angle;
	//float rotVal = 5.0f;
	if (vect == pf::Model3D::axisX) {
		angle = modelState[0][limitID] + rotVal*direction;
		//cout << "min: " << limits[limitID].min << " max: " << limits[limitID].max << endl;
		if (limits[limitID].min <= angle && angle <= limits[limitID].max) 
			modelState[0][limitID] += rotVal*direction;

		//cout << "angle: " << angle << " min: " << limits[limitID].min << " max: " << limits[limitID].max << endl;
	}
	if (vect == pf::Model3D::axisY) {

		angle = modelState[0][limitID+1] + rotVal*direction;
		if (limits[limitID+1].min <= angle && angle <= limits[limitID+1].max)
			modelState[0][limitID+1] += rotVal*direction;

		//cout << "angle: " << angle << " min: " << limits[limitID+1].min << " max: " << limits[limitID+1].max << endl;
	}
	if (vect == pf::Model3D::axisZ) {

		angle = modelState[0][limitID+2] + rotVal*direction;
		if (limits[limitID+2].min <= angle && angle <= limits[limitID+2].max)
			modelState[0][limitID+2] += rotVal*direction;

		//cout << "angle: " << angle << " min: " << limits[limitID+2].min << " max: " << limits[limitID+2].max << endl;
	}

	//cout << modelState[0][limitID] << " " << modelState[0][limitID+1] << " " << modelState[0][limitID + 2] << endl;
	//cout << endl;

	model->setModelState(modelState[0]);

	update();
}

void GLWidget::updateLength(string boneName, float direction, float value) {
	for (int i = 0; i < bonesGeometry.size(); i++) {
		if (bonesGeometry[i].name == boneName) {
			
			bonesGeometry[i].length += value*direction;

			if (bonesGeometry[i].length < 0)
				bonesGeometry[i].length = 0;

			model->updateBoneGeometry(boneName, bonesGeometry[i]);
			break;
		}
	}
	update();
}

void GLWidget::updateTopRadius(string boneName, float direction, float value) {
	for (int i = 0; i < bonesGeometry.size(); i++) {
		if (bonesGeometry[i].name == boneName) {

			bonesGeometry[i].topRadius1 += value*direction;
			bonesGeometry[i].topRadius2 += value*direction;

			if (bonesGeometry[i].topRadius1 < 0)
				bonesGeometry[i].topRadius1 = 0;

			if (bonesGeometry[i].topRadius2 < 0)
				bonesGeometry[i].topRadius2 = 0;

			model->updateBoneGeometry(boneName, bonesGeometry[i]);
			break;
		}
	}
	update();
}

void GLWidget::updateBottomRadius(string boneName, float direction, float value) {
	for (int i = 0; i < bonesGeometry.size(); i++) {
		if (bonesGeometry[i].name == boneName) {

			bonesGeometry[i].bottomRadius1 += value*direction;
			bonesGeometry[i].bottomRadius2 += value*direction;

			if (bonesGeometry[i].bottomRadius1 < 0)
				bonesGeometry[i].bottomRadius1 = 0;

			if (bonesGeometry[i].bottomRadius2 < 0)
				bonesGeometry[i].bottomRadius2 = 0;

			model->updateBoneGeometry(boneName, bonesGeometry[i]);
			break;
		}
	}
	update();
}

void GLWidget::translate(float direction, pf::Vec3 vect, int value) {
	if (vect == pf::Model3D::axisX){
		modelState[0][0] += value*direction;
		modelTranslation[0] = modelState[0][0];
	}

	if (vect == pf::Model3D::axisY) {
		modelState[0][1] += value*direction;
		modelTranslation[1] = modelState[0][1];
	}

	if (vect == pf::Model3D::axisZ) {
		//modelState[0][2] += value*direction;
		//modelTranslation[2] = modelState[0][2];
	}

	//cout << modelState[0][0] << " " << modelState[0][1] << " " << modelState[0][2] << endl;

	model->setModelState(modelState[0]);
	update();
}

void GLWidget::scale(float direction, int value) {
	for (int i = 0; i < bonesConf.size(); i++) {
		for (int j = 0; j < bonesGeometry.size(); j++) {
			if (bonesConf[i].name == bonesGeometry[j].name) {
				bonesGeometry[j].length += value*direction;

				if (bonesGeometry[j].length < 0)
					bonesGeometry[j].length = 0;

				model->updateBoneGeometry(bonesGeometry[j].name, bonesGeometry[j]);
				model->updateBoneLength(bonesGeometry[j].name, bonesGeometry[j].length);
			}
		}
	}
	update();
}

float GLWidget::getBoneLength(string boneName) {
	float res = 0;
	for (int i = 0; i < bonesGeometry.size(); i++) {
		if (boneName == bonesGeometry[i].name) {
			res = bonesGeometry[i].length;
			break;
		}
	}
	return res;
}

float GLWidget::getBoneTopRadius(string boneName){
	float res = 0;
	for (int i = 0; i < bonesGeometry.size(); i++) {
		if (boneName == bonesGeometry[i].name) {
			res = bonesGeometry[i].topRadius1;
			break;
		}
	}
	return res;
}

float GLWidget::getBoneBottomRadius(string boneName) {
	float res = 0;
	for (int i = 0; i < bonesGeometry.size(); i++) {
		if (boneName == bonesGeometry[i].name) {
			res = bonesGeometry[i].bottomRadius1;
			break;
		}
	}
	return res;
}

int GLWidget::getModelStateID(pf::Model3D *model, string boneName, pf::Vec3 vect) {
	int result;
	vector <string> usedBonesNames = model->getNamesMovingBones();
	for (int i = 0; i < usedBonesNames.size(); i++) {
		if (boneName == usedBonesNames[i]) {
			result = i + 3;
			break;
		}
	}
	if (vect == pf::Model3D::axisX)
		return result;
	else if (vect == pf::Model3D::axisY)
		return result + 1;
	else if (vect == pf::Model3D::axisZ)
		return result + 2;

	//vector<string>().swap(usedBonesNames);
}

void GLWidget::drawBackgroud(string img) {
	cv::Mat mat = cv::imread(img, CV_LOAD_IMAGE_COLOR);

	if (mat.empty()) {
		cout << "img empty" << endl;
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
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-w - (700 * ar), -h - (700 * ar), 1000);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(w + (700 * ar), -h - (700 * ar), 1000);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(w + (700 * ar), h + (700 * ar), 1000);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-w - (700 * ar), h + (700 * ar), 1000);
		glEnd();

		glDisable(GL_TEXTURE_2D);
		glFinish();

		tmp.release();
		glDeleteTextures(1, &tex);
	}
	mat.release();
}

void GLWidget::drawBackground(cv::Mat mat) {
	if (mat.empty()) {
		cout << "img empty" << endl;
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
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-w - (700 * ar), -h - (700 * ar), 1000);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(w + (700 * ar), -h - (700 * ar), 1000);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(w + (700 * ar), h + (700 * ar), 1000);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-w - (700 * ar), h + (700 * ar), 1000);
		glEnd();

		glDisable(GL_TEXTURE_2D);
		glFinish();

		tmp.release();
		glDeleteTextures(1, &tex);
	}
	mat.release();
}

void GLWidget::setLimitsVector(vector<pf::range2> &limits, vector<pf::boneConfig> bones, vector<pf::boneGeometry> bonesGeometry) {
	limits.clear();
	pf::range2 r;
	r.min = -360;
	r.max = 360;
	/*root ma 6x -360;360*/
	limits.push_back(r);
	limits.push_back(r);
	limits.push_back(r);
	limits.push_back(r);
	limits.push_back(r);
	limits.push_back(r);
	for (int j = 0; j < bonesGeometry.size(); j++) {
		for (int i = 0; i < bones.size(); i++) {
			if (bonesGeometry[j].name == bones[i].name) {
				//cout << bones[i].name << endl;

				pf::range2 rangeX, rangeY, rangeZ;
				rangeX.min = bones[i].minRotX;
				rangeX.max = bones[i].maxRotX;

				rangeY.min = bones[i].minRotY;
				rangeY.max = bones[i].maxRotY;

				rangeZ.min = bones[i].minRotZ;
				rangeZ.max = bones[i].maxRotZ;

				limits.push_back(rangeX);
				limits.push_back(rangeY);
				limits.push_back(rangeZ);

				//cout << rangeX.min << " " << rangeX.max << endl;
				//cout << rangeY.min << " " << rangeY.max << endl;
				//cout << rangeZ.min << " " << rangeZ.max << endl;
				break;
			}
		}
	}
}

void GLWidget::setVelocityVector(vector<float> &velocity, vector<pf::boneConfig> bones, vector<pf::boneGeometry> bonesGeometry) {
	velocity.clear();
	velocity.push_back(160);
	velocity.push_back(160);
	velocity.push_back(120);
	velocity.push_back(10);
	velocity.push_back(10);
	velocity.push_back(10);

	for (int j = 0; j < bonesGeometry.size(); j++) {
		for (int i = 0; i < bones.size(); i++) {
			if (bonesGeometry[j].name == bones[i].name) {
				//cout << bones[i].name << endl;

				float vX, vY, vZ;
				vX = bones[i].vRotX;
				vY = bones[i].vRotY;
				vZ = bones[i].vRotZ;

				velocity.push_back(vX);
				velocity.push_back(vY);
				velocity.push_back(vZ);

				break;
			}
		}
	}
}

void GLWidget::initializeBonesRotationsMap() {
	bonesRotations.clear();
	for (int i = 0; i < asfBones.size(); i++) {
		pf::Vec3f vec(0, 0, 0);
		bonesRotations[asfBones[i].name] = vec;
	}
}

void GLWidget::initializeBonesLengthMap() {
	bonesLength.clear();
	for (int i = 0; i < asfBones.size(); i++) {
		float length = asfBones[i].length;
		bonesLength[asfBones[i].name] = length;
	}
}

void GLWidget::initializeBonesVelocityMap() {
	modelVelocity.clear();
	for (int i = 0; i < asfBones.size(); i++) {
		string str = asfBones[i].name;
		vector<float> velocityTmp;
		if (str == "root") {
			velocityTmp.push_back(160);
			velocityTmp.push_back(160);
			velocityTmp.push_back(120);
			velocityTmp.push_back(10);
			velocityTmp.push_back(10);
			velocityTmp.push_back(10);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "LeftUpLeg_dum") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "RightUpLeg_dum") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "Spine_dum") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "LeftUpLeg") {
			velocityTmp.push_back(32);
			velocityTmp.push_back(0);
			velocityTmp.push_back(10);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "LeftLeg") {
			velocityTmp.push_back(25);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "LeftFoot") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "LeftToeBase") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "RightUpLeg") {
			velocityTmp.push_back(32);
			velocityTmp.push_back(0);
			velocityTmp.push_back(10);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "RightLeg") {
			velocityTmp.push_back(25);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "RightFoot") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "RightToeBase") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "Spine") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "Spine1") {
			velocityTmp.push_back(10);
			velocityTmp.push_back(10);
			velocityTmp.push_back(10);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "Spine1_dum1") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "Spine1_dum2") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "Spine1_dum3") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "Neck") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "Head") {
			velocityTmp.push_back(10);
			velocityTmp.push_back(10);
			velocityTmp.push_back(10);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "LeftShoulder") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "LeftArm") {
			velocityTmp.push_back(25);
			velocityTmp.push_back(15);
			velocityTmp.push_back(25);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "LeftForeArm") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(15);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "LeftHand") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "LeftHand_dum1") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "LeftHand_dum2") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "LeftHandThumb") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "RightShoulder") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "RightArm") {
			velocityTmp.push_back(25);
			velocityTmp.push_back(15);
			velocityTmp.push_back(25);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "RightForeArm") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(15);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "RightHand") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "RightHand_dum1") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "RightHand_dum2") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "RightHandThumb") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		//std::string().swap(str);
		//vector<int>().swap(velocityTmp);
	}
}

void GLWidget::initializeBonesLimitsMap(){
	modelLimits.clear();
	for (int i = 0; i < asfBones.size(); i++) {
		string str = asfBones[i].name;
		vector<int> limitsTMP;
		if (str == "root") {
			limitsTMP.push_back(-360);
			limitsTMP.push_back(360);
			limitsTMP.push_back(-360);
			limitsTMP.push_back(360);
			limitsTMP.push_back(-360);
			limitsTMP.push_back(360);
			limitsTMP.push_back(-360);
			limitsTMP.push_back(360);
			limitsTMP.push_back(-360);
			limitsTMP.push_back(360);
			limitsTMP.push_back(-360);
			limitsTMP.push_back(360);
			modelLimits[str] = limitsTMP;
		} else if (str == "LeftUpLeg_dum"){
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "RightUpLeg_dum") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "Spine_dum") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "LeftUpLeg") {
			limitsTMP.push_back(-95);
			limitsTMP.push_back(50);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(-35);
			limitsTMP.push_back(50);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "LeftLeg") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(140);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "LeftFoot") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "LeftToeBase") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "RightUpLeg") {
			limitsTMP.push_back(-95);
			limitsTMP.push_back(50);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(-50);
			limitsTMP.push_back(35);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "RightLeg") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(140);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "RightFoot") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "RightToeBase") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "Spine") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "Spine1") {
			limitsTMP.push_back(-15);
			limitsTMP.push_back(25);
			limitsTMP.push_back(-25);
			limitsTMP.push_back(25);
			limitsTMP.push_back(-25);
			limitsTMP.push_back(25);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "Spine1_dum1") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "Spine1_dum2") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "Spine1_dum3") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "Neck") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "Head") {
			limitsTMP.push_back(-50);
			limitsTMP.push_back(95);
			limitsTMP.push_back(-95);
			limitsTMP.push_back(95);
			limitsTMP.push_back(-50);
			limitsTMP.push_back(50);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "LeftShoulder") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "LeftArm") {
			limitsTMP.push_back(-150);
			limitsTMP.push_back(180);
			limitsTMP.push_back(-150);
			limitsTMP.push_back(180);
			limitsTMP.push_back(-150);
			limitsTMP.push_back(180);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "LeftForeArm") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(-140);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "LeftHand") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "LeftHand_dum1") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "LeftHand_dum2") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "LeftHandThumb") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "RightShoulder") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "RightArm") {
			limitsTMP.push_back(-180);
			limitsTMP.push_back(150);
			limitsTMP.push_back(-180);
			limitsTMP.push_back(150);
			limitsTMP.push_back(-180);
			limitsTMP.push_back(150);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "RightForeArm") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(140);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "RightHand") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "RightHand_dum1") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "RightHand_dum2") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "RightHandThumb") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else {
			limitsTMP.push_back(360);
			limitsTMP.push_back(360);
			limitsTMP.push_back(360);
			limitsTMP.push_back(360);
			limitsTMP.push_back(360);
			limitsTMP.push_back(360);
			modelLimits[str] = limitsTMP;
		}
		//std::string().swap(str);
		//vector<int>().swap(limitsTMP);
	}
}

void GLWidget::initializeBonesDOFMap() {
	modelDOF.clear();
	for (int i = 0; i < asfBones.size(); i++) {
		string str = asfBones[i].name;
		vector<bool> dofTMP;
		if (str == "root") {
			dofTMP.push_back(1);
			dofTMP.push_back(1);
			dofTMP.push_back(1);
			dofTMP.push_back(1);
			dofTMP.push_back(1);
			dofTMP.push_back(1);
			modelDOF[str] = dofTMP;
		}
		else if (str == "LeftUpLeg_dum") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "RightUpLeg_dum") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "Spine_dum") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "LeftUpLeg") {
			dofTMP.push_back(1);
			dofTMP.push_back(0);
			dofTMP.push_back(1);
			modelDOF[str] = dofTMP;
		}
		else if (str == "LeftLeg") {
			dofTMP.push_back(1);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "LeftFoot") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;;
		}
		else if (str == "LeftToeBase") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "RightUpLeg") {
			dofTMP.push_back(1);
			dofTMP.push_back(0);
			dofTMP.push_back(1);
			modelDOF[str] = dofTMP;
		}
		else if (str == "RightLeg") {
			dofTMP.push_back(1);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "RightFoot") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "RightToeBase") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "Spine") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "Spine1") {
			dofTMP.push_back(1);
			dofTMP.push_back(1);
			dofTMP.push_back(1);
			modelDOF[str] = dofTMP;
		}
		else if (str == "Spine1_dum1") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "Spine1_dum2") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "Spine1_dum3") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "Neck") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "Head") {
			dofTMP.push_back(1);
			dofTMP.push_back(1);
			dofTMP.push_back(1);
			modelDOF[str] = dofTMP;
		}
		else if (str == "LeftShoulder") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "LeftArm") {
			dofTMP.push_back(1);
			dofTMP.push_back(1);
			dofTMP.push_back(1);
			modelDOF[str] = dofTMP;
		}
		else if (str == "LeftForeArm") {
			dofTMP.push_back(0);
			dofTMP.push_back(1);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "LeftHand") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "LeftHand_dum1") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "LeftHand_dum2") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "LeftHandThumb") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "RightShoulder") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "RightArm") {
			dofTMP.push_back(1);
			dofTMP.push_back(1);
			dofTMP.push_back(1);
			modelDOF[str] = dofTMP;
		}
		else if (str == "RightForeArm") {
			dofTMP.push_back(0);
			dofTMP.push_back(1);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "RightHand") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "RightHand_dum1") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "RightHand_dum2") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "RightHandThumb") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		//vector<bool>().swap(dofTMP);
		//std::string(str).swap(str);
	}
}

void GLWidget::initializeBonesRadiusMap() {
	modelRadius.clear();
	for (int i = 0; i < asfBones.size(); i++) {
		vector<float> radiusTmp;
		bool found = false;
		string str = asfBones[i].name;
		for (int j = 0; j < bonesGeometry.size(); j++) {
			if (str == bonesGeometry[j].name) {
				radiusTmp.push_back(bonesGeometry[j].topRadius1);
				radiusTmp.push_back(bonesGeometry[j].topRadius2);
				radiusTmp.push_back(bonesGeometry[j].bottomRadius1);
				radiusTmp.push_back(bonesGeometry[j].bottomRadius2);
				found = true;
				break;
			}
		}
		if (!found) {
			radiusTmp.push_back(20);
			radiusTmp.push_back(20);
			radiusTmp.push_back(20);
			radiusTmp.push_back(20);
		}

		modelRadius[str] = radiusTmp;
	}
}

void GLWidget::saveModelStateToMap(map<string, pf::Vec3f> &bonesRotations, vector<vector<float>> mState, vector<pf::boneConfig> bonesConfig) {
	for (int i = 0; i < bonesConfig.size(); i++) {
		//std::map<char, int>::iterator it = bonesRotations.find(bonesConfig[i].name);
		int j = i + 1;
		pf::Vec3f vec(mState[0][j*3], mState[0][j*3+1], mState[0][j*3+2]);
		bonesRotations.at(bonesConfig[i].name) = vec;
	}

	for (auto it = bonesRotations.cbegin(); it != bonesRotations.cend(); ++it)
	{
		std::cout << it->first << " " << it->second.x() << " " << it->second.y() << " " << it->second.z() << "\n";
	}
}

void GLWidget::saveBonesLengthToMap(map<string, float> &bonesLength, vector<pf::boneGeometry> boneGeometry) {
	for (int i = 0; i < bonesGeometry.size(); i++) {
		bonesLength.at(bonesGeometry[i].name) = bonesGeometry[i].length;
	}
}

void GLWidget::saveRadiusToMap(map<string, vector<float>> &bonesRadius, vector<pf::boneGeometry> boneGeometry) {
	for (int i = 0; i < bonesGeometry.size(); i++) {
		vector<float> tmp;
		tmp.push_back(bonesGeometry[i].topRadius1);
		tmp.push_back(bonesGeometry[i].topRadius2);
		tmp.push_back(bonesGeometry[i].bottomRadius1);
		tmp.push_back(bonesGeometry[i].bottomRadius2);
		bonesRadius.at(bonesGeometry[i].name) = tmp;
	}
}

void GLWidget::saveDOFToMap(map<string, vector<bool>> &bonesDOF, vector<pf::boneConfig> boneConfig) {
	for (int i = 0; i < boneConfig.size(); i++) {
		//cout << boneConfig[i].name << " " << boneConfig[i].isRotX << boneConfig[i].isRotY << boneConfig[i].isRotZ << endl;
	}

	for (int i = 0; i < boneConfig.size(); i++) {
		vector<bool> tmp;
		if (boneConfig[i].isRotX)
			tmp.push_back(1);
		else tmp.push_back(0);

		if (boneConfig[i].isRotY)
			tmp.push_back(1);
		else tmp.push_back(0);

		if (boneConfig[i].isRotZ)
			tmp.push_back(1);
		else tmp.push_back(0);

		bonesDOF.at(boneConfig[i].name) = tmp;
		//vector<bool>().swap(tmp);
	}

	/*for (auto const& x : bonesDOF)
	{
		std::cout << x.first  // string (key)
			<< ':'
			<< x.second[0] << x.second[1] << x.second[2] << endl;// string's value 
	}
	cout << endl;*/
}

void GLWidget::saveLimitsToMap(map<string, vector<int>> modelLimits, vector<pf::boneConfig> boneConfig) {
	for (int i = 0; i < boneConfig.size(); i++) {
		vector<int> tmp;
		if (boneConfig[i].name == "root") {
			tmp.push_back(-360);
			tmp.push_back(360);
			tmp.push_back(-360);
			tmp.push_back(360);
			tmp.push_back(-360);
			tmp.push_back(360);
			tmp.push_back(-360);
			tmp.push_back(360);
			tmp.push_back(-360);
			tmp.push_back(360);
			tmp.push_back(-360);
			tmp.push_back(360);
			modelLimits.at(boneConfig[i].name) = tmp;
		}
		else {
			tmp.push_back(boneConfig[i].minRotX);
			tmp.push_back(boneConfig[i].maxRotX);
			tmp.push_back(boneConfig[i].minRotY);
			tmp.push_back(boneConfig[i].maxRotY);
			tmp.push_back(boneConfig[i].minRotZ);
			tmp.push_back(boneConfig[i].maxRotZ);
			modelLimits.at(boneConfig[i].name) = tmp;
		}
		//vector<int>().swap(tmp);
	}
}

void GLWidget::saveVelocityToMap(map<string, vector<float>> modelVelocity, vector<pf::boneConfig> boneConfig) {
	for (int i = 0; i < boneConfig.size(); i++) {
		vector<float> tmp;
		if (boneConfig[i].name == "root") {
			tmp.push_back(160);
			tmp.push_back(160);
			tmp.push_back(120);
			tmp.push_back(10);
			tmp.push_back(10);
			tmp.push_back(10);
			modelVelocity.at(boneConfig[i].name) = tmp;
		}
		else {
			tmp.push_back(boneConfig[i].vRotX);
			tmp.push_back(boneConfig[i].vRotY);
			tmp.push_back(boneConfig[i].vRotZ);
			modelVelocity.at(boneConfig[i].name) = tmp;
		}
		//vector<int>().swap(tmp);
	}
}

void GLWidget::updateModelStateFromMap(vector<vector<float>> &mState, map<string, pf::Vec3f> bonesRotations, vector<pf::boneConfig> bonesConfig) {
	mState.clear();
	vector<float> newModelState;

	newModelState.push_back(modelTranslation[0]);
	newModelState.push_back(modelTranslation[1]); // ############################# przesuniecie root
	newModelState.push_back(modelTranslation[2]);

	//cout << endl;

	for (int i = 0; i < bonesConfig.size(); i++) {
		pf::Vec3f vec = bonesRotations.at(bonesConfig[i].name);
		//cout << vec.x() << " " << vec.y() << " " << vec.z() << endl;
		newModelState.push_back(vec.x());
		newModelState.push_back(vec.y());
		newModelState.push_back(vec.z());
	}
	mState.push_back(newModelState);

	//vector<float>().swap(newModelState);
}

void GLWidget::updateBonesLengthFromMap(map<string, float> bonesLength) {
	for (int i = 0; i < bonesGeometry.size(); i++) 
		bonesGeometry[i].length = bonesLength.find(bonesGeometry[i].name)->second;
}

void GLWidget::updateBonesRadiusFromMap(map<string, vector<float>> bonesRadius) {
	for (int i = 0; i < bonesGeometry.size(); i++) {
		bonesGeometry[i].topRadius1 = bonesRadius.find(bonesGeometry[i].name)->second[0];
		bonesGeometry[i].topRadius2 = bonesRadius.find(bonesGeometry[i].name)->second[1];
		bonesGeometry[i].bottomRadius1 = bonesRadius.find(bonesGeometry[i].name)->second[2];
		bonesGeometry[i].bottomRadius2 = bonesRadius.find(bonesGeometry[i].name)->second[3];
	}
}

void GLWidget::updateBonesDOFMap(map<string, vector<bool>> &bonesDOF, string boneName, vector<bool> dofs) {
	bonesDOF.at(boneName) = dofs;	
}

void GLWidget::updateDOFFromMap(map<string, vector<bool>> bonesDOF, vector<pf::boneConfig> &bonesConfig) {
	for (int i = 0; i < bonesConfig.size(); i++) {
		vector<bool> dof;
		dof = bonesDOF.find(bonesConfig[i].name)->second;
		if (dof[0])
			bonesConfig[i].isRotX = true;
		else 
			bonesConfig[i].isRotX = false;

		if (dof[1])
			bonesConfig[i].isRotY = true;
		else
			bonesConfig[i].isRotY = false;

		if (dof[2])
			bonesConfig[i].isRotZ = true;
		else
			bonesConfig[i].isRotZ = false;

		//vector<bool>().swap(dof);
	}
}

void GLWidget::updateLimitsFromMap(map<string, vector<int>> modelLimits, vector<pf::boneConfig> &bonesConfig) {
	for (int i = 0; i < bonesConfig.size(); i++) {
		vector<int> limits = modelLimits.find(bonesConfig[i].name)->second;

		bonesConfig[i].minRotX = limits[0];
		bonesConfig[i].maxRotX = limits[1];
		bonesConfig[i].minRotY = limits[2];
		bonesConfig[i].maxRotY = limits[3];
		bonesConfig[i].minRotZ = limits[4];
		bonesConfig[i].maxRotZ = limits[5];
	}
}

void GLWidget::updateVelocityFromMap(map<string, vector<float>> modelVelocity, vector<pf::boneConfig> &bonesConfig) {
	for (int i = 0; i < bonesConfig.size(); i++) {
		vector<float> velocity = modelVelocity.find(bonesConfig[i].name)->second;

		bonesConfig[i].vRotX = velocity[0];
		bonesConfig[i].vRotY = velocity[1];
		bonesConfig[i].vRotZ = velocity[2];
	}
}

void GLWidget::updateUsedBones(vector<string> &usedBones, vector<pf::boneConfig> bonesConfig) {
	usedBones.clear();
	for (int i = 0; i < bonesConfig.size(); i++) {
		usedBones.push_back(bonesConfig[i].name);
	}
}

void GLWidget::loadFiles() {
	loadedImagesFolderPath = new QString(QFileDialog::getExistingDirectory(this, tr("Img folder")));
	if (!loadedImagesFolderPath->isEmpty()) {

		QDir dir(*loadedImagesFolderPath);
		QStringList filter;
		filter << QLatin1String("*.png");
		filter << QLatin1String("*.jpg");
		filter << QLatin1String("*.jpeg");
		filter << QLatin1String("*.bmp");
		dir.setNameFilters(filter);
		list = dir.entryInfoList();	

		isAvi = false;
	}
}

void GLWidget::loadAviFile() {
	aviFilePath = QFileDialog::getOpenFileName(this,
		tr("Wczytaj .avi"), "",
		tr("AVI (*.avi);;All Files(*)"));

	if (!aviFilePath.isEmpty()) {

		VideoCapture cap(aviFilePath.toUtf8().constData());
		cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
		cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
		AviHzDialog *aviDialog = new AviHzDialog(this);
		aviDialog->setFramesCounter(cap.get(CV_CAP_PROP_FRAME_COUNT));

		aviDialog->resize(200, 100);
		aviDialog->exec();

		hz = aviDialog->getHz();
		delete aviDialog;

		if (!cap.isOpened()) {
			QMessageBox::warning(NULL, QObject::tr("app"),
				QObject::tr("B³¹d w czasie otwierania pliku avi"),
				QMessageBox::Cancel,
				QMessageBox::Cancel);
		}
		else {
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
					cv::resize(frame, frame, cv::Size(640, 480), 0, 0, CV_INTER_LINEAR);
					cout << "." << endl;
					aviFrames.push_back(frame);
				}
				i++;
				//frame.release();
			}
			isAvi = true;
			cap.release();
		}
		
	}
}

void GLWidget::loadAviFile(string path, int hz) {
	VideoCapture cap(path);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	if (!cap.isOpened()) {
		QMessageBox::warning(NULL, QObject::tr("app"),
			QObject::tr("B³¹d w czasie otwierania pliku avi"),
			QMessageBox::Cancel,
			QMessageBox::Cancel);
	}
	else {
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
				cv::resize(frame, frame, cv::Size(640, 480), 0, 0, CV_INTER_LINEAR);
				cout << "." << endl;
				aviFrames.push_back(frame);
			}
			i++;
			//frame.release();
		}
		isAvi = true;
		cap.release();
	}

}

void GLWidget::copyConfigToGlWidget(GLWidget *&dest, GLWidget *source) {
	dest->limits = source->limits;
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

	delete fileHandler;
}