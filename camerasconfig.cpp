#include "camerasconfig.h"

CamerasConfig::CamerasConfig(QWidget *parent)
	: QDialog(parent)
{
	QDialog *dialog = new QDialog();
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	dialog->setWindowTitle("Cameras configuration");

	prepareLayout();

	setLayout(mainLayout);

}

CamerasConfig::~CamerasConfig()
{
}

void CamerasConfig::prepareLayout() {
	okButton = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	connect(okButton, &QDialogButtonBox::accepted, this, &CamerasConfig::okButtonAccepted);
	connect(okButton, &QDialogButtonBox::rejected, this, &CamerasConfig::okButtonRejected);
	
	mainLayout = new QVBoxLayout();
	mainLayout->setAlignment(Qt::AlignTop);
	hBoxLayout = new QHBoxLayout();

	cameraComboBox = new QComboBox(this);
	connect(cameraComboBox, SIGNAL(activated(int)), this, SLOT(setCameraParametersToSpinBoxes()));

	cameraTypeComboBox = new QComboBox(this);
	//connect(cameraTypeComboBox, SIGNAL(activated(int)), this, SLOT(setCameraType()));
	addNewCamera = new QPushButton("Dodaj kamere");
	connect(addNewCamera, SIGNAL(released()), this, SLOT(buttonAddCameraPressed()));

	hBoxLayout->addWidget(cameraComboBox);
	hBoxLayout->addSpacing(250);
	hBoxLayout->addWidget(addNewCamera);

	cameraStackedWidget = new QStackedWidget(this);

	scrollTsai1Widget = new QWidget(this);
	scrollTsai2Widget = new QWidget(this);
	scrollPerspectiveWidget = new QWidget(this);
	scrollPerspectiveDisortionsWidget = new QWidget(this);
	scrollOtherParamsWidget = new QWidget(this);

	vboxTsai1Layout = new QVBoxLayout();
	vboxTsai2Layout = new QVBoxLayout();
	vboxPerspectiveLayout = new QVBoxLayout();
	vboxPerspectiveDisortionsLayout = new QVBoxLayout();
	vboxOtherParamsLayout = new QVBoxLayout();

	scrollTsai1Area = new QScrollArea();
	scrollTsai1Area->setWidgetResizable(true);
	scrollTsai1Widget->setLayout(vboxTsai1Layout);

	scrollTsai2Area = new QScrollArea();
	scrollTsai2Area->setWidgetResizable(true);
	scrollTsai2Widget->setLayout(vboxTsai2Layout);

	scrollPerspectiveArea = new QScrollArea();
	scrollPerspectiveArea->setWidgetResizable(true);
	scrollPerspectiveWidget->setLayout(vboxPerspectiveLayout);

	scrollPerspectiveDisortionsArea = new QScrollArea();
	scrollPerspectiveDisortionsArea->setWidgetResizable(true);
	scrollPerspectiveDisortionsWidget->setLayout(vboxPerspectiveDisortionsLayout);

	scrollOtherParamsArea = new QScrollArea();
	scrollOtherParamsArea->setWidgetResizable(true);
	scrollOtherParamsWidget->setLayout(vboxOtherParamsLayout);

	scrollTsai1Area->setWidget(scrollTsai1Widget);
	scrollTsai2Area->setWidget(scrollTsai2Widget);
	scrollPerspectiveArea->setWidget(scrollPerspectiveWidget);
	scrollPerspectiveDisortionsArea->setWidget(scrollPerspectiveDisortionsWidget);
	scrollOtherParamsArea->setWidget(scrollOtherParamsWidget);

	cameraComboBox->setFixedWidth(100);

	cameraStackedWidget->addWidget(scrollTsai1Area);
	cameraStackedWidget->addWidget(scrollTsai2Area);
	cameraStackedWidget->addWidget(scrollPerspectiveArea);
	cameraStackedWidget->addWidget(scrollPerspectiveDisortionsArea);

	addTsai1Tab();
	addTsai2Tab();
	addPerspectiveTab();
	addPerspectiveDisortionsTab();

	mainLayout->addLayout(hBoxLayout);
	mainLayout->addWidget(cameraTypeComboBox);
	mainLayout->addWidget(cameraStackedWidget);
	mainLayout->addWidget(okButton);

	if (cameras.size() == 0) {
		disableWidgets();
	}
}

void CamerasConfig::addTsai1Tab() {

	for (int i = 0; i < 10; i++)
		tsai1GroupBoxes.push_back(new QGroupBox(this));

	for (int i = 0; i < 2; i++)
		tsai1Labels.push_back(new QLabel(this));

	for (int i = 0; i < 12; i++) 
		tsai1HLayout.push_back(new QHBoxLayout());

	for (int i = 0; i < 19; i++)
		tsai1SpinBoxes.push_back(new QDoubleSpinBox(this));

	tsai1GroupBoxes[0]->setTitle("trans");
	tsai1GroupBoxes[1]->setTitle("rot");
	tsai1GroupBoxes[2]->setTitle("focal");
	tsai1GroupBoxes[3]->setTitle("kappa1");
	tsai1GroupBoxes[4]->setTitle("c");
	tsai1GroupBoxes[5]->setTitle("s");
	tsai1GroupBoxes[6]->setTitle("ncx");
	tsai1GroupBoxes[7]->setTitle("nfx");
	tsai1GroupBoxes[8]->setTitle("d");
	tsai1GroupBoxes[9]->setTitle("dp");

	tsai1Labels[0]->setText("width: ");
	tsai1Labels[1]->setText("height: ");

	for (int i = 0; i < 2; i++) {
		tsai1Labels[i]->setStyleSheet("font-weight: bold");
	}
	for (int i = 0; i < 2; i++) {
		tsai1SpinBoxes[i]->setFixedWidth(125);
		tsai1SpinBoxes[i]->setDecimals(0);
		tsai1SpinBoxes[i]->setMinimum(0);
		tsai1SpinBoxes[i]->setMaximum(11111111);
	}

	for (int i = 2; i < tsai1SpinBoxes.size(); i++) {
		tsai1SpinBoxes[i]->setFixedWidth(125);
		tsai1SpinBoxes[i]->setDecimals(6);
		tsai1SpinBoxes[i]->setMinimum(-11111111);
		tsai1SpinBoxes[i]->setMaximum(11111111);
	}

	for (int i = 0; i < tsai1HLayout.size(); i++) {
		tsai1HLayout[i]->setAlignment(Qt::AlignLeft);
	}

	/*width, height*/
	tsai1HLayout[0]->addWidget(tsai1Labels[0]);
	tsai1HLayout[0]->addWidget(tsai1SpinBoxes[0]);
	tsai1HLayout[1]->addWidget(tsai1Labels[1]);
	tsai1HLayout[1]->addWidget(tsai1SpinBoxes[1]);
	tsai1HLayout[1]->setContentsMargins(0, 0, 0, 10);

	/*tsai1*/
	/*trans*/
	tsai1HLayout[2]->addWidget(tsai1SpinBoxes[2]);
	tsai1SpinBoxes[2]->setPrefix("x: ");
	tsai1SpinBoxes[2]->setSuffix(" mm");
	tsai1HLayout[2]->addWidget(tsai1SpinBoxes[3]);
	tsai1SpinBoxes[3]->setPrefix("y: ");
	tsai1SpinBoxes[3]->setSuffix(" mm");
	tsai1HLayout[2]->addWidget(tsai1SpinBoxes[4]);
	tsai1SpinBoxes[4]->setPrefix("z: ");
	tsai1SpinBoxes[4]->setSuffix(" mm");
	tsai1HLayout[2]->setContentsMargins(10, 0, 10, 10);
	tsai1GroupBoxes[0]->setLayout(tsai1HLayout[2]);

	/*rot*/
	tsai1HLayout[3]->addWidget(tsai1SpinBoxes[5]);
	tsai1SpinBoxes[5]->setPrefix("x: ");
	tsai1SpinBoxes[5]->setSuffix(" deg");
	tsai1HLayout[3]->addWidget(tsai1SpinBoxes[6]);
	tsai1SpinBoxes[6]->setPrefix("y: ");
	tsai1SpinBoxes[6]->setSuffix(" deg");
	tsai1HLayout[3]->addWidget(tsai1SpinBoxes[7]);
	tsai1SpinBoxes[7]->setPrefix("z: ");
	tsai1SpinBoxes[7]->setSuffix(" deg");
	tsai1HLayout[3]->setContentsMargins(10, 0, 10, 10);
	tsai1GroupBoxes[1]->setLayout(tsai1HLayout[3]);

	/*focal*/
	tsai1HLayout[4]->addWidget(tsai1SpinBoxes[8]);
	tsai1HLayout[4]->setContentsMargins(10, 0, 10, 10);
	tsai1GroupBoxes[2]->setLayout(tsai1HLayout[4]);

	/*kappa1*/
	tsai1HLayout[5]->addWidget(tsai1SpinBoxes[9]);
	tsai1HLayout[5]->setContentsMargins(10, 0, 10, 10);
	tsai1GroupBoxes[3]->setLayout(tsai1HLayout[5]);

	/*c*/
	tsai1HLayout[6]->addWidget(tsai1SpinBoxes[10]);
	tsai1SpinBoxes[10]->setPrefix("x: ");
	tsai1SpinBoxes[10]->setSuffix(" mm");
	tsai1HLayout[6]->addWidget(tsai1SpinBoxes[11]);
	tsai1SpinBoxes[11]->setPrefix("y: ");
	tsai1SpinBoxes[11]->setSuffix(" mm");
	tsai1HLayout[6]->setContentsMargins(10, 0, 10, 10);
	tsai1GroupBoxes[4]->setLayout(tsai1HLayout[6]);

	/*sx*/
	tsai1HLayout[7]->addWidget(tsai1SpinBoxes[12]);
	tsai1HLayout[7]->setContentsMargins(10, 0, 10, 10);
	tsai1GroupBoxes[5]->setLayout(tsai1HLayout[7]);

	/*ncx*/
	tsai1HLayout[8]->addWidget(tsai1SpinBoxes[13]);
	tsai1HLayout[8]->setContentsMargins(10, 0, 10, 10);
	tsai1GroupBoxes[6]->setLayout(tsai1HLayout[8]);

	/*nfx*/
	tsai1HLayout[9]->addWidget(tsai1SpinBoxes[14]);
	tsai1HLayout[9]->setContentsMargins(10, 0, 10, 10);
	tsai1GroupBoxes[7]->setLayout(tsai1HLayout[9]);

	/*d*/
	tsai1HLayout[10]->addWidget(tsai1SpinBoxes[15]);
	tsai1SpinBoxes[15]->setPrefix("x: ");
	tsai1SpinBoxes[15]->setSuffix(" mm");
	tsai1HLayout[10]->addWidget(tsai1SpinBoxes[16]);
	tsai1SpinBoxes[16]->setPrefix("y: ");
	tsai1SpinBoxes[16]->setSuffix(" mm");
	tsai1HLayout[10]->setContentsMargins(10, 0, 10, 10);
	tsai1GroupBoxes[8]->setLayout(tsai1HLayout[10]);

	/*dp*/
	tsai1HLayout[11]->addWidget(tsai1SpinBoxes[17]);
	tsai1SpinBoxes[17]->setPrefix("x: ");
	tsai1SpinBoxes[17]->setSuffix(" mm");
	tsai1HLayout[11]->addWidget(tsai1SpinBoxes[18]);
	tsai1SpinBoxes[18]->setPrefix("y: ");
	tsai1SpinBoxes[18]->setSuffix(" mm");
	tsai1HLayout[11]->setContentsMargins(10, 0, 10, 10);
	tsai1GroupBoxes[9]->setLayout(tsai1HLayout[11]);

	for (int i = 0; i <2; i++)
		vboxTsai1Layout->addLayout(tsai1HLayout[i]);

	for (int i = 0; i < tsai1GroupBoxes.size(); i++) 
		vboxTsai1Layout->addWidget(tsai1GroupBoxes[i]);

	for (int i = 0; i<tsai1SpinBoxes.size(); i++)
		connect(tsai1SpinBoxes[i], SIGNAL(valueChanged(double)), this, SLOT(saveCameraParametersFromSB()));
}

