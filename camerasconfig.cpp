#include "camerasconfig.h"

CamerasConfig::CamerasConfig(QWidget *parent)
	: QDialog(parent)
{
	QDialog *dialog = new QDialog();
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	dialog->setWindowTitle("Cameras configuration");

	mainLayout = new QHBoxLayout();
	parametersLayout = new QHBoxLayout();

	cameraTypeComboBox = new QComboBox();
	cameraTypeComboBox->addItem(tr("TSAI1"));
	cameraTypeComboBox->addItem(tr("TSAI2"));

	cameraTypeComboBox->setFixedWidth(100);

	connect(cameraTypeComboBox, SIGNAL(activated(int)), this, SLOT(showCameraParameters()));

	mainLayout->addWidget(cameraTypeComboBox);
	mainLayout->addLayout(parametersLayout);

	setLayout(mainLayout);
}

CamerasConfig::~CamerasConfig()
{
}


void CamerasConfig::showCameraParameters() {
	if (cameraTypeComboBox->currentIndex() == 0) {
		qDeleteAll(hLayouts);
		hLayouts.clear();

	}
	else {
		qDeleteAll(hLayouts);
		hLayouts.clear();

	}
}

void CamerasConfig::addTsai1Layout() {

}

void CamerasConfig::okButtonAccepted() {
	close();
}

void CamerasConfig::okButtonRejected() {
	close();
}