#include "glwidget.h"
#include "asf.h"
#include "Model3D.h"
#include "gl/glu.h"
#include "SOIL.h"
#include "FileHandler.h"

using namespace std;

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
	usedBones = model->getNamesMovingBones();
	allBones = model->getNamesBones();
	pf::MotionData::loadASF(ASF_TEMPLATE_PATH, idxBonesMap, asfBones);
	model->loadConfig(DAT_TEMPLATE_PATH, bonesConf, bonesGeometry);
	initializeBonesRotationsMap();
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
	
	//drawBackgroud("C:/Users/barte/Desktop/badania.png");
	
	glPushMatrix();
	// draw skeleton model
	glLineWidth(5);
		this->drawSkeletonModel(vertices);
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
		cout << "min: " << limits[limitID].min << " max: " << limits[limitID].max << endl;
		if (limits[limitID].min <= angle && angle <= limits[limitID].max) 
			modelState[0][limitID] += rotVal*direction;

		cout << "angle: " << angle << " min: " << limits[limitID].min << " max: " << limits[limitID].max << endl;
	}
	if (vect == pf::Model3D::axisY) {

		angle = modelState[0][limitID+1] + rotVal*direction;
		if (limits[limitID+1].min <= angle && angle <= limits[limitID+1].max)
			modelState[0][limitID+1] += rotVal*direction;

		cout << "angle: " << angle << " min: " << limits[limitID+1].min << " max: " << limits[limitID+1].max << endl;
	}
	if (vect == pf::Model3D::axisZ) {

		angle = modelState[0][limitID+2] + rotVal*direction;
		if (limits[limitID+2].min <= angle && angle <= limits[limitID+2].max)
			modelState[0][limitID+2] += rotVal*direction;

		cout << "angle: " << angle << " min: " << limits[limitID+2].min << " max: " << limits[limitID+2].max << endl;
	}

	cout << modelState[0][limitID] << " " << modelState[0][limitID+1] << " " << modelState[0][limitID + 2] << endl;
	cout << endl;

	//cout << model->getNamesMovingBones().size() << endl;
	//model->updateModel();
	model->setModelState(modelState[0]);
	
	update();
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
}

void GLWidget::drawBackgroud(string img) {
	GLuint tex = SOIL_load_OGL_texture(img.c_str(), SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y);

	if (tex == 0)
		cout << "Texture Load Error: " << endl;

	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, tex);
	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-w - (700 * ar), -h - (700 * ar), 1000);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(w + (700 * ar), -h - (700 * ar), 1000);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(w + (700 * ar), h + (700 * ar), 1000);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-w - (700 * ar), h + (700 * ar), 1000);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glFinish();
}

void GLWidget::setLimitsVector(vector<pf::range2> &limits, vector<pf::boneConfig> bones) {
	limits.clear();
	pf::range2 r;
	r.min = -360;
	r.max = 360;
	limits.push_back(r);
	limits.push_back(r);
	limits.push_back(r);
	for (int i = 0; i < bones.size(); i++) {
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
	}
}

void GLWidget::initializeBonesRotationsMap() {
	bonesRotations.clear();
	for (int i = 0; i < asfBones.size(); i++) {
		pf::Vec3f vec(0, 0, 0);
		bonesRotations[asfBones[i].name] = vec;
	}
}

void GLWidget::saveModelStateToMap(map<string, pf::Vec3f> &bonesRotations, vector<vector<float>> mState, vector<pf::boneConfig> bonesConfig) {
	for (int i = 0; i < bonesConfig.size(); i++) {
		//std::map<char, int>::iterator it = bonesRotations.find(bonesConfig[i].name);
		int j = i + 1;
		pf::Vec3f vec(mState[0][j*3], mState[0][j*3+1], mState[0][j*3+2]);
		bonesRotations.at(bonesConfig[i].name) = vec;
	}
	
	/*for (auto it = bonesRotations.cbegin(); it != bonesRotations.cend(); ++it) {
		std::cout << it->first << " " << it->second.x() << " " << it->second.y() << " " << it->second.z() << endl;
	}*/

}

void GLWidget::updateModelStateFromMap(vector<vector<float>> &mState, map<string, pf::Vec3f> bonesRotations, vector<pf::boneConfig> bonesConfig) {
	mState.clear();
	vector<float> newModelState;

	newModelState.push_back(0);
	newModelState.push_back(0); // ############################# przesuniecie root
	newModelState.push_back(0);

	for (int i = 0; i < bonesConfig.size(); i++) {
		pf::Vec3f vec = bonesRotations.at(bonesConfig[i].name);
		newModelState.push_back(vec.x());
		newModelState.push_back(vec.y());
		newModelState.push_back(vec.z());
	}

	cout << newModelState.size() << endl;

	for (int i = 0; i < newModelState.size(); i++) {
		cout << newModelState[i] << endl;
	}

	mState.push_back(newModelState);
}