void CamerasConfig::addTsai2Tab() {
	for (int i = 0; i < 9; i++)
		tsai2GroupBoxes.push_back(new QGroupBox(this));

	for (int i = 0; i < 2; i++)
		tsai2Labels.push_back(new QLabel(this));

	for (int i = 0; i < 11; i++)
		tsai2HLayout.push_back(new QHBoxLayout());

	for (int i = 0; i < 16; i++)
		tsai2SpinBoxes.push_back(new QDoubleSpinBox(this));

	tsai2GroupBoxes[0]->setTitle("trans");
	tsai2GroupBoxes[1]->setTitle("rot");
	tsai2GroupBoxes[2]->setTitle("focal1");
	tsai2GroupBoxes[3]->setTitle("focal2");
	tsai2GroupBoxes[4]->setTitle("k1");
	tsai2GroupBoxes[5]->setTitle("k2");
	tsai2GroupBoxes[6]->setTitle("p1");
	tsai2GroupBoxes[7]->setTitle("p2");
	tsai2GroupBoxes[8]->setTitle("c");

	tsai2Labels[0]->setText("width: ");
	tsai2Labels[1]->setText("height: ");

	for (int i = 0; i < 2; i++) {
		tsai2Labels[i]->setStyleSheet("font-weight: bold");
	}
	for (int i = 0; i < 2; i++) {
		tsai2SpinBoxes[i]->setFixedWidth(125);
		tsai2SpinBoxes[i]->setDecimals(0);
		tsai2SpinBoxes[i]->setMinimum(0);
		tsai2SpinBoxes[i]->setMaximum(11111111);
	}

	for (int i = 2; i < tsai2SpinBoxes.size(); i++) {
		tsai2SpinBoxes[i]->setFixedWidth(125);
		tsai2SpinBoxes[i]->setDecimals(6);
		tsai2SpinBoxes[i]->setMinimum(-11111111);
		tsai2SpinBoxes[i]->setMaximum(11111111);
	}

	for (int i = 0; i < tsai2HLayout.size(); i++) {
		tsai2HLayout[i]->setAlignment(Qt::AlignLeft);
	}

	/*wspolne*/
	/*width, height*/
	tsai2HLayout[0]->addWidget(tsai2Labels[0]);
	tsai2HLayout[0]->addWidget(tsai2SpinBoxes[0]);
	tsai2HLayout[1]->addWidget(tsai2Labels[1]);
	tsai2HLayout[1]->addWidget(tsai2SpinBoxes[1]);
	tsai2HLayout[1]->setContentsMargins(0, 0, 0, 10);

	/*tsai2*/
	/*trans*/
	tsai2HLayout[2]->addWidget(tsai2SpinBoxes[2]);
	tsai2SpinBoxes[2]->setPrefix("x: ");
	tsai2SpinBoxes[2]->setSuffix(" mm");
	tsai2HLayout[2]->addWidget(tsai2SpinBoxes[3]);
	tsai2SpinBoxes[3]->setPrefix("y: ");
	tsai2SpinBoxes[3]->setSuffix(" mm");
	tsai2HLayout[2]->addWidget(tsai2SpinBoxes[4]);
	tsai2SpinBoxes[4]->setPrefix("z: ");
	tsai2SpinBoxes[4]->setSuffix(" mm");
	tsai2HLayout[2]->setContentsMargins(10, 0, 10, 10);
	tsai2GroupBoxes[0]->setLayout(tsai2HLayout[2]);

	/*rot*/
	tsai2HLayout[3]->addWidget(tsai2SpinBoxes[5]);
	tsai2SpinBoxes[5]->setPrefix("x: ");
	tsai2SpinBoxes[5]->setSuffix(" deg");
	tsai2HLayout[3]->addWidget(tsai2SpinBoxes[6]);
	tsai2SpinBoxes[6]->setPrefix("y: ");
	tsai2SpinBoxes[6]->setSuffix(" deg");
	tsai2HLayout[3]->addWidget(tsai2SpinBoxes[7]);
	tsai2SpinBoxes[7]->setPrefix("z: ");
	tsai2SpinBoxes[7]->setSuffix(" deg");
	tsai2HLayout[3]->setContentsMargins(10, 0, 10, 10);
	tsai2GroupBoxes[1]->setLayout(tsai2HLayout[3]);

	/*focal1*/
	tsai2HLayout[4]->addWidget(tsai2SpinBoxes[8]);
	tsai2HLayout[4]->setContentsMargins(10, 0, 10, 10);
	tsai2GroupBoxes[2]->setLayout(tsai2HLayout[4]);

	/*focal2*/
	tsai2HLayout[5]->addWidget(tsai2SpinBoxes[9]);
	tsai2HLayout[5]->setContentsMargins(10, 0, 10, 10);
	tsai2GroupBoxes[3]->setLayout(tsai2HLayout[5]);

	/*k1*/
	tsai2HLayout[6]->addWidget(tsai2SpinBoxes[10]);
	tsai2HLayout[6]->setContentsMargins(10, 0, 10, 10);
	tsai2GroupBoxes[4]->setLayout(tsai2HLayout[6]);

	/*k2*/
	tsai2HLayout[7]->addWidget(tsai2SpinBoxes[11]);
	tsai2HLayout[7]->setContentsMargins(10, 0, 10, 10);
	tsai2GroupBoxes[5]->setLayout(tsai2HLayout[7]);

	/*p1*/
	tsai2HLayout[8]->addWidget(tsai2SpinBoxes[12]);
	tsai2HLayout[8]->setContentsMargins(10, 0, 10, 10);
	tsai2GroupBoxes[6]->setLayout(tsai2HLayout[8]);

	/*p2*/
	tsai2HLayout[9]->addWidget(tsai2SpinBoxes[13]);
	tsai2HLayout[9]->setContentsMargins(10, 0, 10, 10);
	tsai2GroupBoxes[7]->setLayout(tsai2HLayout[9]);

	/*c*/
	tsai2HLayout[10]->addWidget(tsai2SpinBoxes[14]);
	tsai2SpinBoxes[14]->setPrefix("x: ");
	tsai2SpinBoxes[14]->setSuffix(" mm");
	tsai2HLayout[10]->addWidget(tsai2SpinBoxes[15]);
	tsai2SpinBoxes[15]->setPrefix("y: ");
	tsai2SpinBoxes[15]->setSuffix(" mm");
	tsai2HLayout[10]->setContentsMargins(10, 0, 10, 10);
	tsai2GroupBoxes[8]->setLayout(tsai2HLayout[10]);

	for (int i = 0; i < 2; i++)
		vboxTsai2Layout->addLayout(tsai2HLayout[i]);

	for (int i = 0; i < tsai2GroupBoxes.size(); i++)
		vboxTsai2Layout->addWidget(tsai2GroupBoxes[i]);

	for (int i = 0; i < tsai2SpinBoxes.size(); i++)
		connect(tsai2SpinBoxes[i], SIGNAL(valueChanged(double)), this, SLOT(saveCameraParametersFromSB()));
}

