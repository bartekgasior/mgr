#include "modelindialog.h"

ModelInDialog::ModelInDialog(QWidget *parent)
	: QDialog(parent)
{
	QDialog *dialog = new QDialog();

	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	dialog->setWindowTitle("Manage model");

	gridLayout = new QGridLayout;
	okButton = new QDialogButtonBox(QDialogButtonBox::Cancel);

	//connect(okButton, &QDialogButtonBox::accepted, this, &ModelInDialog::okButtonAccepted);
	connect(okButton, &QDialogButtonBox::rejected, this, &QDialog::reject);

	//gridLayout->addWidget(, 0, 0, 20, 20);
	gridLayout->addWidget(okButton, 21, 9, 1, 2);
	setLayout(gridLayout);
}

ModelInDialog::~ModelInDialog()
{
	closed = true;
}

void ModelInDialog::setGLWidget(vector<vector<float>> modelState, pf::Model3D *model, map<string, int> idxBonesMap, vector<pf::boneGeometry> boneGeometry, vector<pf::boneConfig> boneConfig) {
	GLWidget *tmpGLWidget = new GLWidget();
	
	/*tmpGLWidget->isInMainWindow = false;
	tmpGLWidget->model = glWidget->model;
	tmpGLWidget->bonesConf = glWidget->bonesConf;
	tmpGLWidget->bonesGeometry = glWidget->bonesGeometry;
	tmpGLWidget->modelState = glWidget->modelState;
	tmpGLWidget->limits = glWidget->limits;
	tmpGLWidget->rotations = glWidget->rotations;
	tmpGLWidget->usedBones = glWidget->usedBones;
	tmpGLWidget->allBones = glWidget->allBones;
	tmpGLWidget->asfBones = glWidget->asfBones;
	tmpGLWidget->idxBonesMap = glWidget->idxBonesMap;
	tmpGLWidget->bonesRotations = glWidget->bonesRotations;*/
	tmpGLWidget->isInMainWindow = false;
	tmpGLWidget->model = model;
	tmpGLWidget->modelState = modelState;
	tmpGLWidget->idxBonesMap = idxBonesMap;
	tmpGLWidget->bonesConf = boneConfig;
	tmpGLWidget->bonesGeometry = boneGeometry;
	gridLayout->addWidget(tmpGLWidget, 0, 0, 20, 20);
}

void ModelInDialog::okButtonAccepted() {
	closed = true;
	close();
}

void ModelInDialog::okButtonRejected() {
	closed = true;
	close();
}