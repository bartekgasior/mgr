#include "modelindialog.h"

ModelInDialog::ModelInDialog(QWidget *parent)
	: QDialog(parent)
{
	QDialog *dialog = new QDialog();

	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	dialog->setWindowTitle("zarz¹dzaj modelem");

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
}

void ModelInDialog::setGLWidget(GLWidget *glWidget) {
	tmpGLWidget = new GLWidget();
	
	tmpGLWidget->isInMainWindow = false;
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
	tmpGLWidget->bonesRotations = glWidget->bonesRotations;

	gridLayout->addWidget(tmpGLWidget, 0, 0, 20, 20);
}

void ModelInDialog::okButtonAccepted() {
	close();
}

void ModelInDialog::okButtonRejected() {
	close();
}