void CamerasConfig::addPerspectiveTab() {

	perspectiveQuatCB = new QCheckBox(this);
	perspectiveQuatCB->setChecked(true);
	perspectiveRadCB = new QCheckBox(this);
	perspectiveRadCB->setChecked(false);

	connect(perspectiveQuatCB, SIGNAL(stateChanged(int)), this, SLOT(hidePerspectiveQuatLayout()));
	connect(perspectiveRadCB, SIGNAL(stateChanged(int)), this, SLOT(hidePerspectiveRadLayout()));
	
	//hidePerspectiveQuatLayout();

	for (int i = 0; i < 3; i++)
		perspectiveGroupBoxes.push_back(new QGroupBox(this));

	for (int i = 0; i < 4; i++)
		perspectiveLabels.push_back(new QLabel(this));

	for (int i = 0; i < 8; i++)
		perspectiveHLayout.push_back(new QHBoxLayout());

	for (int i = 0; i < 13; i++)
		perspectiveSpinBoxes.push_back(new QDoubleSpinBox(this));

	perspectiveGroupBoxes[0]->setTitle("position");
	perspectiveGroupBoxes[1]->setTitle("rotation");
	perspectiveGroupBoxes[2]->setTitle("fovy");

	perspectiveLabels[0]->setText("width: ");
	perspectiveLabels[1]->setText("height: ");
	perspectiveLabels[2]->setText("quaternions");
	perspectiveLabels[3]->setText("angles");

	for (int i = 0; i < 2; i++) {
		perspectiveLabels[i]->setStyleSheet("font-weight: bold");
	}
	for (int i = 0; i < 2; i++) {
		perspectiveSpinBoxes[i]->setFixedWidth(125);
		perspectiveSpinBoxes[i]->setDecimals(1);
		perspectiveSpinBoxes[i]->setMinimum(0);
		perspectiveSpinBoxes[i]->setMaximum(1111111);
	}

	for (int i = 2; i < perspectiveSpinBoxes.size(); i++) {
		perspectiveSpinBoxes[i]->setFixedWidth(125);
		perspectiveSpinBoxes[i]->setDecimals(6);
		perspectiveSpinBoxes[i]->setMinimum(-11111111);
		perspectiveSpinBoxes[i]->setMaximum(11111111);
	}

	for (int i = 0; i < perspectiveHLayout.size(); i++) {
		perspectiveHLayout[i]->setAlignment(Qt::AlignLeft);
	}

	/*wspolne*/
	/*width, height*/
	perspectiveHLayout[0]->addWidget(perspectiveLabels[0]);
	perspectiveHLayout[0]->addWidget(perspectiveSpinBoxes[0]);
	perspectiveHLayout[1]->addWidget(perspectiveLabels[1]);
	perspectiveHLayout[1]->addWidget(perspectiveSpinBoxes[1]);
	perspectiveHLayout[1]->setContentsMargins(0, 0, 0, 10);

	/*position*/
	perspectiveHLayout[2]->addWidget(perspectiveSpinBoxes[2]);
	perspectiveSpinBoxes[2]->setPrefix("x: ");
	perspectiveSpinBoxes[2]->setSuffix(" mm");
	perspectiveHLayout[2]->addWidget(perspectiveSpinBoxes[3]);
	perspectiveSpinBoxes[3]->setPrefix("y: ");
	perspectiveSpinBoxes[3]->setSuffix(" mm");
	perspectiveHLayout[2]->addWidget(perspectiveSpinBoxes[4]);
	perspectiveSpinBoxes[4]->setPrefix("z: ");
	perspectiveSpinBoxes[4]->setSuffix(" mm");
	perspectiveHLayout[2]->setContentsMargins(10, 0, 10, 10);

	perspectiveGroupBoxes[0]->setLayout(perspectiveHLayout[2]);

	/*rotation*/
	QVBoxLayout *rotVBox = new QVBoxLayout;
	perspectiveHLayout[3]->addWidget(perspectiveLabels[2]);
	perspectiveHLayout[3]->addWidget(perspectiveQuatCB);
	perspectiveHLayout[4]->addWidget(perspectiveSpinBoxes[5]);
	perspectiveSpinBoxes[5]->setPrefix("x: ");
	perspectiveHLayout[4]->addWidget(perspectiveSpinBoxes[6]);
	perspectiveSpinBoxes[6]->setPrefix("y: ");
	perspectiveHLayout[4]->addWidget(perspectiveSpinBoxes[7]);
	perspectiveSpinBoxes[7]->setPrefix("z: ");
	perspectiveHLayout[4]->addWidget(perspectiveSpinBoxes[8]);
	perspectiveSpinBoxes[8]->setPrefix("w: ");
	rotVBox->addLayout(perspectiveHLayout[3]);
	rotVBox->addLayout(perspectiveHLayout[4]);

	perspectiveHLayout[5]->addWidget(perspectiveLabels[3]);
	perspectiveHLayout[5]->addWidget(perspectiveRadCB);
	perspectiveHLayout[6]->addWidget(perspectiveSpinBoxes[9]);
	perspectiveSpinBoxes[9]->setPrefix("x: ");
	perspectiveSpinBoxes[9]->setSuffix(" deg");
	perspectiveHLayout[6]->addWidget(perspectiveSpinBoxes[10]);
	perspectiveSpinBoxes[10]->setPrefix("y: ");
	perspectiveSpinBoxes[10]->setSuffix(" deg");
	perspectiveHLayout[6]->addWidget(perspectiveSpinBoxes[11]);
	perspectiveSpinBoxes[11]->setPrefix("z: ");
	perspectiveSpinBoxes[11]->setSuffix(" deg");
	rotVBox->addLayout(perspectiveHLayout[5]);
	rotVBox->addLayout(perspectiveHLayout[6]);

	perspectiveGroupBoxes[1]->setLayout(rotVBox);

	/*perspective*/
	/*fovy*/
	perspectiveHLayout[7]->addWidget(perspectiveSpinBoxes[12]);
	perspectiveHLayout[7]->setContentsMargins(10, 0, 10, 10);
	perspectiveGroupBoxes[2]->setLayout(perspectiveHLayout[7]);

	for (int i = 0; i < 2; i++)
		vboxPerspectiveLayout->addLayout(perspectiveHLayout[i]);

	for (int i = 0; i < perspectiveGroupBoxes.size(); i++)
		vboxPerspectiveLayout->addWidget(perspectiveGroupBoxes[i]);

	for (int i = 0; i < perspectiveSpinBoxes.size(); i++)
		connect(perspectiveSpinBoxes[i], SIGNAL(valueChanged(double)), this, SLOT(saveCameraParametersFromSB()));

	hidePerspectiveQuatLayout();
}

void CamerasConfig::addPerspectiveDisortionsTab() {
	perspectiveDisortionQuatCB = new QCheckBox(this);
	perspectiveDisortionQuatCB->setChecked(true);
	perspectiveDisortionRadCB = new QCheckBox(this);
	perspectiveDisortionRadCB->setChecked(false);

	connect(perspectiveDisortionQuatCB, SIGNAL(stateChanged(int)), this, SLOT(hidePerspectiveDisortionQuatLayout()));
	connect(perspectiveDisortionRadCB, SIGNAL(stateChanged(int)), this, SLOT(hidePerspectiveDisortionRadLayout()));

	for (int i = 0; i < 7; i++)
		perspectiveDisortionsGroupBoxes.push_back(new QGroupBox(this));

	for (int i = 0; i < 4; i++)
		perspectiveDisortionsLabels.push_back(new QLabel(this));

	for (int i = 0; i < 12; i++)
		perspectiveDisortionsHLayout.push_back(new QHBoxLayout());

	for (int i = 0; i < 17; i++)
		perspectiveDisortionsSpinBoxes.push_back(new QDoubleSpinBox(this));

	perspectiveDisortionsGroupBoxes[0]->setTitle("position");
	perspectiveDisortionsGroupBoxes[1]->setTitle("rotation");
	perspectiveDisortionsGroupBoxes[2]->setTitle("fovy");
	perspectiveDisortionsGroupBoxes[3]->setTitle("k1");
	perspectiveDisortionsGroupBoxes[4]->setTitle("k2");
	perspectiveDisortionsGroupBoxes[5]->setTitle("p1");
	perspectiveDisortionsGroupBoxes[6]->setTitle("p2");

	perspectiveDisortionsLabels[0]->setText("width: ");
	perspectiveDisortionsLabels[1]->setText("height: ");
	perspectiveDisortionsLabels[2]->setText("quaternions");
	perspectiveDisortionsLabels[3]->setText("angles");

	for (int i = 0; i < 2; i++) {
		perspectiveDisortionsLabels[i]->setStyleSheet("font-weight: bold");
	}
	for (int i = 0; i < 2; i++) {
		perspectiveDisortionsSpinBoxes[i]->setFixedWidth(125);
		perspectiveDisortionsSpinBoxes[i]->setDecimals(0);
		perspectiveDisortionsSpinBoxes[i]->setMinimum(0);
		perspectiveDisortionsSpinBoxes[i]->setMaximum(11111111);
	}

	for (int i = 2; i < perspectiveDisortionsSpinBoxes.size(); i++) {
		perspectiveDisortionsSpinBoxes[i]->setFixedWidth(125);
		perspectiveDisortionsSpinBoxes[i]->setDecimals(6);
		perspectiveDisortionsSpinBoxes[i]->setMinimum(-11111111);
		perspectiveDisortionsSpinBoxes[i]->setMaximum(11111111);
	}

	for (int i = 0; i < perspectiveDisortionsHLayout.size(); i++) {
		perspectiveDisortionsHLayout[i]->setAlignment(Qt::AlignLeft);
	}

	/*wspolne*/
	/*width, height*/
	perspectiveDisortionsHLayout[0]->addWidget(perspectiveDisortionsLabels[0]);
	perspectiveDisortionsHLayout[0]->addWidget(perspectiveDisortionsSpinBoxes[0]);
	perspectiveDisortionsHLayout[1]->addWidget(perspectiveDisortionsLabels[1]);
	perspectiveDisortionsHLayout[1]->addWidget(perspectiveDisortionsSpinBoxes[1]);
	perspectiveDisortionsHLayout[1]->setContentsMargins(0, 0, 0, 10);

	/*position*/
	perspectiveDisortionsHLayout[2]->addWidget(perspectiveDisortionsSpinBoxes[2]);
	perspectiveDisortionsSpinBoxes[2]->setPrefix("x: ");
	perspectiveDisortionsSpinBoxes[2]->setSuffix(" mm");
	perspectiveDisortionsHLayout[2]->addWidget(perspectiveDisortionsSpinBoxes[3]);
	perspectiveDisortionsSpinBoxes[3]->setPrefix("y: ");
	perspectiveDisortionsSpinBoxes[3]->setSuffix(" mm");
	perspectiveDisortionsHLayout[2]->addWidget(perspectiveDisortionsSpinBoxes[4]);
	perspectiveDisortionsSpinBoxes[4]->setPrefix("z: ");
	perspectiveDisortionsSpinBoxes[4]->setSuffix(" mm");
	perspectiveDisortionsHLayout[2]->setContentsMargins(10, 0, 10, 10);

	perspectiveDisortionsGroupBoxes[0]->setLayout(perspectiveDisortionsHLayout[2]);

	/*rotation*/
	QVBoxLayout *rotVBox = new QVBoxLayout;
	perspectiveDisortionsHLayout[3]->addWidget(perspectiveDisortionsLabels[2]);
	perspectiveDisortionsHLayout[3]->addWidget(perspectiveDisortionQuatCB);
	perspectiveDisortionsHLayout[4]->addWidget(perspectiveDisortionsSpinBoxes[5]);
	perspectiveDisortionsSpinBoxes[5]->setPrefix("x: ");
	perspectiveDisortionsHLayout[4]->addWidget(perspectiveDisortionsSpinBoxes[6]);
	perspectiveDisortionsSpinBoxes[6]->setPrefix("y: ");
	perspectiveDisortionsHLayout[4]->addWidget(perspectiveDisortionsSpinBoxes[7]);
	perspectiveDisortionsSpinBoxes[7]->setPrefix("z: ");
	perspectiveDisortionsHLayout[4]->addWidget(perspectiveDisortionsSpinBoxes[8]);
	perspectiveDisortionsSpinBoxes[8]->setPrefix("w: ");
	rotVBox->addLayout(perspectiveDisortionsHLayout[3]);
	rotVBox->addLayout(perspectiveDisortionsHLayout[4]);

	perspectiveDisortionsHLayout[5]->addWidget(perspectiveDisortionsLabels[3]);
	perspectiveDisortionsHLayout[5]->addWidget(perspectiveDisortionRadCB);
	perspectiveDisortionsHLayout[6]->addWidget(perspectiveDisortionsSpinBoxes[9]);
	perspectiveDisortionsSpinBoxes[9]->setPrefix("x: ");
	perspectiveDisortionsSpinBoxes[9]->setSuffix(" deg");
	perspectiveDisortionsHLayout[6]->addWidget(perspectiveDisortionsSpinBoxes[10]);
	perspectiveDisortionsSpinBoxes[10]->setPrefix("y: ");
	perspectiveDisortionsSpinBoxes[10]->setSuffix(" deg");
	perspectiveDisortionsHLayout[6]->addWidget(perspectiveDisortionsSpinBoxes[11]);
	perspectiveDisortionsSpinBoxes[11]->setPrefix("z: ");
	perspectiveDisortionsSpinBoxes[11]->setSuffix(" deg");
	rotVBox->addLayout(perspectiveDisortionsHLayout[5]);
	rotVBox->addLayout(perspectiveDisortionsHLayout[6]);

	perspectiveDisortionsGroupBoxes[1]->setLayout(rotVBox);

	/*perspectiveDisortion*/
	/*fovy*/
	perspectiveDisortionsHLayout[7]->addWidget(perspectiveDisortionsSpinBoxes[12]);
	perspectiveDisortionsHLayout[7]->setContentsMargins(10, 0, 10, 10);
	perspectiveDisortionsGroupBoxes[2]->setLayout(perspectiveDisortionsHLayout[7]);

	/*k1*/
	perspectiveDisortionsHLayout[8]->addWidget(perspectiveDisortionsSpinBoxes[13]);
	perspectiveDisortionsHLayout[8]->setContentsMargins(10, 0, 10, 10);
	perspectiveDisortionsGroupBoxes[3]->setLayout(perspectiveDisortionsHLayout[8]);

	/*k2*/
	perspectiveDisortionsHLayout[9]->addWidget(perspectiveDisortionsSpinBoxes[14]);
	perspectiveDisortionsHLayout[9]->setContentsMargins(10, 0, 10, 10);
	perspectiveDisortionsGroupBoxes[4]->setLayout(perspectiveDisortionsHLayout[9]);

	/*p1*/
	perspectiveDisortionsHLayout[10]->addWidget(perspectiveDisortionsSpinBoxes[15]);
	perspectiveDisortionsHLayout[10]->setContentsMargins(10, 0, 10, 10);
	perspectiveDisortionsGroupBoxes[5]->setLayout(perspectiveDisortionsHLayout[10]);

	/*p2*/
	perspectiveDisortionsHLayout[11]->addWidget(perspectiveDisortionsSpinBoxes[16]);
	perspectiveDisortionsHLayout[11]->setContentsMargins(10, 0, 10, 10);
	perspectiveDisortionsGroupBoxes[6]->setLayout(perspectiveDisortionsHLayout[11]);

	for (int i = 0; i < 2; i++)
		vboxPerspectiveDisortionsLayout->addLayout(perspectiveDisortionsHLayout[i]);

	for (int i = 0; i < perspectiveDisortionsGroupBoxes.size(); i++)
		vboxPerspectiveDisortionsLayout->addWidget(perspectiveDisortionsGroupBoxes[i]);

	for (int i = 0; i < perspectiveDisortionsSpinBoxes.size(); i++)
		connect(perspectiveDisortionsSpinBoxes[i], SIGNAL(valueChanged(double)), this, SLOT(saveCameraParametersFromSB()));

	hidePerspectiveDisortionQuatLayout();
}

void CamerasConfig::setCameraParametersToSpinBoxes() {
	int cameraID = cameraComboBox->currentIndex();
	int cameraTypeID = cameraType[cameraID];
	cameraStackedWidget->setCurrentIndex(cameraTypeID);

	float w, h;
	vector<double> tsai1Params, tsai2Params, perspectiveParams, perspectiveDisortionParams;

	w = cameras[cameraID]->getWidth();
	h = cameras[cameraID]->getHeight();
	if (cameraTypeID == 0) {

		tsai1Params.push_back(cameras[cameraID]->getTSAI1Param().trans.x);
		tsai1Params.push_back(cameras[cameraID]->getTSAI1Param().trans.y);
		tsai1Params.push_back(cameras[cameraID]->getTSAI1Param().trans.z);
		tsai1Params.push_back(cameras[cameraID]->getTSAI1Param().rot.x);
		tsai1Params.push_back(cameras[cameraID]->getTSAI1Param().rot.y);
		tsai1Params.push_back(cameras[cameraID]->getTSAI1Param().rot.z);
		tsai1Params.push_back(cameras[cameraID]->getTSAI1Param().focal);
		tsai1Params.push_back(cameras[cameraID]->getTSAI1Param().kappa1);
		tsai1Params.push_back(cameras[cameraID]->getTSAI1Param().c.x);
		tsai1Params.push_back(cameras[cameraID]->getTSAI1Param().c.y);
		tsai1Params.push_back(cameras[cameraID]->getTSAI1Param().sx);
		tsai1Params.push_back(cameras[cameraID]->getTSAI1Param().ncx);
		tsai1Params.push_back(cameras[cameraID]->getTSAI1Param().nfx);
		tsai1Params.push_back(cameras[cameraID]->getTSAI1Param().d.x);
		tsai1Params.push_back(cameras[cameraID]->getTSAI1Param().d.y);
		tsai1Params.push_back(cameras[cameraID]->getTSAI1Param().dp.x);
		tsai1Params.push_back(cameras[cameraID]->getTSAI1Param().dp.y);

		tsai1SpinBoxes[0]->setValue(w);
		tsai1SpinBoxes[1]->setValue(h);
		tsai1SpinBoxes[2]->setValue(tsai1Params[0]);
		tsai1SpinBoxes[3]->setValue(tsai1Params[1]);
		tsai1SpinBoxes[4]->setValue(tsai1Params[2]);
		tsai1SpinBoxes[5]->setValue(tsai1Params[3]);
		tsai1SpinBoxes[6]->setValue(tsai1Params[4]);
		tsai1SpinBoxes[7]->setValue(tsai1Params[5]);
		tsai1SpinBoxes[8]->setValue(tsai1Params[6]);
		tsai1SpinBoxes[9]->setValue(tsai1Params[7]);
		tsai1SpinBoxes[10]->setValue(tsai1Params[8]);
		tsai1SpinBoxes[11]->setValue(tsai1Params[9]);
		tsai1SpinBoxes[12]->setValue(tsai1Params[10]);
		tsai1SpinBoxes[13]->setValue(tsai1Params[11]);
		tsai1SpinBoxes[14]->setValue(tsai1Params[12]);
		tsai1SpinBoxes[15]->setValue(tsai1Params[13]);
		tsai1SpinBoxes[16]->setValue(tsai1Params[14]);
		tsai1SpinBoxes[17]->setValue(tsai1Params[15]);
		tsai1SpinBoxes[18]->setValue(tsai1Params[16]);
	}
	else if (cameraTypeID == 1) {
		tsai2Params.push_back(cameras[cameraID]->getTSAI2Param().trans.x);
		tsai2Params.push_back(cameras[cameraID]->getTSAI2Param().trans.y);
		tsai2Params.push_back(cameras[cameraID]->getTSAI2Param().trans.z);
		tsai2Params.push_back(cameras[cameraID]->getTSAI2Param().rot.x);
		tsai2Params.push_back(cameras[cameraID]->getTSAI2Param().rot.y);
		tsai2Params.push_back(cameras[cameraID]->getTSAI2Param().rot.z);
		tsai2Params.push_back(cameras[cameraID]->getTSAI2Param().focal1);
		tsai2Params.push_back(cameras[cameraID]->getTSAI2Param().focal2);
		tsai2Params.push_back(cameras[cameraID]->getTSAI2Param().K1);
		tsai2Params.push_back(cameras[cameraID]->getTSAI2Param().K2);
		tsai2Params.push_back(cameras[cameraID]->getTSAI2Param().P1);
		tsai2Params.push_back(cameras[cameraID]->getTSAI2Param().P2);
		tsai2Params.push_back(cameras[cameraID]->getTSAI2Param().c.x);
		tsai2Params.push_back(cameras[cameraID]->getTSAI2Param().c.y);

		tsai2SpinBoxes[0]->setValue(w);
		tsai2SpinBoxes[1]->setValue(h);
		tsai2SpinBoxes[2]->setValue(tsai2Params[0]);
		tsai2SpinBoxes[3]->setValue(tsai2Params[1]);
		tsai2SpinBoxes[4]->setValue(tsai2Params[2]);
		tsai2SpinBoxes[5]->setValue(tsai2Params[3]);
		tsai2SpinBoxes[6]->setValue(tsai2Params[4]);
		tsai2SpinBoxes[7]->setValue(tsai2Params[5]);
		tsai2SpinBoxes[8]->setValue(tsai2Params[6]);
		tsai2SpinBoxes[9]->setValue(tsai2Params[7]);
		tsai2SpinBoxes[10]->setValue(tsai2Params[8]);
		tsai2SpinBoxes[11]->setValue(tsai2Params[9]);
		tsai2SpinBoxes[12]->setValue(tsai2Params[10]);
		tsai2SpinBoxes[13]->setValue(tsai2Params[11]);
		tsai2SpinBoxes[14]->setValue(tsai2Params[12]);
		tsai2SpinBoxes[15]->setValue(tsai2Params[13]);
	}
	else if (cameraTypeID == 2) {
		perspectiveParams.push_back(cameras[cameraID]->getEye().x());
		perspectiveParams.push_back(cameras[cameraID]->getEye().y());
		perspectiveParams.push_back(cameras[cameraID]->getEye().z());
		perspectiveParams.push_back(0);
		perspectiveParams.push_back(cameras[cameraID]->getOrientation().x());
		perspectiveParams.push_back(cameras[cameraID]->getOrientation().y());
		perspectiveParams.push_back(cameras[cameraID]->getOrientation().z());
		perspectiveParams.push_back(cameras[cameraID]->getOrientation().w());
		perspectiveParams.push_back(cameras[cameraID]->getAlpha()*RAD_TO_DEG);
		perspectiveParams.push_back(cameras[cameraID]->getBeta()*RAD_TO_DEG);
		perspectiveParams.push_back(cameras[cameraID]->getGama()*RAD_TO_DEG);
		perspectiveParams.push_back(cameras[cameraID]->getFovy());
		cout << "value: " << perspectiveSpinBoxes[0]->value() << endl;
		cout << "W: " << w << endl;
		/*perspectiveSpinBoxes[0]->setValue(w);
		perspectiveSpinBoxes[1]->setValue(h);
		perspectiveSpinBoxes[2]->setValue(perspectiveParams[0]);
		perspectiveSpinBoxes[3]->setValue(perspectiveParams[1]);
		perspectiveSpinBoxes[4]->setValue(perspectiveParams[2]);
		if (perspectiveParams[3] == 0) {

		}
		else {

		}
		perspectiveSpinBoxes[5]->setValue(perspectiveParams[4]);
		perspectiveSpinBoxes[6]->setValue(perspectiveParams[5]);
		perspectiveSpinBoxes[7]->setValue(perspectiveParams[6]);
		perspectiveSpinBoxes[8]->setValue(perspectiveParams[7]);
		if (isnan(perspectiveParams[8]))
			perspectiveSpinBoxes[9]->setValue(0.0);
		else
			perspectiveSpinBoxes[9]->setValue(perspectiveParams[8]);

		if (isnan(perspectiveParams[9]))
			perspectiveSpinBoxes[10]->setValue(0.0);
		else
			perspectiveSpinBoxes[10]->setValue(perspectiveParams[9]);

		if (isnan(perspectiveParams[10]))
			perspectiveSpinBoxes[11]->setValue(0.0);
		else
			perspectiveSpinBoxes[11]->setValue(perspectiveParams[10]);

		perspectiveSpinBoxes[12]->setValue(perspectiveParams[11]);*/
	}
	else if (cameraTypeID == 3) {
		perspectiveDisortionParams.push_back(cameras[cameraID]->getEye().x());
		perspectiveDisortionParams.push_back(cameras[cameraID]->getEye().y());
		perspectiveDisortionParams.push_back(cameras[cameraID]->getEye().z());
		perspectiveDisortionParams.push_back(0);
		perspectiveDisortionParams.push_back(cameras[cameraID]->getOrientation().x());
		perspectiveDisortionParams.push_back(cameras[cameraID]->getOrientation().y());
		perspectiveDisortionParams.push_back(cameras[cameraID]->getOrientation().z());
		perspectiveDisortionParams.push_back(cameras[cameraID]->getOrientation().w());
		perspectiveDisortionParams.push_back(cameras[cameraID]->getAlpha()*RAD_TO_DEG);
		perspectiveDisortionParams.push_back(cameras[cameraID]->getBeta()*RAD_TO_DEG);
		perspectiveDisortionParams.push_back(cameras[cameraID]->getGama()*RAD_TO_DEG);
		perspectiveDisortionParams.push_back(cameras[cameraID]->getFovy());
		perspectiveDisortionParams.push_back(cameras[cameraID]->getDisortionParams().K1);
		perspectiveDisortionParams.push_back(cameras[cameraID]->getDisortionParams().K2);
		perspectiveDisortionParams.push_back(cameras[cameraID]->getDisortionParams().P1);
		perspectiveDisortionParams.push_back(cameras[cameraID]->getDisortionParams().P2);

		perspectiveDisortionsSpinBoxes[0]->setValue(w);
		perspectiveDisortionsSpinBoxes[1]->setValue(h);
		perspectiveDisortionsSpinBoxes[2]->setValue(perspectiveDisortionParams[0]);
		perspectiveDisortionsSpinBoxes[3]->setValue(perspectiveDisortionParams[1]);
		perspectiveDisortionsSpinBoxes[4]->setValue(perspectiveDisortionParams[2]);
		if (perspectiveDisortionParams[3] == 0) {

		}
		else {

		}
		perspectiveDisortionsSpinBoxes[5]->setValue(perspectiveDisortionParams[4]);
		perspectiveDisortionsSpinBoxes[6]->setValue(perspectiveDisortionParams[5]);
		perspectiveDisortionsSpinBoxes[7]->setValue(perspectiveDisortionParams[6]);
		perspectiveDisortionsSpinBoxes[8]->setValue(perspectiveDisortionParams[7]);

		if (isnan(perspectiveDisortionParams[8]))
			perspectiveDisortionsSpinBoxes[9]->setValue(0.0);
		else
			perspectiveDisortionsSpinBoxes[9]->setValue(perspectiveDisortionParams[8]);

		if (isnan(perspectiveDisortionParams[9]))
			perspectiveDisortionsSpinBoxes[10]->setValue(0.0);
		else
			perspectiveDisortionsSpinBoxes[10]->setValue(perspectiveDisortionParams[9]);

		if (isnan(perspectiveDisortionParams[10]))
			perspectiveDisortionsSpinBoxes[11]->setValue(0.0);
		else
			perspectiveDisortionsSpinBoxes[11]->setValue(perspectiveDisortionParams[10]);

		perspectiveDisortionsSpinBoxes[12]->setValue(perspectiveDisortionParams[11]);
		perspectiveDisortionsSpinBoxes[13]->setValue(perspectiveDisortionParams[12]);
		perspectiveDisortionsSpinBoxes[14]->setValue(perspectiveDisortionParams[13]);
		perspectiveDisortionsSpinBoxes[15]->setValue(perspectiveDisortionParams[14]);
		perspectiveDisortionsSpinBoxes[16]->setValue(perspectiveDisortionParams[15]);
	}
	else {
		cout << "camera error" << endl;
	}
}

void CamerasConfig::okButtonAccepted() {
	okClicked = true;
	close();
}

void CamerasConfig::okButtonRejected() {
	close();
}

void CamerasConfig::buttonAddCameraPressed() {
	int selectedCameraID;
	if (firstTime) {
		CameraTypeDialog *dialog = new CameraTypeDialog(this);
		dialog->resize(200, 50);
		dialog->exec();
		if (dialog->okClicked) {
			selectedCameraID = dialog->id;
			cameraType.push_back(selectedCameraID);

			QStringList list;
			int currentID;

			cameras.push_back(new pf::Camera());
			useQuat.push_back(false);
			for (int i = 0; i < cameras.size(); i++) {
				list.push_back("Camera " + QString::number(i));
			}

			int size = cameras.size();
			cameraComboBox->clear();
			for (int i = 0; i < size; i++) {
				cameraComboBox->addItem(list[i]);
			}

				cameraComboBox->setCurrentIndex(cameraComboBox->count()-1);
				setCameraParametersToSpinBoxes();
		}
		delete dialog;
	}

	else {
		QStringList list;
		int currentID;

		if (wasDialogOpen)
			currentID = cameraComboBox->currentIndex();

		cameras.push_back(new pf::Camera());
		for (int i = 0; i < cameras.size(); i++) {
			list.push_back("Camera " + QString::number(i));
		}

		int size = cameras.size();
		cameraComboBox->clear();
		for (int i = 0; i < size; i++) {
			cameraComboBox->addItem(list[i]);
		}

		if (wasDialogOpen)
			cameraComboBox->setCurrentIndex(currentID);
	}
	if (cameraComboBox->count() > 0)
		enableWidgets();
	
}

void CamerasConfig::saveCameraParametersFromSB() {
	int cameraID = cameraComboBox->currentIndex();
	int cameraTypeID = cameraType[cameraID];

	//width = widthSpinBox->value();
	//height = heightSpinBox->value();

	if (cameraTypeID == 0) {
		pf::TSAI1Camera tsai1Camera;
		int w = tsai1SpinBoxes[0]->value();
		int h = tsai1SpinBoxes[1]->value();
		tsai1Camera.trans.x = tsai1SpinBoxes[2]->value();
		tsai1Camera.trans.y = tsai1SpinBoxes[3]->value();
		tsai1Camera.trans.z = tsai1SpinBoxes[4]->value();
		tsai1Camera.rot.x = tsai1SpinBoxes[5]->value();
		tsai1Camera.rot.y = tsai1SpinBoxes[6]->value();
		tsai1Camera.rot.z = tsai1SpinBoxes[7]->value();
		tsai1Camera.focal = tsai1SpinBoxes[8]->value();
		tsai1Camera.kappa1 = tsai1SpinBoxes[9]->value();
		tsai1Camera.c.x = tsai1SpinBoxes[10]->value();
		tsai1Camera.c.y = tsai1SpinBoxes[11]->value();
		tsai1Camera.sx = tsai1SpinBoxes[12]->value();
		tsai1Camera.ncx = tsai1SpinBoxes[13]->value();
		tsai1Camera.nfx = tsai1SpinBoxes[14]->value();
		tsai1Camera.d.x = tsai1SpinBoxes[15]->value();
		tsai1Camera.d.y = tsai1SpinBoxes[16]->value();
		tsai1Camera.dp.x = tsai1SpinBoxes[17]->value();
		tsai1Camera.dp.y = tsai1SpinBoxes[18]->value();
		delete cameras[cameraID];
		cameras[cameraID] = new pf::Camera(tsai1Camera, w, h);
		cout << cameras[cameraID]->getGama() << endl;
	}
	else if (cameraTypeID == 1) {
		pf::TSAI2Camera tsai2Camera;
		int w = tsai2SpinBoxes[0]->value();
		int h = tsai2SpinBoxes[1]->value();
		tsai2Camera.trans.x = tsai2SpinBoxes[2]->value();
		tsai2Camera.trans.y = tsai2SpinBoxes[3]->value();
		tsai2Camera.trans.z = tsai2SpinBoxes[4]->value();
		tsai2Camera.rot.x = tsai2SpinBoxes[5]->value();
		tsai2Camera.rot.y = tsai2SpinBoxes[6]->value();
		tsai2Camera.rot.z = tsai2SpinBoxes[7]->value();
		tsai2Camera.focal1 = tsai2SpinBoxes[8]->value();
		tsai2Camera.focal2 = tsai2SpinBoxes[9]->value();
		tsai2Camera.K1 = tsai2SpinBoxes[10]->value();
		tsai2Camera.K2 = tsai2SpinBoxes[11]->value();
		tsai2Camera.P1 = tsai2SpinBoxes[12]->value();
		tsai2Camera.P2 = tsai2SpinBoxes[13]->value();
		tsai2Camera.c.x = tsai2SpinBoxes[14]->value();
		tsai2Camera.c.y = tsai2SpinBoxes[15]->value();
		delete cameras[cameraID];
		cameras[cameraID] = new pf::Camera(tsai2Camera, w, h);
	}
	else if (cameraTypeID == 2) {
		int w = perspectiveSpinBoxes[0]->value();
		int h = perspectiveSpinBoxes[1]->value();
		float pos_x = perspectiveSpinBoxes[2]->value();
		float pos_y = perspectiveSpinBoxes[3]->value();
		float pos_z = perspectiveSpinBoxes[4]->value();
		if (perspectiveQuatCB->isChecked())
			useQuat[cameraID] = true;
		else
			useQuat[cameraID] = false;
		float quat_x = perspectiveSpinBoxes[5]->value();
		float quat_y = perspectiveSpinBoxes[6]->value();
		float quat_z = perspectiveSpinBoxes[7]->value();
		float quat_w = perspectiveSpinBoxes[8]->value();
		float rot_x = perspectiveSpinBoxes[9]->value() * DEG_TO_RAD;
		float rot_y = perspectiveSpinBoxes[10]->value()* DEG_TO_RAD;
		float rot_z = perspectiveSpinBoxes[11]->value()* DEG_TO_RAD;
		float fovy = perspectiveSpinBoxes[12]->value();

		pf::Camera *camTmp;
		if (useQuat[cameraID]) camTmp = new pf::Camera(pf::Vec3f(pos_x, pos_y, pos_z), pf::Quat(quat_x, quat_y, quat_z, quat_w), fovy, w, h);
		else camTmp = new pf::Camera(pf::Vec3f(pos_x, pos_y, pos_z), rot_x, rot_y, rot_z, fovy, w, h);

		delete cameras[cameraID];
		cameras[cameraID] = camTmp;
	}
	else if(cameraTypeID == 3) {
		int w = perspectiveDisortionsSpinBoxes[0]->value();
		int h = perspectiveDisortionsSpinBoxes[1]->value();
		float pos_x = perspectiveDisortionsSpinBoxes[2]->value();
		float pos_y = perspectiveDisortionsSpinBoxes[3]->value();
		float pos_z = perspectiveDisortionsSpinBoxes[4]->value();
		
		if (perspectiveDisortionQuatCB->isChecked())
			useQuat[cameraID] = true;
		else
			useQuat[cameraID] = false;
		float quat_x = perspectiveDisortionsSpinBoxes[5]->value();
		float quat_y = perspectiveDisortionsSpinBoxes[6]->value();
		float quat_z = perspectiveDisortionsSpinBoxes[7]->value();
		float quat_w = perspectiveDisortionsSpinBoxes[8]->value();
		float rot_x = perspectiveDisortionsSpinBoxes[9]->value()* DEG_TO_RAD;
		float rot_y = perspectiveDisortionsSpinBoxes[10]->value()* DEG_TO_RAD;
		float rot_z = perspectiveDisortionsSpinBoxes[11]->value()* DEG_TO_RAD;
		float fovy = perspectiveDisortionsSpinBoxes[12]->value();
		pf::disortions dis;
		dis.K1 = perspectiveDisortionsSpinBoxes[13]->value();
		dis.K2 = perspectiveDisortionsSpinBoxes[14]->value();
		dis.P1 = perspectiveDisortionsSpinBoxes[15]->value();
		dis.P2 = perspectiveDisortionsSpinBoxes[16]->value();

		pf::Camera *camTmp;

		if (useQuat[cameraID]) camTmp = new  pf::Camera(pf::Vec3f(pos_x, pos_y, pos_z), pf::Quat(quat_x, quat_y, quat_z, quat_w), fovy, w, h, dis);
		else camTmp = new pf::Camera(pf::Vec3f(pos_x, pos_y, pos_z), rot_x, rot_y, rot_z, fovy, w, h, dis);
		delete cameras[cameraID];
		cameras[cameraID] = camTmp;
	}
	else {
		cerr << "error cameraType" << endl;
	}
}

void CamerasConfig::setCameraType() {
	int cameraID = cameraComboBox->currentIndex();
	int cameraTypeID = cameraType[cameraID];

	cameraStackedWidget->setCurrentIndex(cameraTypeID);
	saveCameraParametersFromSB();
}

void CamerasConfig::returnCameras(QVector<pf::Camera*> &dest) {
	qDeleteAll(dest);
	dest.clear();

	for (int i = 0; i < cameras.size(); i++) {
		if (cameraType[i] == 0) {
			pf::TSAI1Camera tsai1Camera;
			int w = cameras[i]->getWidth();
			int h = cameras[i]->getHeight();
			tsai1Camera.trans.x = cameras[i]->getTSAI1Param().trans.x;
			tsai1Camera.trans.y = cameras[i]->getTSAI1Param().trans.y;
			tsai1Camera.trans.z = cameras[i]->getTSAI1Param().trans.z;
			tsai1Camera.rot.x = DEG_TO_RAD*cameras[i]->getTSAI1Param().rot.x;
			tsai1Camera.rot.y = DEG_TO_RAD*cameras[i]->getTSAI1Param().rot.y;
			tsai1Camera.rot.z = DEG_TO_RAD*cameras[i]->getTSAI1Param().rot.z;
			tsai1Camera.focal = cameras[i]->getTSAI1Param().focal;
			tsai1Camera.kappa1 = cameras[i]->getTSAI1Param().kappa1;
			tsai1Camera.c.x = cameras[i]->getTSAI1Param().c.x;
			tsai1Camera.c.y = cameras[i]->getTSAI1Param().c.y;
			tsai1Camera.sx = cameras[i]->getTSAI1Param().sx;
			tsai1Camera.ncx = cameras[i]->getTSAI1Param().ncx;
			tsai1Camera.nfx = cameras[i]->getTSAI1Param().nfx;
			tsai1Camera.d.x = cameras[i]->getTSAI1Param().d.x;
			tsai1Camera.d.y = cameras[i]->getTSAI1Param().d.y;
			tsai1Camera.dp.x = cameras[i]->getTSAI1Param().dp.x;
			tsai1Camera.dp.y = cameras[i]->getTSAI1Param().dp.y;

			pf::Camera *camTmp = new pf::Camera(tsai1Camera, w, h);
			dest.push_back(camTmp);
			//delete camTmp;
		}

		else if (cameraType[i] == 1) {
			pf::TSAI2Camera tsai2Camera;
			int w = cameras[i]->getWidth();
			int h = cameras[i]->getHeight();
			tsai2Camera.trans.x = cameras[i]->getTSAI2Param().trans.x;
			tsai2Camera.trans.y = cameras[i]->getTSAI2Param().trans.y;
			tsai2Camera.trans.z = cameras[i]->getTSAI2Param().trans.z;
			tsai2Camera.rot.x = DEG_TO_RAD*cameras[i]->getTSAI2Param().rot.x;
			tsai2Camera.rot.y = DEG_TO_RAD*cameras[i]->getTSAI2Param().rot.y;
			tsai2Camera.rot.z = DEG_TO_RAD*cameras[i]->getTSAI2Param().rot.z;
			tsai2Camera.focal1 = cameras[i]->getTSAI2Param().focal1;
			tsai2Camera.focal2 = cameras[i]->getTSAI2Param().focal2;
			tsai2Camera.K1 = cameras[i]->getTSAI2Param().K1;
			tsai2Camera.K2 = cameras[i]->getTSAI2Param().K2;
			tsai2Camera.P1 = cameras[i]->getTSAI2Param().P1;
			tsai2Camera.P2 = cameras[i]->getTSAI2Param().P2;
			tsai2Camera.c.x = cameras[i]->getTSAI2Param().c.x;
			tsai2Camera.c.y = cameras[i]->getTSAI2Param().c.y;

			pf::Camera *camTmp = new pf::Camera(tsai2Camera, w, h);
			dest.push_back(camTmp);
			//delete camTmp;
		}
		else if (cameraType[i] == 2) {
			int w = cameras[i]->getWidth();
			int h = cameras[i]->getHeight();

			float pos_x = cameras[i]->getEye().x();
			float pos_y = cameras[i]->getEye().y();
			float pos_z = cameras[i]->getEye().z();
			float quat_x = cameras[i]->getOrientation().x();
			float quat_y = cameras[i]->getOrientation().y();
			float quat_z = cameras[i]->getOrientation().z();
			float quat_w = cameras[i]->getOrientation().w();
			float rot_x = DEG_TO_RAD*cameras[i]->getAlpha();
			float rot_y = DEG_TO_RAD*cameras[i]->getBeta();
			float rot_z = DEG_TO_RAD*cameras[i]->getGama();
			float fovy = cameras[i]->getFovy();

			pf::Camera *camTmp;
			if(useQuat[i])
				camTmp = new pf::Camera(pf::Vec3f(pos_x, pos_y, pos_z), pf::Quat(quat_x, quat_y, quat_z, quat_w), fovy, w, h);
			else 
				camTmp = new pf::Camera(pf::Vec3f(pos_x, pos_y, pos_z), rot_x, rot_y, rot_z, fovy, w, h);
			
			dest.push_back(camTmp);
		}
		else if (cameraType[i] == 3) {
			int w = cameras[i]->getWidth();
			int h = cameras[i]->getHeight();

			float pos_x = cameras[i]->getEye().x();
			float pos_y = cameras[i]->getEye().y();
			float pos_z = cameras[i]->getEye().z();
			float quat_x = cameras[i]->getOrientation().x();
			float quat_y = cameras[i]->getOrientation().y();
			float quat_z = cameras[i]->getOrientation().z();
			float quat_w = cameras[i]->getOrientation().w();
			float rot_x = DEG_TO_RAD*cameras[i]->getAlpha();
			float rot_y = DEG_TO_RAD*cameras[i]->getBeta();
			float rot_z = DEG_TO_RAD*cameras[i]->getGama();
			float fovy = cameras[i]->getFovy();

			pf::disortions dis;
			dis.K1 = cameras[i]->getDisortionParams().K1;
			dis.K2 = cameras[i]->getDisortionParams().K2;
			dis.P1 = cameras[i]->getDisortionParams().P1;
			dis.P2 = cameras[i]->getDisortionParams().P2;

			pf::Camera *camTmp;
			if (useQuat[i])
				camTmp = new pf::Camera(pf::Vec3f(pos_x, pos_y, pos_z), pf::Quat(quat_x, quat_y, quat_z, quat_w), fovy, w, h, dis);
			else
				camTmp = new pf::Camera(pf::Vec3f(pos_x, pos_y, pos_z), rot_x, rot_y, rot_z, fovy, w, h, dis);

			dest.push_back(camTmp);
		}
		else {
			cout << "else" << endl;
		}
	}
}

void CamerasConfig::getCamerasFromMainWindow(QVector<pf::Camera*> cams) {
	for (int i = 0; i < cams.size(); i++)
		addNewCamera->click();

	qDeleteAll(cameras);
	cameras.clear();
	//useQuat.clear();
	cameraType.clear();

	for (int i = 0; i < cams.size(); i++) {
		if (cams[i]->getType() == 3) {

			int w = cams[i]->getWidth();
			int h = cams[i]->getHeight();

			pf::TSAI1Camera tsai1Camera;
			tsai1Camera.trans.x = cams[i]->getTSAI1Param().trans.x;
			tsai1Camera.trans.y = cams[i]->getTSAI1Param().trans.y;
			tsai1Camera.trans.z = cams[i]->getTSAI1Param().trans.z;
			tsai1Camera.rot.x = RAD_TO_DEG*cams[i]->getTSAI1Param().rot.x;
			tsai1Camera.rot.y = RAD_TO_DEG*cams[i]->getTSAI1Param().rot.y;
			tsai1Camera.rot.z = RAD_TO_DEG*cams[i]->getTSAI1Param().rot.z;
			tsai1Camera.focal = cams[i]->getTSAI1Param().focal;
			tsai1Camera.kappa1 = cams[i]->getTSAI1Param().kappa1;
			tsai1Camera.c.x = cams[i]->getTSAI1Param().c.x;
			tsai1Camera.c.y = cams[i]->getTSAI1Param().c.y;
			tsai1Camera.sx = cams[i]->getTSAI1Param().sx;
			tsai1Camera.ncx = cams[i]->getTSAI1Param().ncx;
			tsai1Camera.nfx = cams[i]->getTSAI1Param().nfx;
			tsai1Camera.d.x = cams[i]->getTSAI1Param().d.x;
			tsai1Camera.d.y = cams[i]->getTSAI1Param().d.y;
			tsai1Camera.dp.x = cams[i]->getTSAI1Param().dp.x;
			tsai1Camera.dp.y = cams[i]->getTSAI1Param().dp.y;

			pf::Camera *camTmp = new pf::Camera(tsai1Camera, w, h);
			cameras.push_back(camTmp);
			cameraType.push_back(0);
		}

		else if (cams[i]->getType() == 4) {

			int w = cams[i]->getWidth();
			int h = cams[i]->getHeight();

			pf::TSAI2Camera tsai2Camera;
			tsai2Camera.trans.x = cams[i]->getTSAI2Param().trans.x;
			tsai2Camera.trans.y = cams[i]->getTSAI2Param().trans.y;
			tsai2Camera.trans.z = cams[i]->getTSAI2Param().trans.z;
			tsai2Camera.rot.x = RAD_TO_DEG*cams[i]->getTSAI2Param().rot.x;
			tsai2Camera.rot.y = RAD_TO_DEG*cams[i]->getTSAI2Param().rot.y;
			tsai2Camera.rot.z = RAD_TO_DEG*cams[i]->getTSAI2Param().rot.z;
			tsai2Camera.focal1 = cams[i]->getTSAI2Param().focal1;
			tsai2Camera.focal2 = cams[i]->getTSAI2Param().focal2;
			tsai2Camera.c.x = cams[i]->getTSAI2Param().c.x;
			tsai2Camera.c.y = cams[i]->getTSAI2Param().c.y;
			tsai2Camera.K1 = cams[i]->getTSAI2Param().K1;
			tsai2Camera.K2 = cams[i]->getTSAI2Param().K2;
			tsai2Camera.P1 = cams[i]->getTSAI2Param().P1;
			tsai2Camera.P2 = cams[i]->getTSAI2Param().P2;

			pf::Camera *camTmp = new pf::Camera(tsai2Camera, w, h);
			cameras.push_back(camTmp);
			cameraType.push_back(1);
		}
		else if (cams[i]->getType() == 1) {
			int w = cams[i]->getWidth();
			int h = cams[i]->getHeight();

			float pos_x = cams[i]->getEye().x();
			float pos_y = cams[i]->getEye().y();
			float pos_z = cams[i]->getEye().z();
			float quat_x = cams[i]->getOrientation().x();
			float quat_y = cams[i]->getOrientation().y();
			float quat_z = cams[i]->getOrientation().z();
			float quat_w = cams[i]->getOrientation().w();
			float rot_x = RAD_TO_DEG*cams[i]->getAlpha();
			float rot_y = RAD_TO_DEG*cams[i]->getBeta();
			float rot_z = RAD_TO_DEG*cams[i]->getGama();
			float fovy = cams[i]->getFovy();

			pf::Camera *camTmp;
			if(useQuat[i])
				camTmp  = new pf::Camera(pf::Vec3f(pos_x, pos_y, pos_z), pf::Quat(quat_x, quat_y, quat_z, quat_w), fovy, w, h);
			else
				camTmp = new pf::Camera(pf::Vec3f(pos_x, pos_y, pos_z), rot_x, rot_y, rot_z, fovy, w, h);

			cameras.push_back(camTmp);
			cameraType.push_back(2);
		}
		else if (cams[i]->getType() == 2) {
			int w = cams[i]->getWidth();
			int h = cams[i]->getHeight();

			float pos_x = cams[i]->getEye().x();
			float pos_y = cams[i]->getEye().y();
			float pos_z = cams[i]->getEye().z();
			float quat_x = cams[i]->getOrientation().x();
			float quat_y = cams[i]->getOrientation().y();
			float quat_z = cams[i]->getOrientation().z();
			float quat_w = cams[i]->getOrientation().w();
			float rot_x = RAD_TO_DEG*cams[i]->getAlpha();
			float rot_y = RAD_TO_DEG*cams[i]->getBeta();
			float rot_z = RAD_TO_DEG*cams[i]->getGama();
			float fovy = cams[i]->getFovy();

			pf::disortions dis;
			dis.K1 = cams[i]->getDisortionParams().K1;
			dis.K2 = cams[i]->getDisortionParams().K2;
			dis.P1 = cams[i]->getDisortionParams().P1;
			dis.P2 = cams[i]->getDisortionParams().P2;

			pf::Camera *camTmp;
			if (useQuat[i])
				camTmp = new pf::Camera(pf::Vec3f(pos_x, pos_y, pos_z), pf::Quat(quat_x, quat_y, quat_z, quat_w), fovy, w, h, dis);
			else
				camTmp = new pf::Camera(pf::Vec3f(pos_x, pos_y, pos_z), rot_x, rot_y, rot_z, fovy, w, h, dis);

			cameras.push_back(camTmp);
			cameraType.push_back(3);
		}
		else{
			cout << "Error loading cameras" << endl;
		}
	}
	setCameraParametersToSpinBoxes();
	setCameraType();
	wasDialogOpen = true;
	firstTime = true;
}

void CamerasConfig::disableWidgets() {
	cameraStackedWidget->setDisabled(true);
	cameraComboBox->setDisabled(true);
	cameraTypeComboBox->hide();
}

void CamerasConfig::enableWidgets() {
	cameraStackedWidget->setDisabled(false);
	cameraComboBox->setDisabled(false);
	cameraComboBox->setDisabled(false);
}

void CamerasConfig::hidePerspectiveDisortionQuatLayout() {
	if (perspectiveDisortionQuatCB->isChecked()){
		perspectiveDisortionsSpinBoxes[5]->setDisabled(false);
		perspectiveDisortionsSpinBoxes[6]->setDisabled(false);
		perspectiveDisortionsSpinBoxes[7]->setDisabled(false);
		perspectiveDisortionsSpinBoxes[8]->setDisabled(false);

		perspectiveDisortionsSpinBoxes[9]->setDisabled(true);
		perspectiveDisortionsSpinBoxes[10]->setDisabled(true);
		perspectiveDisortionsSpinBoxes[11]->setDisabled(true);

		perspectiveDisortionRadCB->setChecked(false);
		
	}
	else {
		perspectiveDisortionsSpinBoxes[5]->setDisabled(true);
		perspectiveDisortionsSpinBoxes[6]->setDisabled(true);
		perspectiveDisortionsSpinBoxes[7]->setDisabled(true);
		perspectiveDisortionsSpinBoxes[8]->setDisabled(true);

		perspectiveDisortionsSpinBoxes[9]->setDisabled(false);
		perspectiveDisortionsSpinBoxes[10]->setDisabled(false);
		perspectiveDisortionsSpinBoxes[11]->setDisabled(false);

		perspectiveDisortionRadCB->setChecked(true);
	}
}

void CamerasConfig::hidePerspectiveDisortionRadLayout() {
	if (!perspectiveDisortionRadCB->isChecked()) {
		perspectiveDisortionsSpinBoxes[5]->setDisabled(false);
		perspectiveDisortionsSpinBoxes[6]->setDisabled(false);
		perspectiveDisortionsSpinBoxes[7]->setDisabled(false);
		perspectiveDisortionsSpinBoxes[8]->setDisabled(false);

		perspectiveDisortionsSpinBoxes[9]->setDisabled(true);
		perspectiveDisortionsSpinBoxes[10]->setDisabled(true);
		perspectiveDisortionsSpinBoxes[11]->setDisabled(true);

		perspectiveDisortionQuatCB->setChecked(true);

	}
	else {
		perspectiveDisortionsSpinBoxes[5]->setDisabled(true);
		perspectiveDisortionsSpinBoxes[6]->setDisabled(true);
		perspectiveDisortionsSpinBoxes[7]->setDisabled(true);
		perspectiveDisortionsSpinBoxes[8]->setDisabled(true);

		perspectiveDisortionsSpinBoxes[9]->setDisabled(false);
		perspectiveDisortionsSpinBoxes[10]->setDisabled(false);
		perspectiveDisortionsSpinBoxes[11]->setDisabled(false);

		perspectiveDisortionQuatCB->setChecked(false);
	}
}

void CamerasConfig::hidePerspectiveRadLayout() {
	if (!perspectiveRadCB->isChecked()) {
		perspectiveSpinBoxes[5]->setDisabled(false);
		perspectiveSpinBoxes[6]->setDisabled(false);
		perspectiveSpinBoxes[7]->setDisabled(false);
		perspectiveSpinBoxes[8]->setDisabled(false);

		perspectiveSpinBoxes[9]->setDisabled(true);
		perspectiveSpinBoxes[10]->setDisabled(true);
		perspectiveSpinBoxes[11]->setDisabled(true);

		perspectiveQuatCB->setChecked(true);

	}
	else {
		perspectiveSpinBoxes[5]->setDisabled(true);
		perspectiveSpinBoxes[6]->setDisabled(true);
		perspectiveSpinBoxes[7]->setDisabled(true);
		perspectiveSpinBoxes[8]->setDisabled(true);

		perspectiveSpinBoxes[9]->setDisabled(false);
		perspectiveSpinBoxes[10]->setDisabled(false);
		perspectiveSpinBoxes[11]->setDisabled(false);

		perspectiveQuatCB->setChecked(false);
	}
}

void CamerasConfig::hidePerspectiveQuatLayout() {
	if (perspectiveQuatCB->isChecked()) {
		perspectiveSpinBoxes[5]->setDisabled(false);
		perspectiveSpinBoxes[6]->setDisabled(false);
		perspectiveSpinBoxes[7]->setDisabled(false);
		perspectiveSpinBoxes[8]->setDisabled(false);

		perspectiveSpinBoxes[9]->setDisabled(true);
		perspectiveSpinBoxes[10]->setDisabled(true);
		perspectiveSpinBoxes[11]->setDisabled(true);

		perspectiveRadCB->setChecked(false);

	}
	else {
		perspectiveSpinBoxes[5]->setDisabled(true);
		perspectiveSpinBoxes[6]->setDisabled(true);
		perspectiveSpinBoxes[7]->setDisabled(true);
		perspectiveSpinBoxes[8]->setDisabled(true);

		perspectiveSpinBoxes[9]->setDisabled(false);
		perspectiveSpinBoxes[10]->setDisabled(false);
		perspectiveSpinBoxes[11]->setDisabled(false);

		perspectiveRadCB->setChecked(true);
	}
}