#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glwidget.h"

#include <stdio.h>

#include <QFileDialog>
#include <QtWidgets>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	/*enables drop action*/
	setAcceptDrops(true);
	/*rezerwacja pamieci dla wektorow - aplikacja operujaca na duzej liczbie wektorow moze miec spowolniony czas dzialana*/
	reserveVectorsMemory();

    ui->setupUi(this);

	prepareLayouts();
	prepareMenus();
	prepareMainModel();

	initializeMaps();
	
	addFrameButton->setVisible(true);
	addFrameButton->setText(QString("Add window"));

	/*dodaj 4 okna przy starcie aplikacji*/
	for (int i = 0; i < 4; i++) {
		addFrameButton->click();
	}

	/*inicjalzacja wartosci glownego slidera*/
	globalSlider->setVisible(true);
	globalSlider->setOrientation(Qt::Horizontal);
	globalSlider->setMinimum(1);
	globalSlider->setMaximum(1);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::prepareMenus() {
	menuBar = new QMenuBar(this);
	setMenuBar(menuBar);

	/*########################*/
	/*save, open files*/
	plikMenu = menuBar->addMenu("&File");
	plikMenu->addSeparator();

	loadMenu = plikMenu->addMenu(tr("&Load"));

	loadDatAction = new QAction("Load dat file", this);
	connect(loadDatAction, &QAction::triggered, this, &MainWindow::loadDatFromFile);
	loadMenu->addAction(loadDatAction);

	loadFromAmcAction = new QAction("Load amc file", this);
	connect(loadFromAmcAction, &QAction::triggered, this, &MainWindow::loadFromAmc);
	loadMenu->addAction(loadFromAmcAction);

	loadAsfAction = new QAction("Load asf file", this);
	connect(loadAsfAction, &QAction::triggered, this, &MainWindow::loadFromASF);
	loadMenu->addAction(loadAsfAction);

	loadMenu->addSeparator();

	loadConfigAction = new QAction("Load project", this);
	connect(loadConfigAction, &QAction::triggered, this, &MainWindow::loadProjectPressed);
	loadMenu->addAction(loadConfigAction);

	/****/
	saveMenu = plikMenu->addMenu(tr("&Save"));

	saveDatAction = new QAction("Save dat file", this);
	connect(saveDatAction, &QAction::triggered, this, &MainWindow::saveDatToFile);
	saveMenu->addAction(saveDatAction);

	saveAmcSequenceAction = new QAction("Save amc sequence", this);
	connect(saveAmcSequenceAction, &QAction::triggered, this, &MainWindow::saveAMCToFile);
	saveMenu->addAction(saveAmcSequenceAction);

	saveAsfAction = new QAction("Save asf file", this);
	connect(saveAsfAction, &QAction::triggered, this, &MainWindow::saveAsfToFile);
	saveMenu->addAction(saveAsfAction);

	saveMenu->addSeparator();

	saveConfigAction = new QAction("Save project", this);
	connect(saveConfigAction, &QAction::triggered, this, &MainWindow::saveProjectPressed);
	saveMenu->addAction(saveConfigAction);
	/****/

	plikMenu->addSeparator();

	manageModelBonesAction = new QAction("Manage model bones", this);
	connect(manageModelBonesAction, &QAction::triggered, this, &MainWindow::manageBonesPressed);
	plikMenu->addAction(manageModelBonesAction);
	if (comboBoxFrameSelector->count() == 0)
		manageModelBonesAction->setDisabled(true);

	/*setModelAction = new QAction("Set model", this);
	connect(setModelAction, &QAction::triggered, this, &MainWindow::setModelPressed);
	plikMenu->addAction(setModelAction);*/

	/******************************/
	/*cameras*/
	cameraMenu = menuBar->addMenu("&Camera");
	openCameraConfigAction = new QAction("Camera configuration", this);
	connect(openCameraConfigAction, &QAction::triggered, this, &MainWindow::openCameraConfigPressed);
	cameraMenu->addAction(openCameraConfigAction);

	cameraMenu->addSeparator();

	saveCamerasAction = new QAction("Save cameras", this);
	connect(saveCamerasAction, &QAction::triggered, this, &MainWindow::saveCamerasPressed);
	cameraMenu->addAction(saveCamerasAction);

	loadCamerasAction = new QAction(tr("&Load cameras"));
	connect(loadCamerasAction, &QAction::triggered, this, &MainWindow::loadCamerasPressed);
	cameraMenu->addAction(loadCamerasAction);

	/**********************************/

	/*config*/
	configAction = new QAction(tr("&Config"));
	connect(configAction, &QAction::triggered, this, &MainWindow::configPressed);
	menuBar->addAction(configAction);

	/*help*/
	helpAction = new QAction(tr("&Help"));
	menuBar->addAction(helpAction);

}

void MainWindow::prepareLayouts() {
	/*przycisk do dodawania nowego okna*/
	addFrameButton = new QPushButton;
	addFrameButton->setVisible(false);
	connect(addFrameButton, SIGNAL(released()), this, SLOT(buttonAddFramePressed()));

	/*glowny slider*/
	globalSlider = new QSlider;
	globalSlider->setVisible(false);
	connect(globalSlider, SIGNAL(valueChanged(int)), this, SLOT(updateSliders()));

	/*przyciski do obslugi glownego slidera*/
	increaseGlobalSlider = new QPushButton(this);
	reduceGlobalSlider = new QPushButton(this);
	connect(increaseGlobalSlider, SIGNAL(released()), this, SLOT(increaseGlobalSliderPressed()));
	connect(reduceGlobalSlider, SIGNAL(released()), this, SLOT(reduceGlobalSliderPressed()));

	/*comboBox do wyboru okna*/
	comboBoxFrameSelector = new QComboBox;
	connect(comboBoxFrameSelector, SIGNAL(activated(int)), this, SLOT(refreshRotateSideMenu()));

	/*przycisk do ustawienia aktualnego indeksu do pozostalych okien*/
	setIteratorButton = new QPushButton("Set frame", this);
	connect(setIteratorButton, SIGNAL(released()), this, SLOT(setIteratorToFrames()));

	gridLayout = new QGridLayout;
	vboxRotateLayout = new QVBoxLayout;
	vboxLengthLayout = new QVBoxLayout;
	vboxGeometryLayout = new QVBoxLayout;
	gridChildLayout = new QGridLayout;
	sliderLayout = new QHBoxLayout;
	selectFrameLayout = new QHBoxLayout;
	selectFrameLabel = new QLabel;

	windowLayout = new QWidget;

	scrollWidget = new QWidget;
	scrollRotateSideWidget = new QWidget;
	scrollLengthSideWidget = new QWidget;
	scrollGeometrySideWidget = new QWidget;
	tabWidget = new QWidget;
	sideManuTabWidget = new QTabWidget(tabWidget);

	/*głowny przewijany obszar*/
	scrollArea = new QScrollArea;
	scrollArea->setWidgetResizable(true);
	scrollWidget->setLayout(gridChildLayout);

	/*boczny przewijany obszar - rotacja*/
	scrollRotateSideArea = new QScrollArea;
	scrollRotateSideArea->setWidgetResizable(true);
	scrollRotateSideWidget->setLayout(vboxRotateLayout);

	/*boczny przewijany obszar - kalibracja dlugosci*/
	scrollLengthSideArea = new QScrollArea;
	scrollLengthSideArea->setWidgetResizable(true);
	scrollLengthSideWidget->setLayout(vboxLengthLayout);

	/*boczny przewijany obszar - geometry*/
	scrollGeometrySideArea = new QScrollArea;
	scrollGeometrySideArea->setWidgetResizable(true);
	scrollGeometrySideWidget->setLayout(vboxGeometryLayout);

	/*dodanie widgetu do scrollowanego obszaru*/
	scrollArea->setWidget(scrollWidget);
	scrollRotateSideArea->setWidget(scrollRotateSideWidget);
	scrollLengthSideArea->setWidget(scrollLengthSideWidget);
	scrollGeometrySideArea->setWidget(scrollGeometrySideWidget);

	/*ustawienie zakladek w QTabWidget*/
	sideManuTabWidget->addTab(scrollLengthSideArea, "Bones");
	sideManuTabWidget->addTab(scrollRotateSideArea, "Model pose");
	sideManuTabWidget->addTab(scrollGeometrySideArea, "Geometry");
	sideManuTabWidget->setMinimumWidth(260);

	selectFrameLabel->setText("Select window");
	selectFrameLayout->addWidget(selectFrameLabel);
	selectFrameLayout->addWidget(comboBoxFrameSelector);
	selectFrameLayout->addWidget(setIteratorButton);

	sliderLayout->addWidget(reduceGlobalSlider);
	sliderLayout->addWidget(increaseGlobalSlider);
	sliderLayout->addWidget(globalSlider);

	connect(sideManuTabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabSelected()));

	gridLayout->addWidget(addFrameButton, 0, 0, 1, 1);
	gridLayout->addLayout(sliderLayout, 0, 1, 1, 8);
	gridLayout->addLayout(selectFrameLayout, 0, 9, 1, 2);
	gridLayout->addWidget(scrollArea, 1, 0, 9, 9);
	gridLayout->addWidget(sideManuTabWidget, 1, 9, 9, 2);

	windowLayout->setLayout(gridLayout);

	setCentralWidget(windowLayout);

	addGlobalSliderButtonsImages(reduceGlobalSlider, increaseGlobalSlider);
}

void MainWindow::prepareMainModel() {
	FileHandler fileHandler;

	try {
		mainModel = new pf::Model3D(pf::Model3D::Cylinder, ASF_TEMPLATE_PATH, DAT_TEMPLATE_PATH);
		pf::MotionData::loadASF(ASF_TEMPLATE_PATH, idxBonesMap, asfBones);
		mainModel->loadConfig(DAT_TEMPLATE_PATH, bonesConf, bonesGeometry);

		modelState = fileHandler.getAMCTemplate(mainModel);

		mainModel->setModelState(modelState[0]);
	}
	catch (std::exception &e) {
		QMessageBox::critical(this, "Error", e.what(), QMessageBox::Ok);
		exit(-1);
	}

	limits = mainModel->getLimits();
	usedBones = mainModel->getNamesMovingBones();
	velocity = mainModel->getVelocity();
	allBones = mainModel->getNamesBones();
	
}

void MainWindow::initializeMaps() {
	ModelHandler modelHandler;
	modelHandler.initializeBonesDOFMap(modelDOF, asfBones);
	modelHandler.initializeBonesRadiusMap(modelRadius, asfBones, bonesGeometry);
	modelHandler.initializeBonesLengthMap(bonesLength, asfBones);
	modelHandler.initializeBonesLimitsMap(modelLimits, asfBones);
	modelHandler.initializeBonesVelocityMap(modelVelocity, asfBones);

	map<string, pf::Vec3f> bonesRotationsTMP;
	bonesRotationsTMP["tmp"] = pf::Vec3f(0, 0, 0);
	vector<float> modelTranslationTMP;

	bonesRotations.push_back(bonesRotationsTMP);
	modelTranslation.push_back(modelTranslationTMP);
	saveModelState.push_back(false);

	modelHandler.initializeBonesRotationsMap(bonesRotations[0], asfBones);
	modelTranslation[0].push_back(0);
	modelTranslation[0].push_back(0);
	modelTranslation[0].push_back(0);
}

void MainWindow::saveDatToFile() {
	FileHandler fileHandler;

	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save as .dat"), "",
		tr("DAT (*.dat);;All Files(*)"));

	if (!fileName.isEmpty()) {

		/*zapis pliku dat*/
		fileHandler.saveDATToFile(
			bonesConf,
			bonesGeometry,
			fileName
		);

		QMessageBox::information(this, "DAT saved", "DAT file saved.", QMessageBox::Ok);
	}

	//delete fileHandler;
}

void MainWindow::saveDatFromQStringToFile(QString fileName) {
	FileHandler fileHandler;

	if (!fileName.isEmpty()) {

		/*zapis pliku dat*/
		fileHandler.saveDATToFile(
			bonesConf,
			bonesGeometry,
			fileName
		);

	}
}

void MainWindow::loadDatFromFile() {
	QString filePath = QFileDialog::getOpenFileName(this,
		tr("Open .dat file"), "",
		tr("DAT (*.dat);;All Files(*)"));

	if (!filePath.isEmpty()) {
		loadDAT(filePath);
		QMessageBox::information(this, "DAT loaded", "DAT file loaded.", QMessageBox::Ok);
	}	

	//delete fileHandler;

}

void MainWindow::loadDatDropped(QString str) {
	if (!str.isEmpty()) {
		loadDAT(str);
		QMessageBox::information(this, "DAT loaded", "DAT file loaded.", QMessageBox::Ok);
	}

}

void MainWindow::loadDAT(QString filePath) {
	FileHandler fileHandler;
	ModelHandler modelHandler;
	datLoaded = true;
	datPath = filePath;
	/*ustaw wartosci mapy dlugosci kosci na 0*/
	modelHandler.clearBonesLengthMap(bonesLength);

	/*zapis modelState do mapy aktualnych kosci modelu*/
	for (int j = 0; j < bonesRotations.size(); j++) {
		modelHandler.saveModelStateToMap(bonesRotations[j], modelState[j], bonesConf);
	}

	bonesConf.clear();
	bonesGeometry.clear();

	/*wczytanie pliku dat*/
	if (!asfLoaded)
		fileHandler.loadDatFromFile(mainModel, filePath.toUtf8().constData(), bonesConf, bonesGeometry);
	else
		fileHandler.loadDatFromFile(mainModel, filePath.toUtf8().constData(), bonesConf, bonesGeometry, asfPath);

	/*wczytanie pliku amc*/
	if (!amcLoaded) {
		modelState = fileHandler.getAMCTemplate(mainModel);
		modelHandler.updateModelStateFromMap(modelState, bonesRotations, bonesConf, modelTranslation);
	}
	else {
		modelState = fileHandler.getAMCFile(mainModel, amcPath);
		//modelHandler.updateModelStateFromMap(modelState, bonesRotations, bonesConf, modelTranslation);
	}

	modelHandler.saveBonesLengthToMap(bonesLength, bonesGeometry);
	modelHandler.updateBonesLengthFromMap(bonesLength, bonesGeometry);

	goldenRatioSpinBox->setValue(modelHandler.getModelHeightFromMap(bonesLength));

	limits = mainModel->getLimits();
	usedBones = mainModel->getNamesMovingBones();
	velocity = mainModel->getVelocity();
	allBones = mainModel->getNamesBones();

	/*ustawienie modelState na 1 konfiguracje*/
	mainModel->setModelState(modelState[0]);

	/*ustawienie wielkosci wektorow przecowujacych stany poszczegolnych konfiguracji na rozmiar rowny liczbie klatek tla pierwszego okna */
	if (glWidgetsVector[0]->isAvi)
		updateModelStateVectorSize(glWidgetsVector[0]->aviFrames.size());
	else
		updateModelStateVectorSize(glWidgetsVector[0]->list.size());

	/*rotacja elementow zgodna z plikiem dat (w bonesGeometry moga wystapic elementy ktorych nie ma w bonesConfig)*/
	justLaunched = true;

	/*odswiez model - rozmiar modelState mogl sie zmienic*/
	for (int i = 0; i < glWidgetsVector.size(); i++) {
		if (asfLoaded)
			fileHandler.reloadParams(glWidgetsVector[i], bonesConf, bonesGeometry, asfPath);
		else
			fileHandler.reloadParams(glWidgetsVector[i], bonesConf, bonesGeometry, ASF_TEMPLATE_PATH);

		updateGLWidgetDrawning(i);
	}

	modelHandler.setLimitsVector(limits, bonesConf, bonesGeometry);
	modelHandler.setVelocityVector(velocity, bonesConf, bonesGeometry);

	modelHandler.saveLimitsToMap(modelLimits, bonesConf);
	modelHandler.saveVelocityToMap(modelVelocity, bonesConf);

	/*zaladuj ponownie boczne*/
	reloadGLWidgetMenu();
	/*ustaw dlugosci kosci na podstawie wartosci line edit*/
	for (int i = 0; i < bonesGeometry.size(); i++) {
		QString str = QString::fromStdString(bonesGeometry[i].name) + ";" + QString::number(i);
		setBoneLengthFromLineEdit(str);
		//glWidgetsVector[i]->update();
	}
}

void MainWindow::saveAMCToFile() {
	FileHandler fileHandler;// = new FileHandler();
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save as .amc"), "",
		tr("AMC (*.amc);;All Files(*)"));

	if (!fileName.isEmpty()) {
		//glWidgetsVector[id]->updateUsedBones(usedBones, bonesConf);

		fileHandler.saveAMCSeq(
			modelState,
			asfBones,
			usedBones,
			fileName,
			saveModelState
		);
		
		QMessageBox::information(this, "AMC saved", "AMC file saved.", QMessageBox::Ok);
	}

	//delete fileHandler;
}

void MainWindow::saveAMCFromQStringToFile(QString fileName) {
	FileHandler fileHandler;

	if (!fileName.isEmpty()) {
		//glWidgetsVector[id]->updateUsedBones(usedBones, bonesConf);

		fileHandler.saveAMCSeq(
			modelState,
			asfBones,
			usedBones,
			fileName,
			saveModelState
		);

	}
}

void MainWindow::loadFromAmc() {
	QString filePath = QFileDialog::getOpenFileName(this,
		tr("Open .amc file"), "",
		tr("AMC (*.amc);;All Files(*)"));

	if (!filePath.isEmpty()) {
		loadAMC(filePath);
		
		QMessageBox::information(this, "AMC loaded", "AMC file loaded.", QMessageBox::Ok);
	}

}

void MainWindow::loadFromDroppedAMC(QString str) {
	FileHandler fileHandler;// = new FileHandler();

	if (!str.isEmpty()) {
		loadAMC(str);

		QMessageBox::information(this, "AMC loaded", "AMC file loaded.", QMessageBox::Ok);
	}

	//delete fileHandler;
}

void MainWindow::loadAMC(QString filePath) {
	FileHandler fileHandler;
	ModelHandler modelHandler;
	amcLoaded = true;
	amcPath = filePath;
	vector<string> bones;
	/*wektor state zawiera tylko zaakceptowane wczesniej konfiguracje*/
	vector<vector<float>> states = fileHandler.loadAmcFromFile(filePath, allBones, saveModelState, bones);

	int currentModelState = 0;
	int imagesCounter;
	/*zapis do mapy*/
	/*jesli liczba klatek w danym oknie jest wieksza nic liczba wczytanych modelState to zostaja domyslne,
	w przeciwnym przypadku modelState wczytywane jest do momentu przekroczenia iteratora klatek*/
	for (int j = 0; j < saveModelState.size(); j++) {
		if (saveModelState[j]) {
			modelHandler.saveModelStateToMap(bonesRotations[j], states[currentModelState], bones);
			currentModelState++;

			/*odczyt z mapy do glwidget modelState*/
			modelHandler.updateModelStateFromMap(modelState, bonesRotations, bonesConf, modelTranslation);
		}
	}

	for (int id = 0; id < glWidgetsVector.size(); id++)
		updateGLWidgetDrawning(id);

	/*odswiez rotate menu po wczytaniu amc*/
	refreshRotateSideMenu();
}

void MainWindow::saveAsfToFile(){
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save as .asf"), "",
		tr("ASF (*.asf);;All Files(*)"));

	if (!fileName.isEmpty()) {
		pf::MotionData::saveASF(fileName.toUtf8().constData(), asfBones);
		QMessageBox::information(this, "ASF saved", "ASF file saved.", QMessageBox::Ok);
	}
}

void MainWindow::saveAsfFromQStringToFile(QString fileName) {
	if (!fileName.isEmpty()) {
		pf::MotionData::saveASF(fileName.toUtf8().constData(), asfBones);
	}
}

void MainWindow::loadFromASF() {
	QString filePath = QFileDialog::getOpenFileName(this,
		tr("Open .asf file"), "",
		tr("ASF (*.asf);;All Files(*)"));

	if (!filePath.isEmpty()) {
		loadASF(filePath);

		QMessageBox::information(this, "ASF loaded", "ASF file loaded.", QMessageBox::Ok);
	}
}

void MainWindow::loadFromDroppedASF(QString str) {
	if (!str.isEmpty()) {
		loadASF(str);

		QMessageBox::information(this, "ASF loaded", "ASF file loaded.", QMessageBox::Ok);
	}
}

void MainWindow::loadASF(QString filePath) {
	asfLoaded = true;
	asfPath = filePath;
	idxBonesMap.clear();
	asfBones.clear();
	pf::MotionData::loadASF(filePath.toUtf8().constData(), idxBonesMap, asfBones);

	/*ponownie zainicjalizuj mapy*/
	ModelHandler modelHandler;
	modelHandler.initializeBonesDOFMap(modelDOF, asfBones);
	modelHandler.initializeBonesRadiusMap(modelRadius, asfBones, bonesGeometry);
	modelHandler.initializeBonesLengthMap(bonesLength, asfBones);
	modelHandler.initializeBonesLimitsMap(modelLimits, asfBones);
	modelHandler.initializeBonesVelocityMap(modelVelocity, asfBones);

	bonesRotations.clear();
	modelTranslation.clear();

	map<string, pf::Vec3f> bonesRotationsTMP;
	vector<float> modelTranslationTMP;
	bonesRotationsTMP["tmp"] = pf::Vec3f(0, 0, 0);
	modelTranslation.push_back(modelTranslationTMP);

	bonesRotations.push_back(bonesRotationsTMP);
	modelHandler.initializeBonesRotationsMap(bonesRotations[0], asfBones);
	modelTranslation[0].push_back(0);
	modelTranslation[0].push_back(0);
	modelTranslation[0].push_back(0);
}

void MainWindow::buttonAddFramePressed(){
	vector<vector<float>> rad;
	vector<vector<pf::Vec3f>> ver;
	rad = this->mainModel->getRadiusVec();
	ver = this->mainModel->getRotatedVertices();
    addFrameMenu(addFrameButtonCounter);
	comboBoxFrameSelector->addItem(tr(std::to_string(addFrameButtonCounter).c_str()));

	/*po dodaniu nowego okna ustaw modelState[0]*/
	glWidgetsVector[addFrameButtonCounter]->model->setModelState(modelState[0]);
	glWidgetsVector[addFrameButtonCounter]->setGLWidgetModelState(imagesListIterator[addFrameButtonCounter] - 1, modelState);
	glWidgetsVector[addFrameButtonCounter]->setRadiusVertices(ver, rad);
	glWidgetsVector[addFrameButtonCounter]->update();

	if (frameDeleted) {
		/*dla pierwszego okna zainicjalizuj boczne menu*/
		if (addFrameButtonCounter == 0) {
			addGLWidgetRotateMenu();
			addGLWidgetLengthMenu();
			addGLWidgetGeometryMenu();
		}
	}
	addFrameButtonCounter++;
}

void MainWindow::addFrameMenu(int i){
    int j=0;
    bordersWidgetsVector.push_back(new QWidget(this));
	frameIDVector.push_back(new QLabel(this));
    glWidgetsVector.push_back(new GLWidget(this));
	glWidgetLayoutVector.push_back(new QHBoxLayout);
	glWidgetScrollAreaVector.push_back(new QScrollArea(this));
	glWidgetsWidgetVector.push_back(new QWidget(this));
	horizontalGLWidgetMenuVector.push_back(new QHBoxLayout);
	menuButtonsLayoutVector.push_back(new QHBoxLayout);
	hMenuWidgetVector.push_back(new QWidget(this));
	slidersVector.push_back(new QSlider(this));
	sliderCheckBoxesVector.push_back(new QCheckBox(this));
    minButtonsVector.push_back(new QPushButton(this));
    maxButtonsVector.push_back(new QPushButton(this));
    reduceButtonsVector.push_back(new QPushButton(this));
    increaseButtonsVector.push_back(new QPushButton(this));
	playPauseButtonsVector.push_back(new QPushButton(this));
	playPressedVector.push_back(false);
    countersVector.push_back(new QLabel(this));
    imagesListIterator.push_back(0);
	menuButtonsVector.push_back(new QPushButton("Menu"));
	backgroundTimersVector.push_back(new QTimer(this));

	/*rozmiar widgetu jest edytowalny*/
	glWidgetScrollAreaVector[addFrameButtonCounter]->setWidgetResizable(true);
	glWidgetScrollAreaVector[addFrameButtonCounter]->setFrameShape(QFrame::NoFrame);

	glWidgetsWidgetVector[addFrameButtonCounter]->setLayout(glWidgetLayoutVector[addFrameButtonCounter]);
	glWidgetScrollAreaVector[addFrameButtonCounter]->setWidget(glWidgetsWidgetVector[addFrameButtonCounter]);

	/*ustawienie kosci dodanego modelu*/
	FileHandler fileHandler;// = new FileHandler();
	if(asfLoaded)
		fileHandler.reloadParams(glWidgetsVector[addFrameButtonCounter], bonesConf, bonesGeometry, asfPath);
	else 
		fileHandler.reloadParams(glWidgetsVector[addFrameButtonCounter], bonesConf, bonesGeometry, ASF_TEMPLATE_PATH);

	update();
	//delete fileHandler;

	/*domyslny rozmiar widgetu*/
	glWidgetsVector[addFrameButtonCounter]->setFixedSize(450, 333);

	glWidgetScrollAreaVector[addFrameButtonCounter]->setMinimumSize(100, 300);
	glWidgetScrollAreaVector[addFrameButtonCounter]->setMaximumHeight(400);
	glWidgetScrollAreaVector[addFrameButtonCounter]->setMaximumWidth(1200);

	glWidgetLayoutVector[addFrameButtonCounter]->addWidget(glWidgetsVector[addFrameButtonCounter]);
	glWidgetLayoutVector[addFrameButtonCounter]->setAlignment(Qt::AlignTop | Qt::AlignLeft);

	/*dodanie ikon do przyciskow*/
	addFrameMenuButtonsImages(minButtonsVector[addFrameButtonCounter],
		reduceButtonsVector[addFrameButtonCounter],
		increaseButtonsVector[addFrameButtonCounter],
		maxButtonsVector[addFrameButtonCounter],
		playPauseButtonsVector[addFrameButtonCounter]);

	/*dodanie obslugi przyciskow*/
	addFrameMenuMappers(addFrameButtonCounter);

	/*rozmiar label wyswietlajacy numer obiektu*/
	QFont font = frameIDVector[addFrameButtonCounter]->font();
	font.setPointSize(25);

	bordersWidgetsVector[addFrameButtonCounter]->setStyleSheet("border: 1px solid black");
	frameIDVector[addFrameButtonCounter]->setText(" " + QString::number(addFrameButtonCounter));
	frameIDVector[addFrameButtonCounter]->setFont(font);

	frameIDVector[addFrameButtonCounter]->setStyleSheet("color: red");

	/*rozmiar przyciskow menu*/
	minButtonsVector[addFrameButtonCounter]->setFixedSize(30,20);
	maxButtonsVector[addFrameButtonCounter]->setFixedSize(30, 20);
	reduceButtonsVector[addFrameButtonCounter]->setFixedSize(30, 20);
	increaseButtonsVector[addFrameButtonCounter]->setFixedSize(30, 20);
	playPauseButtonsVector[addFrameButtonCounter]->setFixedSize(20, 20);
	menuButtonsVector[addFrameButtonCounter]->setFixedSize(55, 25);

	/*rozwijane menu*/
	menuButtonsLayoutVector[addFrameButtonCounter]->addWidget(menuButtonsVector[addFrameButtonCounter]);
	menuButtonsLayoutVector[addFrameButtonCounter]->setContentsMargins(0, 0, 5, 5);

	/*dolny panel menu - przyciski do iteracji klatek, slider, menubutton*/
	horizontalGLWidgetMenuVector[addFrameButtonCounter]->addWidget(minButtonsVector[addFrameButtonCounter]);
	horizontalGLWidgetMenuVector[addFrameButtonCounter]->addWidget(reduceButtonsVector[addFrameButtonCounter]);
	horizontalGLWidgetMenuVector[addFrameButtonCounter]->addWidget(countersVector[addFrameButtonCounter]);
	horizontalGLWidgetMenuVector[addFrameButtonCounter]->addWidget(increaseButtonsVector[addFrameButtonCounter]);
	horizontalGLWidgetMenuVector[addFrameButtonCounter]->addWidget(maxButtonsVector[addFrameButtonCounter]);
	horizontalGLWidgetMenuVector[addFrameButtonCounter]->addWidget(playPauseButtonsVector[addFrameButtonCounter]);
	horizontalGLWidgetMenuVector[addFrameButtonCounter]->addWidget(sliderCheckBoxesVector[addFrameButtonCounter]);
	horizontalGLWidgetMenuVector[addFrameButtonCounter]->addWidget(slidersVector[addFrameButtonCounter]);

	/*gdy nie ma wybranego tla panel jest ukryty*/
	hMenuWidgetVector[addFrameButtonCounter]->setLayout(horizontalGLWidgetMenuVector[addFrameButtonCounter]);
	hMenuWidgetVector[addFrameButtonCounter]->hide();

	addFrameScrollingMenu(menuButtonsVector[addFrameButtonCounter], addFrameButtonCounter);
    prepareFrameMenu(addFrameButtonCounter);

	/*dodanie kolejnych okien do gridChildLayout*/
    /*rows, cols, rowspan, colspan*/
    if(i%2==0){
        if(i>1) j=(i/2)*2;
		gridChildLayout->addWidget(bordersWidgetsVector[addFrameButtonCounter], (i / 2) * 10 + j, 0, 12, 10);
		gridChildLayout->addWidget(glWidgetScrollAreaVector[addFrameButtonCounter], (i / 2) * 10 + j, 0, 10, 10);
		gridChildLayout->addWidget(hMenuWidgetVector[addFrameButtonCounter], (i / 2) * 10 + 11 + j, 0, 1, 9);
		gridChildLayout->addLayout(menuButtonsLayoutVector[addFrameButtonCounter], (i / 2) * 10 + 11 + j, 9, 1, 1);
		gridChildLayout->addWidget(frameIDVector[addFrameButtonCounter], (i / 2) * 10 + j, 0, 2, 2);
    }else {
        if(i>1) j=(i/2)*2;
		gridChildLayout->addWidget(bordersWidgetsVector[addFrameButtonCounter], (i / 2) * 10 + j, 10, 12, 10);
		gridChildLayout->addWidget(glWidgetScrollAreaVector[addFrameButtonCounter], (i / 2) * 10 + j, 10, 10, 10);
		gridChildLayout->addWidget(hMenuWidgetVector[addFrameButtonCounter], (i / 2) * 10 + 11 + j, 10, 1, 9);
		gridChildLayout->addLayout(menuButtonsLayoutVector[addFrameButtonCounter], (i / 2) * 10 + 11 + j, 19, 1, 1);
		gridChildLayout->addWidget(frameIDVector[addFrameButtonCounter], (i / 2) * 10 + j, 10, 2, 2);
    }

	/*jesli ilosc okien > 0, edycja ilosci kosci modelu jest mozliwa*/
	if (comboBoxFrameSelector->count() == 0) {
		manageModelBonesAction->setEnabled(true);
	}

	/*jesli zostaly dodane lub wczytane kamery menu jest aktywne*/
	if (cameras.size() > 0) {
		addCameraMenuToFrame();
		selectCameraMenuVector[i]->setDisabled(false);
	}
	/*jesli do aplikacji zostalo dodane okno, aktywuj mozliwosc usuniecia*/
	if (glWidgetsVector.size() > 1) {
		for(int id = 0;id < glWidgetsVector.size();id++)
			deleteFrameMenuVector[id]->setDisabled(false);
	}
	/*jesli wybrana zostala nowa rozdzielczosc tla to ja ustaw dla nowego okna*/
	if (resizeBackground) {
		glWidgetsVector[addFrameButtonCounter]->scaleFrame = true;
		glWidgetsVector[addFrameButtonCounter]->frameWidth = backgroundWidth;
		glWidgetsVector[addFrameButtonCounter]->frameHeight = backgroundHeight;
	}
}

void MainWindow::setTextIteratorLabel(QLabel *label, int iterator, int listSize) {
	QString string = QString("%1 / %2").arg(iterator).arg(listSize);
	label->setText(string);
	label->setAlignment(Qt::AlignCenter);
	label->setFixedWidth(50);
}

void MainWindow::increaseIteratorPressed(int i) {
	/*aktualny id okna*/
	int listIterator = imagesListIterator[i];
	/*czy tlo jest plikiem avi*/
	if (!glWidgetsVector[i]->isAvi) {
		if (listIterator < glWidgetsVector[i]->list.size())
			listIterator++;
		else
			listIterator = glWidgetsVector[i]->list.size();

		setTextIteratorLabel(countersVector[i], listIterator, glWidgetsVector[i]->list.size());
		imagesListIterator[i] = listIterator;
		/*sciezka do pliku tla*/
		QString imgString = glWidgetsVector[i]->list[listIterator - 1].absoluteFilePath();
		glWidgetsVector[i]->imgPath = imgString.toUtf8().constData();
	}

	else {
		if (listIterator < glWidgetsVector[i]->aviFrames.size())
			listIterator++;
		else
			listIterator = glWidgetsVector[i]->aviFrames.size();

		setTextIteratorLabel(countersVector[i], listIterator, glWidgetsVector[i]->aviFrames.size());
		imagesListIterator[i] = listIterator;

		/*ustaw mat o wybranym id*/
		glWidgetsVector[i]->aviFrames[listIterator - 1].copyTo(glWidgetsVector[i]->aviFrame);
	}

	slidersVector[i]->setValue(imagesListIterator[i]);

	setGLWidgetDrawing(i);
}

void MainWindow::reduceIteratorPressed(int i) {
	/*aktualny iterator okna*/
	int listIterator = imagesListIterator[i];
	/*czy tlo jest plikiem avi*/
	if (!glWidgetsVector[i]->isAvi) {
		if (listIterator > 1)
			listIterator--;
		else
			listIterator = 1;

		setTextIteratorLabel(countersVector[i], listIterator, glWidgetsVector[i]->list.size());
		imagesListIterator[i] = listIterator;
		/*sciezka do pliku tla*/
		QString imgString = glWidgetsVector[i]->list[listIterator - 1].absoluteFilePath();
		glWidgetsVector[i]->imgPath = imgString.toUtf8().constData();
	}

	else {
		if (listIterator > 1)
			listIterator--;
		else
			listIterator = 1;

		setTextIteratorLabel(countersVector[i], listIterator, glWidgetsVector[i]->aviFrames.size());
		imagesListIterator[i] = listIterator;

		/*ustaw mat o wybranym id*/
		glWidgetsVector[i]->aviFrames[listIterator - 1].copyTo(glWidgetsVector[i]->aviFrame);
	}
	slidersVector[i]->setValue(imagesListIterator[i]);

	setGLWidgetDrawing(i);
}

void MainWindow::minIteratorPressed(int i) {
	/*iterator okna ustaw na 1*/
	imagesListIterator[i] = 1;
	/*czy tlo jest plikiem avi*/
	if (!glWidgetsVector[i]->isAvi) {
		setTextIteratorLabel(countersVector[i], imagesListIterator[i], glWidgetsVector[i]->list.size());
		/*sciezka do pliku tla*/
		QString imgString = glWidgetsVector[i]->list[imagesListIterator[i] - 1].absoluteFilePath();
		glWidgetsVector[i]->imgPath = imgString.toUtf8().constData();
	}
	else {
		setTextIteratorLabel(countersVector[i], imagesListIterator[i], glWidgetsVector[i]->aviFrames.size());
		/*ustaw mat o wybranym id*/
		glWidgetsVector[i]->aviFrames[imagesListIterator[i] - 1].copyTo(glWidgetsVector[i]->aviFrame);
	}

	slidersVector[i]->setValue(imagesListIterator[i]);

	scrollRotateSideArea->setDisabled(false);
	updateGLWidgetDrawning(i);
	refreshRotateSideMenu();
}

void MainWindow::maxIteratorPressed(int i) {
	/*iterator okna ustaw na max*/
	/*czy plik tla jest avi*/
	if (!glWidgetsVector[i]->isAvi) {
		
		imagesListIterator[i] = glWidgetsVector[i]->list.size();
		setTextIteratorLabel(countersVector[i], imagesListIterator[i], glWidgetsVector[i]->list.size());
		/*sciezka do pliku*/
		QString imgString = glWidgetsVector[i]->list[imagesListIterator[i] - 1].absoluteFilePath();
		glWidgetsVector[i]->imgPath = imgString.toUtf8().constData();
	}
	else {
		imagesListIterator[i] = glWidgetsVector[i]->aviFrames.size();
		setTextIteratorLabel(countersVector[i], imagesListIterator[i], glWidgetsVector[i]->aviFrames.size());
		/*ustaw mat o wybranym id*/
		glWidgetsVector[i]->aviFrames[imagesListIterator[i] - 1].copyTo(glWidgetsVector[i]->aviFrame);
	}

	slidersVector[i]->setValue(imagesListIterator[i]);

	setGLWidgetDrawing(i);
}

void MainWindow::playPausePressed(int i) {
	/*przycisk polaczony z timerem*/

	QPixmap pixPause(":/images/ETC/buttonImages/pause.png"), pixPlay(":/images/ETC/buttonImages/play.png");
	QIcon iconPause(pixPause), iconPlay(pixPlay);
	/*jesli wcisnieto play*/
	if (playPressedVector[i] == false) {
		playPauseButtonsVector[i]->setIcon(pixPause);

		if (glWidgetsVector[i]->isAvi) {
			/*jesli iterator doszedl do max */
			if (imagesListIterator[i] == glWidgetsVector[i]->aviFrames.size()) {
				backgroundTimersVector[i]->start(1);
			}
			else {
				backgroundTimersVector[i]->start(timerValue);
			}
		}
		else {
			if (imagesListIterator[i] == glWidgetsVector[i]->list.size()) {
				backgroundTimersVector[i]->start(1);
			}
			else {
				backgroundTimersVector[i]->start(timerValue);
			}
		}
		
	}
	/*jesli wcisnieto pauze*/
	else {
		playPauseButtonsVector[i]->setIcon(pixPlay);
		backgroundTimersVector[i]->stop();
	}

	if (playPressedVector[i] == true)
		playPressedVector[i] = false;
	else
		playPressedVector[i] = true;
}

void MainWindow::manageBonesPressed() {
	BoneManagment *boneManagmentWindow = new BoneManagment(this);

	boneManagmentWindow->setWindowTitle("Manage model bones");

	/*pobierz id aktualnych kosci modelu*/
	vector<int> usedBonesIDs = boneManagmentWindow->getUsedBonesIDs(bonesGeometry, allBones);
	/*ustaw checkboxy kosci*/
	boneManagmentWindow->setUsedBones(asfBones, usedBonesIDs);
	
	boneManagmentWindow->resize(500, 400);
	boneManagmentWindow->exec();

	/*id oraz nazwy wybranych kosci*/
	vector<int> bonesIDs = boneManagmentWindow->selectedBonesIDs;
	vector<string> bonesNames = boneManagmentWindow->selectedBonesNames;

	/*ustawienie kolejnosci wyswietlania kosci*/
	boneManagmentWindow->matchBonesWithASF(bonesIDs, bonesNames, asfBones);
	if (!bonesIDs.empty() && !bonesNames.empty()) {

		FileHandler fileHandler;// = new FileHandler();
		ModelHandler modelHandler;
		/*aktualizacja map*/
		for (int j = 0; j < bonesRotations.size(); j++) {
			modelHandler.saveModelStateToMap(bonesRotations[j], modelState[j], bonesConf);
		}
		modelHandler.saveBonesLengthToMap(bonesLength, bonesGeometry);
		modelHandler.saveRadiusToMap(modelRadius, bonesGeometry);
		modelHandler.saveDOFToMap(modelDOF, bonesConf);
		modelHandler.saveLimitsToMap(modelLimits, bonesConf);
		modelHandler.saveVelocityToMap(modelVelocity, bonesConf);

		bonesConf.clear();
		bonesGeometry.clear();

		for (int j = 0; j < bonesIDs.size(); j++) {
			/*Dodanie nowego obiektu boneConfig - wykorzystywany do tworzenia pliku dat*/
			fileHandler.addBoneConf(bonesConf,
				idxBonesMap,
				asfBones,
				bonesIDs[j]);
				
			/*Dodanie nowego obiektu boneGeometry*/
			fileHandler.addBoneGeo(bonesGeometry,
				idxBonesMap,
				asfBones,
				bonesIDs[j]);
		}

		/*aby mozliwe bylo uzycie nowej konfiguracji kosci nalezy ja zapisac i nadpisac aktualna*/
		fileHandler.saveDATToFile(bonesConf, bonesGeometry, "tmpDatFile.dat");
		bonesConf.clear();
		bonesGeometry.clear();
		mainModel = new pf::Model3D(pf::Model3D::Cylinder, ASF_TEMPLATE_PATH, "tmpDatFile.dat");
		mainModel->loadConfig("tmpDatFile.dat", bonesConf, bonesGeometry);

		limits = mainModel->getLimits();
		usedBones = mainModel->getNamesMovingBones();
		allBones = mainModel->getNamesBones();

		for (int j = 0; j < glWidgetsVector.size(); j++) {

			glWidgetsVector[j]->model = new pf::Model3D(pf::Model3D::Cylinder, ASF_TEMPLATE_PATH, "tmpDatFile.dat");
			glWidgetsVector[j]->usedBones = usedBones;
			glWidgetsVector[j]->asfBones = asfBones;
			glWidgetsVector[j]->idxBonesMap = idxBonesMap;
		}
		std::remove("tmpDatFile.dat");
		/*ustawienie dlugosci wybranych kosci*/
		for (int j = 0; j < bonesGeometry.size(); j++) {

			pf::boneGeometry bGeo = bonesGeometry[j];

			/*czy checkbox kosci zostal zaznaczony*/
			if (fileHandler.isBoneChecked(bGeo.name, bonesNames)) {
				mainModel->updateBoneGeometry(bGeo.name, bGeo);
			}
			else {}
		}
		/*aktualizacja map*/
		try {
			modelHandler.updateBonesLengthFromMap(bonesLength, bonesGeometry);
			modelHandler.updateBonesRadiusFromMap(modelRadius, bonesGeometry);
			modelHandler.updateDOFFromMap(modelDOF, bonesConf);
			modelHandler.updateLimitsFromMap(modelLimits, bonesConf);
			modelHandler.updateVelocityFromMap(modelVelocity, bonesConf);
		}
		catch (MyException& e) {
			cerr << e.what() << endl;
		}
		/*aktualizacja model state*/
		modelHandler.updateModelStateFromMap(modelState, bonesRotations, bonesConf, modelTranslation);

		
		/*spine1_dum1 i spine1 sa swoim 'przedluzeniem'*/
		if (std::find(bonesNames.begin(), bonesNames.end(), "Spine1") != bonesNames.end()) {
			if (std::find(bonesNames.begin(), bonesNames.end(), "Spine1_dum1") != bonesNames.end()) {
				float spine1Length, spine1_dum1Length;
				spine1Length = bonesLength.find("Spine1_dum1")->second;
				spine1_dum1Length = bonesLength.find("Spine1")->second;

				bonesLength.at("Spine1_dum1") = spine1_dum1Length;
				bonesLength.at("Spine1") = spine1Length;
				modelHandler.updateBonesLengthFromMap(bonesLength, bonesGeometry);
			}
		}

		reloadGLWidgetMenu();
		updateBoneConfigFromCB();
		updateBoneConfigFromLimitsSB();
		updateBoneConfigFromVelocitySB();
	}

	refreshLengthSideMenu();
	delete boneManagmentWindow;

}

void MainWindow::openCameraConfigPressed() {
	CamerasConfig *cfg = new CamerasConfig(this);
	/*czy kamery zostaly wczesniej uzyte, wczytane*/
	cfg->firstTime = cameraFirstTime;
	cfg->resize(500, 400);
	/*jesli jakies kamery zostaly wczesniej dodane*/
	if (cameras.size() > 0) {
		cfg->useQuat = this->useQuat;
		cfg->getCamerasFromMainWindow(cameras);
	}
	cfg->exec();
	if (cfg->okClicked) {
		cfg->returnCameras(cameras);
		this->useQuat = cfg->useQuat;
		if (cameras.size() > 0) {
			cameraFirstTime = false;
			/*aktywacja menu*/
			for (int i = 0; i < glWidgetsVector.size(); i++) 
				selectCameraMenuVector[i]->setDisabled(false);

			setCamerasMenu();
			QMessageBox::information(this, "Cameras loaded", "Changes saved.", QMessageBox::Ok);
		}
	}

	//reloadGLWidgetMenu();
	delete cfg;
}

void MainWindow::saveCamerasPressed() {
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save cameras"), "",
		tr("INI (*.ini);;All Files(*)"));

	if (!fileName.isEmpty()) {
		saveCameras(fileName);
		QMessageBox::information(this, "Cameras saved", "Cameras saved successfully.", QMessageBox::Ok);
	}
}

void MainWindow::loadCamerasPressed() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Load cameras configuration"), "", tr("(*.ini)"));

	if (!fileName.isEmpty()) {
		loadCameras(fileName);
		cameraFirstTime = false;
		//reloadGLWidgetMenu();
		QMessageBox::information(this, "Cameras loaded", "Cameras loaded successfully.", QMessageBox::Ok);
	}
}

void MainWindow::loadCamerasFromDroppedINI(QString str) {
	if (!str.isEmpty()) {
		loadCameras(str);
		cameraFirstTime = false;
		//reloadGLWidgetMenu();
		QMessageBox::information(this, "Cameras loaded", "Cameras loaded successfully.", QMessageBox::Ok);
	}
}

void MainWindow::saveProjectPressed() {
	QString filePath = QFileDialog::getSaveFileName(this,
		tr("Save project"), "",
		tr("INI (*.ini);;All Files(*)"));

	if (!filePath.isEmpty()) {
		QString fileDir = QFileInfo(filePath).absolutePath();
		QString fileName = QFileInfo(filePath).baseName();
		QDir().mkdir(fileDir + "/etc");
		QString amc = fileDir + "/ETC/amc" + fileName + ".amc";
		QString asf = fileDir + "/ETC/asf" +  fileName + ".asf";
		QString dat = fileDir + "/ETC/dat" + fileName + ".dat";
		QString cameras = fileDir + "/ETC/cameras" + fileName + ".ini";
		saveProject(filePath, asf, amc, dat, cameras);
		QMessageBox::information(this, "Project saved", "Project saved successfully.", QMessageBox::Ok);
	}
}

void MainWindow::loadProjectPressed() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Load cameras configuration"), "", tr("(*.ini)"));

	if (!fileName.isEmpty()) {
		loadProject(fileName);
		QMessageBox::information(this, "Project loaded", "Project loaded successfully.", QMessageBox::Ok);
	}
}

void MainWindow::configPressed() {
	ConfigDialog *cfg = new ConfigDialog(this);

	/*ustaw wartosci widgetow configu*/
	cfg->setJointRadiusSpinBoxValue(glWidgetsVector[0]->jointRadius);
	cfg->setRadiusCheckBox(glWidgetsVector[0]->drawJoints);

	if(!glWidgetsVector[0]->drawJoints)
		cfg->disableJointSpinBox();

	cfg->setTimerSpinBox(timerValue);

	cfg->setResolutionCheckBox(resizeBackground);
	cfg->setResolutionSpinBoxes(backgroundWidth, backgroundHeight);

	cfg->exec();

	if (cfg->okClicked) {
		/*czy rysowac stawy*/
		bool draw = cfg->drawJointsInGlWidgets;
		if (draw) {
			int radius = cfg->getJointsRadius();
			for (int i = 0; i < glWidgetsVector.size(); i++) {
				glWidgetsVector[i]->drawJoints = true;
				glWidgetsVector[i]->jointRadius = radius;
				glWidgetsVector[i]->update();
			}
		}
		else {
			for (int i = 0; i < glWidgetsVector.size(); i++) {
				glWidgetsVector[i]->drawJoints = false;
				glWidgetsVector[i]->update();
			}
		}
		/*rozmiar tla*/
		resizeBackground = cfg->resizeBackgroundRes;
		if (resizeBackground) {
			backgroundWidth = cfg->getWidthSpinBox();
			backgroundHeight = cfg->getHeightSpinBox();
			for (int i = 0; i < glWidgetsVector.size(); i++) {
				/*skaluj wektor mat jesli rozmiar nie jest taki sam*/
				if ((glWidgetsVector[i]->frameWidth != backgroundWidth || glWidgetsVector[i]->frameHeight != backgroundHeight) && glWidgetsVector[i]->isAvi) {
					glWidgetsVector[i]->scaleFrame = true;
					glWidgetsVector[i]->frameWidth = backgroundWidth;
					glWidgetsVector[i]->frameHeight = backgroundHeight;
					glWidgetsVector[i]->aviFrames = glWidgetsVector[i]->scaleAviBackground();
				}
				/*zapisz parametry*/
				else {
					glWidgetsVector[i]->scaleFrame = true;
					glWidgetsVector[i]->frameWidth = backgroundWidth;
					glWidgetsVector[i]->frameHeight = backgroundHeight;
				}
			}
		}
		/*timer*/
		timerValue = cfg->getTimerSpinBoxValue();
		QMessageBox::information(this, "Config", "Config saved.", QMessageBox::Ok);
	}

	delete cfg;
}

void MainWindow::selectAviBackgroundPressed(int i) {
	/*wczytaj avi*/
	glWidgetsVector[i]->loadAviFile();
	if (glWidgetsVector[i]->isAvi) {
		imagesListIterator[i] = 1;
		sliderCheckBoxesVector[i]->setChecked(true);

		/*jesli ilosc klatek przekracza max wartosc glownego slidera to ustaw nowy max*/
		if (globalSlider->maximum() < glWidgetsVector[i]->aviFrames.size())
			globalSlider->setMaximum(glWidgetsVector[i]->aviFrames.size());

		setTextIteratorLabel(countersVector[i], imagesListIterator[i], glWidgetsVector[i]->aviFrames.size());
		/*ustaw mat*/
		glWidgetsVector[i]->aviFrames[imagesListIterator[i] - 1].copyTo(glWidgetsVector[i]->aviFrame);
		glWidgetsVector[i]->drawBckg = true;

		prepareSlider(slidersVector[i], i);

		hMenuWidgetVector[i]->show();

		/*jesli wczytano tlo do pierwszego okna to ustal nowy rozmiar wektora modelState*/
		if (i == 0) {
			updateModelStateVectorSize(glWidgetsVector[i]->aviFrames.size());
		}

		QMessageBox::information(this, "Avi loaded", "Avi file loaded successfully", QMessageBox::Ok);
	}
	else {
		hMenuWidgetVector[i]->hide();
	}
}

void MainWindow::loadAviDropped(int i) {
	if (glWidgetsVector[i]->isAvi) {
		imagesListIterator[i] = 1;
		sliderCheckBoxesVector[i]->setChecked(true);

		/*jesli ilosc klatek przekracza max wartosc glownego slidera to ustaw nowy max*/
		if (globalSlider->maximum() < glWidgetsVector[i]->aviFrames.size())
			globalSlider->setMaximum(glWidgetsVector[i]->aviFrames.size());

		setTextIteratorLabel(countersVector[i], imagesListIterator[i], glWidgetsVector[i]->aviFrames.size());
		glWidgetsVector[i]->aviFrames[imagesListIterator[i] - 1].copyTo(glWidgetsVector[i]->aviFrame);
		glWidgetsVector[i]->drawBckg = true;

		prepareSlider(slidersVector[i], i);

		hMenuWidgetVector[i]->show();

		/*jesli wczytano tlo do pierwszego okna to ustal nowy rozmiar wektora modelState*/
		if (i == 0) {
			updateModelStateVectorSize(glWidgetsVector[i]->aviFrames.size());
		}

		QMessageBox::information(this, "Avi loaded", "Avi file loaded successfully", QMessageBox::Ok);
	}
	else {
		hMenuWidgetVector[i]->hide();
	}
}

void MainWindow::selectImagesBackgroundPressed(int i) {
	/*wczytaj obrazy tla*/
	glWidgetsVector[i]->loadFiles();
	if (!glWidgetsVector[i]->list.isEmpty()) {
		if (!glWidgetsVector[i]->isAvi) {
			imagesListIterator[i] = 1;
			sliderCheckBoxesVector[i]->setChecked(true);

			/*jesli ilosc klatek przekracza max wartosc glownego slidera to ustaw nowy max*/
			if (globalSlider->maximum() < glWidgetsVector[i]->list.size())
				globalSlider->setMaximum(glWidgetsVector[i]->list.size());

			setTextIteratorLabel(countersVector[i], imagesListIterator[i], glWidgetsVector[i]->list.size());
			/*ustaw sciezke tla*/
			glWidgetsVector[i]->imgPath = glWidgetsVector[i]->list[imagesListIterator[i] - 1].absoluteFilePath().toUtf8().constData();
			glWidgetsVector[i]->drawBckg = true;

			prepareSlider(slidersVector[i], i);

			hMenuWidgetVector[i]->show();

			/*jesli wczytano tlo do pierwszego okna to ustal nowy rozmiar wektora modelState*/
			if (i == 0) {
				updateModelStateVectorSize(glWidgetsVector[i]->list.size());
			}

			QMessageBox::information(this, "Images loaded", "Images loaded successfully", QMessageBox::Ok);
		}
		else {
			hMenuWidgetVector[i]->hide();
		}
	}
}

void MainWindow::loadImagesBackgroundDropped(int i) {
	/*wczytaj obrazy tla*/
	if (!glWidgetsVector[i]->list.isEmpty()) {
		if (!glWidgetsVector[i]->isAvi) {
			imagesListIterator[i] = 1;
			sliderCheckBoxesVector[i]->setChecked(true);

			/*jesli ilosc klatek przekracza max wartosc glownego slidera to ustaw nowy max*/
			if (globalSlider->maximum() < glWidgetsVector[i]->list.size())
				globalSlider->setMaximum(glWidgetsVector[i]->list.size());

			setTextIteratorLabel(countersVector[i], imagesListIterator[i], glWidgetsVector[i]->list.size());
			/*ustaw sciezke tla*/
			glWidgetsVector[i]->imgPath = glWidgetsVector[i]->list[imagesListIterator[i] - 1].absoluteFilePath().toUtf8().constData();
			glWidgetsVector[i]->drawBckg = true;

			prepareSlider(slidersVector[i], i);

			hMenuWidgetVector[i]->show();

			/*jesli wczytano tlo do pierwszego okna to ustal nowy rozmiar wektora modelState*/
			if (i == 0) {
				updateModelStateVectorSize(glWidgetsVector[i]->list.size());
			}
			QMessageBox::information(this, "Images loaded", "Images loaded successfully", QMessageBox::Ok);
		}
		else {
			hMenuWidgetVector[i]->hide();
		}
	}
}

void MainWindow::resizeDownGLWidgetPressed(int i) {
	float ratio = 0.9;
	int width = glWidgetsVector[i]->size().width() * ratio;
	int height = glWidgetsVector[i]->size().height() * ratio;

	if(width > 200 && height > 148)
		glWidgetsVector[i]->setFixedSize(width, height);
}

void MainWindow::resizeUpGLWidgetPressed(int i) {
	float ratio = 1.1;
	int width = glWidgetsVector[i]->size().width() * ratio;
	int height = glWidgetsVector[i]->size().height() * ratio;

	if (width < 1000 && height < 740)
		glWidgetsVector[i]->setFixedSize(width, height);
}

void MainWindow::deleteFramePressed(int i) {
	frameDeleted = false;
	/*nowa ilosc okien = stara ilosc okien - 1*/
	int howManyWidnowsLeft = glWidgetsVector.size() - 1;

	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, "Delete frame", "Do you want to delete selected frame? \nChanges are irreversible.",
		QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes) {
		if (howManyWidnowsLeft > 0) {
			/*wektory pomocnicze*/
			QVector<bool> sliderCheckedVector;

			QVector<int> modeVector;
			QVector<bool> drawJointsVector;
			QVector<bool> scaleFrameVector;
			QVector<int> frameWidthVector;
			QVector<int> frameHeightVector;
			QVector<bool> isAviVector;
			QVector<QString> loadedImagesFolderPathVector;
			QVector<QString> aviFilePathVector;
			QVector<QVector<cv::Mat>> aviFramesVector;
			QVector<QFileInfoList> listVector;
			QVector<string> imgPathVector;
			QVector<cv::Mat> aviFrameVector;
			QVector<bool> isInMainWindowVector;
			QVector<bool> drawBckgVector;
			QVector<bool> castVector;
			QVector<int> cameraIDVector;
			//QVector<vector<vector<float>>> modelStateVector;
			//QVector<vector<bool>> saveModelStateVector;
			//QVector<vector<map<string, pf::Vec3f>>> bonesRotationsVector;
			//QVector<vector<vector<float>>> modelTranslationVector;
			QVector<int> imagesListIteratorVector;

			/*zapisz parametry kazdego okna do wektorow*/
			for (int j = 0; j < glWidgetsVector.size(); j++) {
				if (j != i) {
					if (sliderCheckBoxesVector[j]->isChecked())
						sliderCheckedVector.push_back(true);
					else
						sliderCheckedVector.push_back(false);

					imagesListIteratorVector.push_back(imagesListIterator[j]);
					modeVector.push_back(glWidgetsVector[j]->mode);
					drawJointsVector.push_back(glWidgetsVector[j]->drawJoints);
					scaleFrameVector.push_back(glWidgetsVector[j]->scaleFrame);
					frameWidthVector.push_back(glWidgetsVector[j]->frameWidth);
					frameHeightVector.push_back(glWidgetsVector[j]->frameHeight);

					isAviVector.push_back(glWidgetsVector[j]->isAvi);
					if (glWidgetsVector[j]->isAvi) {
						QString tmp = "";
						loadedImagesFolderPathVector.push_back(tmp);
						aviFilePathVector.push_back(glWidgetsVector[j]->aviFilePath);
						aviFramesVector.push_back(glWidgetsVector[j]->aviFrames);
						QFileInfoList list;
						listVector.push_back(list);
						imgPathVector.push_back("");
						aviFrameVector.push_back(glWidgetsVector[j]->aviFrame);
					}
					else {
						QString tmp = "";

						if (!glWidgetsVector[j]->loadedImagesFolderPath.isEmpty())
							loadedImagesFolderPathVector.push_back(glWidgetsVector[j]->loadedImagesFolderPath);

						else
							loadedImagesFolderPathVector.push_back(tmp);

						aviFilePathVector.push_back(glWidgetsVector[j]->aviFilePath);
						QVector<cv::Mat> matVTmp;
						aviFramesVector.push_back(glWidgetsVector[j]->aviFrames);

						if (!glWidgetsVector[j]->list.isEmpty())
							listVector.push_back(glWidgetsVector[j]->list);
						else {
							QFileInfoList list;
							listVector.push_back(list);
						}

						if (!glWidgetsVector[j]->imgPath.empty())
							imgPathVector.push_back(glWidgetsVector[j]->imgPath);
						else
							imgPathVector.push_back("");

						cv::Mat matTmp;
						aviFrameVector.push_back(glWidgetsVector[j]->aviFrame);
					}
					isInMainWindowVector.push_back(glWidgetsVector[j]->isInMainWindow);
					drawBckgVector.push_back(glWidgetsVector[j]->drawBckg);
					castVector.push_back(glWidgetsVector[j]->cast);
					cameraIDVector.push_back(glWidgetsVector[j]->cameraID);
					//modelStateVector.push_back(glWidgetsVector[j]->modelState);
					//saveModelStateVector.push_back(glWidgetsVector[j]->saveModelState);
					//bonesRotationsVector.push_back(glWidgetsVector[j]->bonesRotations);
					//modelTranslationVector.push_back(glWidgetsVector[j]->modelTranslation);
				}
			}

			deleteGridChildLayout();
			
			/*dodaj nowa ilosc okien*/
			for (int j = 0; j < howManyWidnowsLeft; j++) {
				addFrameButton->click();
				/*jesli checkbox laczacy slider okna z glownym sliderem byl wybrany*/
				if (sliderCheckedVector[j])
					sliderCheckBoxesVector[j]->setChecked(true);
			}

			/*aktualizuj parametry okien z wektorow*/
			for (int j = 0; j < glWidgetsVector.size(); j++) {
				glWidgetsVector[j]->mode = modeVector[j];
				imagesListIterator[j] = imagesListIteratorVector[j];
				glWidgetsVector[j]->drawJoints = drawJointsVector[j];
				glWidgetsVector[j]->scaleFrame = scaleFrameVector[j];
				glWidgetsVector[j]->frameWidth = frameWidthVector[j];
				glWidgetsVector[j]->frameHeight = frameHeightVector[j];
				glWidgetsVector[j]->isAvi = isAviVector[j];

				if (glWidgetsVector[j]->isAvi) {
					glWidgetsVector[j]->aviFilePath = aviFilePathVector[j];
					glWidgetsVector[j]->aviFrames = aviFramesVector[j];
					glWidgetsVector[j]->aviFrame = aviFrameVector[j];

					if (!glWidgetsVector[j]->aviFrames.isEmpty()) {
						if (globalSlider->maximum() < glWidgetsVector[j]->aviFrames.size())
							globalSlider->setMaximum(glWidgetsVector[j]->aviFrames.size());
					}

					setTextIteratorLabel(countersVector[j], imagesListIterator[j], glWidgetsVector[j]->aviFrames.size());
					prepareSlider(slidersVector[j], j);

					slidersVector[j]->setValue(imagesListIterator[j]);
				}
				else {

					glWidgetsVector[j]->loadedImagesFolderPath = loadedImagesFolderPathVector[j];
					glWidgetsVector[j]->list = listVector[j];
					glWidgetsVector[j]->imgPath = imgPathVector[j];

					if (!glWidgetsVector[j]->list.isEmpty()) {
						if (globalSlider->maximum() < glWidgetsVector[j]->list.size())
							globalSlider->setMaximum(glWidgetsVector[j]->list.size());
					}

					setTextIteratorLabel(countersVector[j], imagesListIterator[j], glWidgetsVector[j]->list.size());

					glWidgetsVector[j]->drawBckg = true;

					prepareSlider(slidersVector[j], j);

					slidersVector[j]->setValue(imagesListIterator[j]);
				}
				glWidgetsVector[j]->isInMainWindow = isInMainWindowVector[j];
				glWidgetsVector[j]->drawBckg = drawBckgVector[j];
				if (glWidgetsVector[j]->drawBckg) {
					hMenuWidgetVector[j]->show();
				}

				glWidgetsVector[j]->cast = castVector[j];
				glWidgetsVector[j]->cameraID = cameraIDVector[j];

				if (glWidgetsVector[j]->cast) {
					glWidgetsVector[j]->setGLWidgetCamera(cameras[glWidgetsVector[j]->cameraID]);
					//odznacz wszystkie kamery
					for (int k = 0; k < selectCameraActionVector[j].size(); k++)
						selectCameraActionVector[j][k]->setChecked(false);

					//zaznacz wybrana kamere
					selectCameraActionVector[j][glWidgetsVector[j]->cameraID]->setChecked(true);
				}

			}
			/*aktualizuj model na obrazie*/
			for (int j = 0; j < glWidgetsVector.size(); j++) {
				if (bonesConf.size() != 0) {
					setGLWidgetDrawing(j);
				}
				
			}
			frameDeleted = true;

			refreshRotateSideMenu();
			/*aktywuj mozliwosc wyboru kamery*/
			if (cameras.size() > 0) {
				for (int i = 0; i < glWidgetsVector.size(); i++)
					selectCameraMenuVector[i]->setDisabled(false);
			}

			QMessageBox::information(this, "Frame deleted", "Frame deleted successfully", QMessageBox::Ok);
		}
		/*wylacz mozliwosc usuniecia klatki*/
		if (howManyWidnowsLeft == 1)
			deleteFrameMenuVector[0]->setDisabled(true);
		else {
			deleteFrameMenuVector[0]->setDisabled(false);
		}
	}
	
}

void MainWindow::mapIncreaseButtonSlot(int i){
    QObject::connect(increaseButtonMapperVector[i],SIGNAL(mapped(int)),this,SLOT(increaseIteratorPressed(int)));
	increaseButtonMapperVector[i]->setMapping(increaseButtonsVector[i], i);
    connect(increaseButtonsVector[i],SIGNAL(released()), increaseButtonMapperVector[i],SLOT(map()));
}

void MainWindow::mapReduceButtonSlot(int i){
    QObject::connect(reduceButtonMapperVector[i],SIGNAL(mapped(int)),this,SLOT(reduceIteratorPressed(int)));
	reduceButtonMapperVector[i]->setMapping(reduceButtonsVector[i], i);
    connect(reduceButtonsVector[i],SIGNAL(released()), reduceButtonMapperVector[i],SLOT(map()));
}

void MainWindow::mapMaxButtonSlot(int i){
    QObject::connect(maxButtonMapperVector[i],SIGNAL(mapped(int)),this,SLOT(minIteratorPressed(int)));
	maxButtonMapperVector[i]->setMapping(minButtonsVector[i], i);
    connect(minButtonsVector[i],SIGNAL(released()), maxButtonMapperVector[i],SLOT(map()));
}

void MainWindow::mapMinButtonSlot(int i){
    QObject::connect(minButtonMapperVector[i],SIGNAL(mapped(int)),this,SLOT(maxIteratorPressed(int)));
	minButtonMapperVector[i]->setMapping(maxButtonsVector[i], i);
    connect(maxButtonsVector[i],SIGNAL(released()), minButtonMapperVector[i],SLOT(map()));
}

void MainWindow::mapPlayPauseButtonSlot(int i) {
	QObject::connect(playPauseButtonMapperVector[i], SIGNAL(mapped(int)), this, SLOT(playPausePressed(int)));
	playPauseButtonMapperVector[i]->setMapping(playPauseButtonsVector[i], i);
	connect(playPauseButtonsVector[i], SIGNAL(released()), playPauseButtonMapperVector[i], SLOT(map()));
}

void MainWindow::mapTimerSlot(int i) {
	/*kazde z okien ma swoj timer*/
	connect(sliderMapperVector[i], SIGNAL(mapped(int)), this, SLOT(playBackground(int)));
	sliderMapperVector[i]->setMapping(backgroundTimersVector[i], i);
	connect(backgroundTimersVector[i], SIGNAL(timeout()), sliderMapperVector[i], SLOT(map()));
}

void MainWindow::mapManageBonesSlot(int i) {
}

void MainWindow::mapShowModelInDialog(int i) {
	/*QObject::connect(showModelInDialogMapperVector[i], SIGNAL(mapped(int)), this, SLOT(showModelInDialogPressed(int)));
	showModelInDialogMapperVector[i]->setMapping(showModelInDialogVector[i], i);
	connect(showModelInDialogVector[i], SIGNAL(triggered()), showModelInDialogMapperVector[i], SLOT(map()));*/
}

void MainWindow::mapSetConfigFromModel(int i) {
	/*QObject::connect(setConfigFromModelMapperVector[i], SIGNAL(mapped(int)), this, SLOT(setConfigFromModelPressed(int)));
	setConfigFromModelMapperVector[i]->setMapping(setConfigFromModelVector[i], i);
	connect(setConfigFromModelVector[i], SIGNAL(triggered()), setConfigFromModelMapperVector[i], SLOT(map()));*/
}

void MainWindow::mapSetConfigToAllModels(int i) {
	/*QObject::connect(setConfigToAllModelsMapperVector[i], SIGNAL(mapped(int)), this, SLOT(setConfigToAllModelsPressed(int)));
	setConfigToAllModelsMapperVector[i]->setMapping(setConfigToAllModelsVector[i], i);
	connect(setConfigToAllModelsVector[i], SIGNAL(triggered()), setConfigToAllModelsMapperVector[i], SLOT(map()));*/
}

void MainWindow::mapAddImagesToBackground(int i) {
	QObject::connect(addImagesBackgroundMapperVector[i], SIGNAL(mapped(int)), this, SLOT(selectImagesBackgroundPressed(int)));
	addImagesBackgroundMapperVector[i]->setMapping(selectImagesBackgroundVector[i], i);
	connect(selectImagesBackgroundVector[i], SIGNAL(triggered()), addImagesBackgroundMapperVector[i], SLOT(map()));
}

void MainWindow::mapResizeDownGLWidget(int i) {
	QObject::connect(resizeDownGLWidgetMapperVector[i], SIGNAL(mapped(int)), this, SLOT(resizeDownGLWidgetPressed(int)));
	resizeDownGLWidgetMapperVector[i]->setMapping(resizeDownActionVector[i], i);
	connect(resizeDownActionVector[i], SIGNAL(triggered()), resizeDownGLWidgetMapperVector[i], SLOT(map()));
}

void MainWindow::mapResizeUpGLWidget(int i) {
	QObject::connect(resizeUpGLWidgetMapperVector[i], SIGNAL(mapped(int)), this, SLOT(resizeUpGLWidgetPressed(int)));
	resizeUpGLWidgetMapperVector[i]->setMapping(resizeUpActionVector[i], i);
	connect(resizeUpActionVector[i], SIGNAL(triggered()), resizeUpGLWidgetMapperVector[i], SLOT(map()));
}

void MainWindow::mapDeleteFrame(int i) {
	if (deleteFrameMapperVector[i] != NULL && deleteFrameMenuVector[i] != NULL) {
		QObject::connect(deleteFrameMapperVector[i], SIGNAL(mapped(int)), this, SLOT(deleteFramePressed(int)));
		deleteFrameMapperVector[i]->setMapping(deleteFrameMenuVector[i], i);
		connect(deleteFrameMenuVector[i], SIGNAL(triggered()), deleteFrameMapperVector[i], SLOT(map()));
	}
}

void MainWindow::mapAddAviToBackground(int i) {
	QObject::connect(addAviMapperVector[i], SIGNAL(mapped(int)), this, SLOT(selectAviBackgroundPressed(int)));
	addAviMapperVector[i]->setMapping(selectAviBackgroundVector[i], i);
	connect(selectAviBackgroundVector[i], SIGNAL(triggered()), addAviMapperVector[i], SLOT(map()));
}

void MainWindow::updateFrameIterator(int i) {
	/*ustaw iterator okna na wartosc slidera*/
	imagesListIterator[i] = slidersVector[i]->value();
	if (!glWidgetsVector[i]->isAvi) {

		setTextIteratorLabel(countersVector[i], imagesListIterator[i], glWidgetsVector[i]->list.size());
		/*Sciezka do pliku tla*/
		QString imgString = glWidgetsVector[i]->list[imagesListIterator[i] - 1].absoluteFilePath();
		glWidgetsVector[i]->imgPath = imgString.toUtf8().constData();
	}
	else {
		setTextIteratorLabel(countersVector[i], imagesListIterator[i], glWidgetsVector[i]->aviFrames.size());
		/*ustaw nowy mat tla*/
		glWidgetsVector[i]->aviFrames[imagesListIterator[i] - 1].copyTo(glWidgetsVector[i]->aviFrame);
	}

	setGLWidgetDrawing(i);
}

void MainWindow::prepareFrameMenu(int i) {
	imagesListIterator[i] = 1;

	mapIncreaseButtonSlot(i);
	mapReduceButtonSlot(i);
	mapMaxButtonSlot(i);
	mapMinButtonSlot(i);
	mapPlayPauseButtonSlot(i);
	mapShowModelInDialog(i);
	mapAddAviToBackground(i);
	mapAddImagesToBackground(i);
	mapSetConfigFromModel(i);
	mapSetConfigToAllModels(i);
	mapDeleteFrame(i);
	mapTimerSlot(i);
	mapResizeDownGLWidget(i);
	mapResizeUpGLWidget(i);
}

void MainWindow::prepareSlider(QSlider *slider, int i) {
	slider->setOrientation(Qt::Horizontal);
	if(!glWidgetsVector[i]->isAvi)
		slider->setMaximum(glWidgetsVector[i]->list.size());
	else 
		slider->setMaximum(glWidgetsVector[i]->aviFrames.size());

	slider->setValue(1);
	slider->setMinimum(1);
	slider->setSingleStep(1);
	slider->setVisible(true);

	connect(slidersVector[i], SIGNAL(sliderReleased()), sliderMapperVector[i], SLOT(map()));
	sliderMapperVector[i]->setMapping(slidersVector[i], i);

	connect(sliderMapperVector[i], SIGNAL(mapped(int)), this, SLOT(updateFrameIterator(int)));
	//delete mapper;
}

void MainWindow::rotate(QString str) {
	QStringList values = str.split(";");
	if (values.count() == 5) {
		string boneName = values.at(0).toStdString();
		float direction = values.at(1).toFloat();
		string axis = values.at(2).toStdString();
		int qLineEditID = values.at(3).toInt();
		int qSpinBoxID = values.at(4).toInt();

		int frameNumber = comboBoxFrameSelector->currentIndex();
		int backgroundID = imagesListIterator[frameNumber] - 1;

		/*jesli wybrano okno o indeksie 0*/
		if (comboBoxFrameSelector->currentIndex() == 0) {

			/*obroc kosc okna nr 0*/
			if (axis == "X")
				glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate(boneName, direction, pf::Model3D::axisX, sideRotateMenuSpinBoxesVector[qSpinBoxID]->value(), bonesConf, limits, imagesListIterator[frameNumber] - 1, modelState);
			if (axis == "Y")
				glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate(boneName, direction, pf::Model3D::axisY, sideRotateMenuSpinBoxesVector[qSpinBoxID]->value(), bonesConf, limits, imagesListIterator[frameNumber] - 1, modelState);
			if (axis == "Z")
				glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate(boneName, direction, pf::Model3D::axisZ, sideRotateMenuSpinBoxesVector[qSpinBoxID]->value(), bonesConf, limits, imagesListIterator[frameNumber] - 1, modelState);

			mainModel->setModelState(modelState[imagesListIterator[frameNumber] - 1]);
			glWidgetsVector[0]->model->setModelState(modelState[imagesListIterator[frameNumber] - 1]);
			glWidgetsVector[0]->setRadiusVertices(mainModel->getRotatedVertices(), mainModel->getRadiusVec());
			glWidgetsVector[0]->update();

			/* jesli wybrano okno nr 0, a iteratory zdjec innych okien sa takie same jak okna 0 - rotacja kosci obejmuje wszystkie te okna*/
			for (int i = 1; i < glWidgetsVector.size(); i++) {
				if (imagesListIterator[0] - 1 == imagesListIterator[i] - 1) {

					glWidgetsVector[i]->model->setModelState(modelState[imagesListIterator[frameNumber] - 1]);
					glWidgetsVector[i]->setRadiusVertices(mainModel->getRotatedVertices(), mainModel->getRadiusVec());
					glWidgetsVector[i]->update();

				}
			}
		}
		/*dla pozostalych okien*/
		else {
			/*flaga sprawdzajaca czy indeks klatek wybranego okna (innego niz 0) jest taki sam jak przynajmniej jeden z pozostalych - gdy okno 2 i 3 wskazuja na ten sam indeks rotacja jest nie mozliwa*/
			bool flag = false;
			for (int i = 0; i < glWidgetsVector.size(); i++) {
				if (i != frameNumber) {
					if (imagesListIterator[frameNumber] - 1 == imagesListIterator[i] - 1)
						flag = true;
				}
			}
			if (!flag) {
				if (axis == "X")
					glWidgetsVector[frameNumber]->rotate(boneName, direction, pf::Model3D::axisX, sideRotateMenuSpinBoxesVector[qSpinBoxID]->value(), bonesConf, limits, imagesListIterator[frameNumber] - 1, modelState);
				if (axis == "Y")
					glWidgetsVector[frameNumber]->rotate(boneName, direction, pf::Model3D::axisY, sideRotateMenuSpinBoxesVector[qSpinBoxID]->value(), bonesConf, limits, imagesListIterator[frameNumber] - 1, modelState);
				if (axis == "Z")
					glWidgetsVector[frameNumber]->rotate(boneName, direction, pf::Model3D::axisZ, sideRotateMenuSpinBoxesVector[qSpinBoxID]->value(), bonesConf, limits, imagesListIterator[frameNumber] - 1, modelState);
				
				glWidgetsVector[frameNumber]->model->setModelState(modelState[imagesListIterator[frameNumber] - 1]);
				mainModel->setModelState(modelState[imagesListIterator[frameNumber] - 1]);
				glWidgetsVector[frameNumber]->setRadiusVertices(mainModel->getRotatedVertices(), mainModel->getRadiusVec());
				//mainModel->updateModelState(modelState[0]);
				glWidgetsVector[frameNumber]->update();
			}
			else {
				//QMessageBox::warning(this, "Warning", QString("Other window has selected same frame as the current one!"), QMessageBox::Ok);
			}
		}

		sideRotateMenuLineEditsVector[qLineEditID]->setText(QString::number(modelState[backgroundID][qLineEditID+3]));
	}

	else {
		//Okno błedu
		cout << "ERROR" << endl;
	}
}

void MainWindow::setRotate(QString str) {
	QStringList values = str.split(";");
	if (values.count() == 3) {
		string boneName = values.at(0).toStdString();
		string axis = values.at(1).toStdString();
		int qLineEditID = values.at(2).toInt();

		int frameNumber = comboBoxFrameSelector->currentIndex();
		int backgroundID = imagesListIterator[frameNumber] - 1;

		QString tmp = sideRotateMenuLineEditsVector[qLineEditID]->text();
		bool isDouble = false;

		float value = tmp.toDouble(&isDouble);
		if (isDouble && (value >= -360 && value <= 360)) {

			/*jesli wybrano okno o indeksie 0*/
			if (comboBoxFrameSelector->currentIndex() == 0) {

				/*obroc kosc okna nr 0*/
				if (axis == "X")
					glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate(boneName, pf::Model3D::axisX, value, bonesConf, limits, imagesListIterator[frameNumber] - 1, modelState);
				if (axis == "Y")
					glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate(boneName, pf::Model3D::axisY, value, bonesConf, limits, imagesListIterator[frameNumber] - 1, modelState);
				if (axis == "Z")
					glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate(boneName, pf::Model3D::axisZ, value, bonesConf, limits, imagesListIterator[frameNumber] - 1, modelState);

				mainModel->setModelState(modelState[imagesListIterator[frameNumber] - 1]);
				glWidgetsVector[0]->model->setModelState(modelState[imagesListIterator[frameNumber] - 1]);
				glWidgetsVector[0]->setRadiusVertices(mainModel->getRotatedVertices(), mainModel->getRadiusVec());
				glWidgetsVector[0]->update();

				/* jesli wybrano okno nr 0, a iteratory zdjec innych okien sa takie same jak okna 0 - rotacja kosci obejmuje wszystkie te okna*/
				for (int i = 1; i < glWidgetsVector.size(); i++) {
					if (imagesListIterator[0] - 1 == imagesListIterator[i] - 1) {

						glWidgetsVector[i]->model->setModelState(modelState[imagesListIterator[frameNumber] - 1]);
						glWidgetsVector[i]->setRadiusVertices(mainModel->getRotatedVertices(), mainModel->getRadiusVec());
						glWidgetsVector[i]->update();

					}
				}
			}
			/*dla pozostalych okien*/
			else {
				/*flaga sprawdzajaca czy indeks klatek wybranego okna (innego niz 0) jest taki sam jak przynajmniej jeden z pozostalych - gdy okno 2 i 3 wskazuja na ten sam indeks rotacja jest nie mozliwa*/
				bool flag = false;
				for (int i = 0; i < glWidgetsVector.size(); i++) {
					if (i != frameNumber) {
						if (imagesListIterator[frameNumber] - 1 == imagesListIterator[i] - 1)
							flag = true;
					}
				}
				if (!flag) {
					if (axis == "X")
						glWidgetsVector[frameNumber]->rotate(boneName, pf::Model3D::axisX, value, bonesConf, limits, imagesListIterator[frameNumber] - 1, modelState);
					if (axis == "Y")
						glWidgetsVector[frameNumber]->rotate(boneName, pf::Model3D::axisY, value, bonesConf, limits, imagesListIterator[frameNumber] - 1, modelState);
					if (axis == "Z")
						glWidgetsVector[frameNumber]->rotate(boneName, pf::Model3D::axisZ, value, bonesConf, limits, imagesListIterator[frameNumber] - 1, modelState);

					glWidgetsVector[frameNumber]->model->setModelState(modelState[imagesListIterator[frameNumber] - 1]);
					mainModel->setModelState(modelState[imagesListIterator[frameNumber] - 1]);
					glWidgetsVector[frameNumber]->setRadiusVertices(mainModel->getRotatedVertices(), mainModel->getRadiusVec());
					//mainModel->updateModelState(modelState[0]);
					glWidgetsVector[frameNumber]->update();
				}
				else {
				//	QMessageBox::warning(this, "Warning", QString("Other window has selected same frame as the current one!"), QMessageBox::Ok);
				}
			}
		}
	}


	else {
		//Okno błedu
		cout << "ERROR" << endl;
	}
}

void MainWindow::updateBoneLength(QString str) {
	QStringList values = str.split(";");
	if (values.count() == 4) {
		ModelHandler modelHandler;

		string boneName = values.at(0).toStdString();
		float direction = values.at(1).toFloat();
		int qLineEditID = values.at(2).toInt();
		int qSpinBoxID = values.at(3).toInt();

		try {
			modelHandler.updateLength(mainModel, bonesGeometry, boneName, direction, sideLengthMenuSpinBoxesVector[qSpinBoxID]->value());
			modelHandler.updateBonesLengthMap(bonesLength, boneName, modelHandler.getBoneGeometryLength(bonesGeometry, boneName));
			goldenRatioSpinBox->setValue(modelHandler.getModelHeightFromMap(bonesLength));
		}
		catch(MyException& e) {
			cerr << e.what() << endl;
			QMessageBox::warning(this, "Warning", "There is no " + QString::fromStdString(boneName) + " in bones geometry", QMessageBox::Ok);
			exit(-1);
		}

		for (int i = 0; i < comboBoxFrameSelector->count(); i++) {
			glWidgetsVector[i]->setRadiusVertices(mainModel->getRotatedVertices(), mainModel->getRadiusVec());
			glWidgetsVector[i]->update();
		}

		sideLengthMenuLineEditsVector[qLineEditID]->setText(QString::number(modelHandler.getBoneLength(bonesGeometry, boneName)));

		setGoldenRatioCheckBox->setChecked(false);
	}
	else {
		//Okno błedu
		cout << "ERROR" << endl;
	}
}

void MainWindow::setBoneLengthFromLineEdit(QString str) {
	QStringList values = str.split(";");
	if (values.count() == 2) {
		ModelHandler modelHandler;
		string boneName = values.at(0).toStdString();
		int qLineEditID = values.at(1).toInt();

		QString tmp = sideLengthMenuLineEditsVector[qLineEditID]->text();
		bool isDouble = false;

		float value = tmp.toDouble(&isDouble);
		if (isDouble && value >= 0) {
			try {
				modelHandler.updateLength(mainModel, bonesGeometry, boneName, value);
				modelHandler.updateBonesLengthMap(bonesLength, boneName, modelHandler.getBoneGeometryLength(bonesGeometry, boneName));
				goldenRatioSpinBox->setValue(modelHandler.getModelHeightFromMap(bonesLength));
			}
			catch (MyException& e) {
				cerr << e.what() << endl;
				QMessageBox::warning(this, "Error", "There is no " + QString::fromStdString(boneName) +  " in bones geometry", QMessageBox::Ok);
				exit(-1);
			}

			for (int i = 0; i < comboBoxFrameSelector->count(); i++) {
				glWidgetsVector[i]->setRadiusVertices(mainModel->getRotatedVertices(), mainModel->getRadiusVec());
				glWidgetsVector[i]->update();
			}

			setGoldenRatioCheckBox->setChecked(false);
		}
	}
	else {
		//Okno błedu
		cout << "ERROR" << endl;
	}
}

void MainWindow::updateBoneLengthGeometry(QString str) {
	QStringList values = str.split(";");
	if (values.count() == 4) {
		ModelHandler modelHandler;
		string boneName = values.at(0).toStdString();
		float direction = values.at(1).toFloat();
		int qLineEditID = values.at(2).toInt();
		int qSpinBoxID = values.at(3).toInt();
		try {
			modelHandler.updateLength(mainModel, bonesGeometry, boneName, direction, sideLengthMenuSpinBoxesVector[qSpinBoxID]->value());
			modelHandler.updateBonesLengthMap(bonesLength, boneName, modelHandler.getBoneGeometryLength(bonesGeometry, boneName));
			goldenRatioSpinBox->setValue(modelHandler.getModelHeightFromMap(bonesLength));
		}
		catch (MyException& e) {
			cerr << e.what() << endl;
			QMessageBox::warning(this, "Error", "There is no " + QString::fromStdString(boneName) + " in bones geometry", QMessageBox::Ok);
			exit(-1);
		}
		for (int i = 0; i < comboBoxFrameSelector->count(); i++) {
			glWidgetsVector[i]->setRadiusVertices(mainModel->getRotatedVertices(), mainModel->getRadiusVec());
			glWidgetsVector[i]->update();
		}

		sideGeometryMenuLineEditsVector[qLineEditID]->setText(QString::number(modelHandler.getBoneLength(bonesGeometry, boneName)));
		setGoldenRatioCheckBox->setChecked(false);
	}
	else {
		//Okno błedu
		cout << "ERROR" << endl;
	}
}

void MainWindow::setBoneLengthGeometryFromLineEdit(QString str) {
	QStringList values = str.split(";");
	if (values.count() == 2) {
		ModelHandler modelHandler;
		string boneName = values.at(0).toStdString();
		int qLineEditID = values.at(1).toInt();
		bool isDouble = false;
		QString tmp = sideGeometryMenuLineEditsVector[qLineEditID]->text();

		float value = tmp.toDouble(&isDouble);
		if (isDouble && value >= 0) {

			try {
				modelHandler.updateLength(mainModel, bonesGeometry, boneName, value);
				modelHandler.updateBonesLengthMap(bonesLength, boneName, modelHandler.getBoneGeometryLength(bonesGeometry, boneName));
				goldenRatioSpinBox->setValue(modelHandler.getModelHeightFromMap(bonesLength));
			}
			catch (MyException& e) {
				cerr << e.what() << endl;
				QMessageBox::warning(this, "Error", "There is no " + QString::fromStdString(boneName) + " in bones geometry", QMessageBox::Ok);
				exit(-1);
			}

			for (int i = 0; i < comboBoxFrameSelector->count(); i++) {
				//modelHandler->updateLength(glWidgetsVector[i]->model, glWidgetsVector[i]->bonesGeometry, boneName, direction, sideLengthMenuSpinBoxesVector[qSpinBoxID]->value());
				glWidgetsVector[i]->setRadiusVertices(mainModel->getRotatedVertices(), mainModel->getRadiusVec());
				glWidgetsVector[i]->update();
			}

			//sideLengthMenuLineEditsVector[qLineEditID]->setText(QString::number(modelHandler.getBoneLength(bonesGeometry, boneName)));
			//sideGeometryMenuLineEditsVector[qLineEditID * 3]->setText(QString::number(modelHandler.getBoneLength(bonesGeometry, boneName)));
			setGoldenRatioCheckBox->setChecked(false);
		}
	}
	else {
		//Okno błedu
		cout << "ERROR" << endl;
	}
}

void MainWindow::updateBoneTopRadius(QString str) {
	QStringList values = str.split(";");
	if (values.count() == 4) {
		ModelHandler modelHandler;

		string boneName = values.at(0).toStdString();
		float direction = values.at(1).toFloat();
		int qLineEditID = values.at(2).toInt();
		int qSpinBoxID = values.at(3).toInt();

		modelHandler.updateTopRadius(mainModel, bonesGeometry, boneName, direction, sideGeometryMenuSpinBoxesVector[qSpinBoxID]->value());

		for (int i = 0; i < comboBoxFrameSelector->count(); i++) {
			//modelHandler.updateTopRadius(glWidgetsVector[i]->model, glWidgetsVector[i]->bonesGeometry, boneName, direction, sideGeometryMenuSpinBoxesVector[qSpinBoxID]->value());
			glWidgetsVector[i]->setRadiusVertices(mainModel->getRotatedVertices(), mainModel->getRadiusVec());
			glWidgetsVector[i]->update();
		}

		sideGeometryMenuLineEditsVector[qLineEditID]->setText(QString::number(modelHandler.getBoneTopRadius(bonesGeometry, bonesGeometry[qLineEditID / 3].name)));
		update();
	}
	else {
		//Okno błedu
		cout << "ERRORtop" << endl;
	}
}

void MainWindow::setBoneTopRadiusFromLineEdit(QString str) {
	QStringList values = str.split(";");
	if (values.count() == 2) {
		ModelHandler modelHandler;
		string boneName = values.at(0).toStdString();
		int qLineEditID = values.at(1).toInt();
		bool isDouble = false;
		QString tmp = sideGeometryMenuLineEditsVector[qLineEditID]->text();
		float value = tmp.toDouble(&isDouble);
		if (isDouble && value >= 0) {

			try {
				modelHandler.updateTopRadius(mainModel, bonesGeometry, boneName, value);
			}
			catch (MyException& e) {
				cerr << e.what() << endl;
				QMessageBox::warning(this, "Warning", "There is no " + QString::fromStdString(boneName) + " in bones geometry", QMessageBox::Ok);
				exit(-1);
			}

			for (int i = 0; i < comboBoxFrameSelector->count(); i++) {
				//modelHandler.updateLength(glWidgetsVector[i]->model, glWidgetsVector[i]->bonesGeometry, boneName, direction, sideLengthMenuSpinBoxesVector[qSpinBoxID]->value());
				glWidgetsVector[i]->setRadiusVertices(mainModel->getRotatedVertices(), mainModel->getRadiusVec());
				glWidgetsVector[i]->update();
			}
		}
	}
	else {
		//Okno błedu
		cout << "ERRORsettop" << endl;
	}
}

void MainWindow::updateBoneBottomRadius(QString str) {
	QStringList values = str.split(";");
	if (values.count() == 4) {
		ModelHandler modelHandler;
		string boneName = values.at(0).toStdString();
		float direction = values.at(1).toFloat();
		int qLineEditID = values.at(2).toInt();
		int qSpinBoxID = values.at(3).toInt();

		try {
			modelHandler.updateBottomRadius(mainModel, bonesGeometry, boneName, direction, sideGeometryMenuSpinBoxesVector[qSpinBoxID]->value());
		}
		catch (MyException& e) {
			cerr << e.what() << endl;
			QMessageBox::warning(this, "Warning", "There is no " + QString::fromStdString(boneName) + " in bones geometry", QMessageBox::Ok);
			exit(-1);
		}
		for (int i = 0; i < comboBoxFrameSelector->count(); i++) {
			//modelHandler.updateBottomRadius(glWidgetsVector[i]->model, glWidgetsVector[i]->bonesGeometry, boneName, direction, sideGeometryMenuSpinBoxesVector[qSpinBoxID]->value());
			glWidgetsVector[i]->setRadiusVertices(mainModel->getRotatedVertices(), mainModel->getRadiusVec());
			glWidgetsVector[i]->update();
		}

		sideGeometryMenuLineEditsVector[qLineEditID]->setText(QString::number(modelHandler.getBoneBottomRadius(bonesGeometry, bonesGeometry[qLineEditID / 3].name)));
		update();
	}
	else {
		//Okno błedu
		cout << "ERRORbottom" << endl;
	}
}

void MainWindow::setBoneBottomRadiusFromLineEdit(QString str) {
	QStringList values = str.split(";");
	if (values.count() == 2) {
		ModelHandler modelHandler;
		string boneName = values.at(0).toStdString();
		int qLineEditID = values.at(1).toInt();
		bool isDouble = false;
		QString tmp = sideGeometryMenuLineEditsVector[qLineEditID]->text();
		float value = tmp.toDouble(&isDouble);
		if (isDouble && value >= 0) {
			try {
				modelHandler.updateBottomRadius(mainModel, bonesGeometry, boneName, value);
			}
			catch (MyException& e) {
				cerr << e.what() << endl;
				QMessageBox::warning(this, "Warning", "There is no " + QString::fromStdString(boneName) + " in bones geometry", QMessageBox::Ok);
				exit(-1);
			}
			for (int i = 0; i < comboBoxFrameSelector->count(); i++) {
				//modelHandler.updateLength(glWidgetsVector[i]->model, glWidgetsVector[i]->bonesGeometry, boneName, direction, sideLengthMenuSpinBoxesVector[qSpinBoxID]->value());
				glWidgetsVector[i]->setRadiusVertices(mainModel->getRotatedVertices(), mainModel->getRadiusVec());
				glWidgetsVector[i]->update();
			}
		}
	}
	else {
		//Okno błedu
		cout << "ERRORsetbottom" << endl;
	}
}

void MainWindow::translate(QString str) {
	QStringList values = str.split(";");
	if (values.count() == 4) {
		float direction = values.at(0).toFloat();
		string axis = values.at(1).toStdString();
		int qLineEditID = values.at(2).toInt();
		int qSpinBoxID = values.at(3).toInt();

		int frameNumber = comboBoxFrameSelector->currentIndex();
		int backgroundID = imagesListIterator[frameNumber] - 1;

		/*jesli wybrano okno o indeksie 0*/
		if (comboBoxFrameSelector->currentIndex() == 0) {

			/*przesun model okna nr 0*/
			if (axis == "X")
				glWidgetsVector[frameNumber]->translate(direction, pf::Model3D::axisX, sideLengthMenuSpinBoxesExtraVector[qSpinBoxID]->value(), backgroundID, modelTranslation, modelState);
			if (axis == "Y")
				glWidgetsVector[frameNumber]->translate(direction, pf::Model3D::axisY, sideLengthMenuSpinBoxesExtraVector[qSpinBoxID]->value(), backgroundID, modelTranslation, modelState);
			if (axis == "Z")
				glWidgetsVector[frameNumber]->translate(direction, pf::Model3D::axisZ, sideLengthMenuSpinBoxesExtraVector[qSpinBoxID]->value(), backgroundID, modelTranslation, modelState);

			mainModel->setModelState(modelState[imagesListIterator[frameNumber] - 1]);
			glWidgetsVector[0]->model->setModelState(modelState[imagesListIterator[frameNumber] - 1]);
			glWidgetsVector[0]->setRadiusVertices(mainModel->getRotatedVertices(), mainModel->getRadiusVec());
			glWidgetsVector[0]->update();

			/* jesli wybrano okno nr 0, a iteratory zdjec innych okien sa takie same jak okna 0 - translacja modelu obejmuje wszystkie te okna*/
			for (int i = 1; i < glWidgetsVector.size(); i++) {
				if (imagesListIterator[0] - 1 == imagesListIterator[i] - 1) {
					for (int i = 1; i < glWidgetsVector.size(); i++) {
						if (imagesListIterator[0] - 1 == imagesListIterator[i] - 1) {

							glWidgetsVector[i]->model->setModelState(modelState[imagesListIterator[frameNumber] - 1]);
							glWidgetsVector[i]->setRadiusVertices(mainModel->getRotatedVertices(), mainModel->getRadiusVec());
							glWidgetsVector[i]->update();
						}
					}
				}
			}
		}
		/*dla pozostalych okien*/
		else {
			/*flaga sprawdzajaca czy indeks klatek wybranego okna (innego niz 0) jest taki sam jak przynajmniej jeden z pozostalych - gdy okno 2 i 3 wskazuja na ten sam indeks rotacja jest nie mozliwa*/
			bool flag = false;
			for (int i = 0; i < glWidgetsVector.size(); i++) {
				if (i != frameNumber) {
					if (imagesListIterator[frameNumber] - 1 == imagesListIterator[i] - 1)
						flag = true;
				}
			}
			if (!flag) {
				if (axis == "X")
					glWidgetsVector[frameNumber]->translate(direction, pf::Model3D::axisX, sideLengthMenuSpinBoxesExtraVector[qSpinBoxID]->value(), backgroundID, modelTranslation, modelState);
				if (axis == "Y")
					glWidgetsVector[frameNumber]->translate(direction, pf::Model3D::axisY, sideLengthMenuSpinBoxesExtraVector[qSpinBoxID]->value(), backgroundID, modelTranslation, modelState);
				if (axis == "Z")
					glWidgetsVector[frameNumber]->translate(direction, pf::Model3D::axisZ, sideLengthMenuSpinBoxesExtraVector[qSpinBoxID]->value(), backgroundID, modelTranslation, modelState);
					
				glWidgetsVector[frameNumber]->model->setModelState(modelState[imagesListIterator[frameNumber] - 1]);
				mainModel->setModelState(modelState[imagesListIterator[frameNumber] - 1]);
				glWidgetsVector[frameNumber]->setRadiusVertices(mainModel->getRotatedVertices(), mainModel->getRadiusVec());
				glWidgetsVector[frameNumber]->update();
			}
			else {
				//QMessageBox::warning(this, "Warning", QString("Other window has selected same frame as the current one!"), QMessageBox::Ok);
			}
		}
		sideLengthMenuLineEditsExtraVector[qLineEditID]->setText(QString::number(modelState[backgroundID][qLineEditID]));
	}
	else {
		//Okno błedu
		cout << "ERROR" << endl;
	}
}

void MainWindow::setTranslate(QString str) {
	QStringList values = str.split(";");
	if (values.count() == 2) {
		string axis = values.at(0).toStdString();
		int qLineEditID = values.at(1).toInt();

		int frameNumber = comboBoxFrameSelector->currentIndex();
		int backgroundID = imagesListIterator[frameNumber] - 1;

		QString tmp = sideLengthMenuLineEditsExtraVector[qLineEditID]->text();
		bool isDouble = false;

		float value = tmp.toDouble(&isDouble);
		if (isDouble) {
			/*jesli wybrano okno o indeksie 0*/
			if (comboBoxFrameSelector->currentIndex() == 0) {

				/*przesun model okna nr 0*/
				if (axis == "X")
					glWidgetsVector[frameNumber]->translate(pf::Model3D::axisX, value, backgroundID, modelTranslation, modelState);
				if (axis == "Y")
					glWidgetsVector[frameNumber]->translate(pf::Model3D::axisY, value, backgroundID, modelTranslation, modelState);
				if (axis == "Z")
					glWidgetsVector[frameNumber]->translate(pf::Model3D::axisZ, value, backgroundID, modelTranslation, modelState);

				mainModel->setModelState(modelState[imagesListIterator[frameNumber] - 1]);
				glWidgetsVector[0]->model->setModelState(modelState[imagesListIterator[frameNumber] - 1]);
				glWidgetsVector[0]->setRadiusVertices(mainModel->getRotatedVertices(), mainModel->getRadiusVec());
				glWidgetsVector[0]->update();

				/* jesli wybrano okno nr 0, a iteratory zdjec innych okien sa takie same jak okna 0 - translacja modelu obejmuje wszystkie te okna*/
				for (int i = 1; i < glWidgetsVector.size(); i++) {
					if (imagesListIterator[0] - 1 == imagesListIterator[i] - 1) {
						for (int i = 1; i < glWidgetsVector.size(); i++) {
							if (imagesListIterator[0] - 1 == imagesListIterator[i] - 1) {

								glWidgetsVector[i]->model->setModelState(modelState[imagesListIterator[frameNumber] - 1]);
								glWidgetsVector[i]->setRadiusVertices(mainModel->getRotatedVertices(), mainModel->getRadiusVec());
								glWidgetsVector[i]->update();
							}
						}
					}
				}
			}
			/*dla pozostalych okien*/
			else {
				/*flaga sprawdzajaca czy indeks klatek wybranego okna (innego niz 0) jest taki sam jak przynajmniej jeden z pozostalych - gdy okno 2 i 3 wskazuja na ten sam indeks rotacja jest nie mozliwa*/
				bool flag = false;
				for (int i = 0; i < glWidgetsVector.size(); i++) {
					if (i != frameNumber) {
						if (imagesListIterator[frameNumber] - 1 == imagesListIterator[i] - 1)
							flag = true;
					}
				}
				if (!flag) {
					if (axis == "X")
						glWidgetsVector[frameNumber]->translate(pf::Model3D::axisX,value, backgroundID, modelTranslation, modelState);
					if (axis == "Y")
						glWidgetsVector[frameNumber]->translate(pf::Model3D::axisY, value, backgroundID, modelTranslation, modelState);
					if (axis == "Z")
						glWidgetsVector[frameNumber]->translate(pf::Model3D::axisZ, value, backgroundID, modelTranslation, modelState);

					glWidgetsVector[frameNumber]->model->setModelState(modelState[imagesListIterator[frameNumber] - 1]);
					mainModel->setModelState(modelState[imagesListIterator[frameNumber] - 1]);
					glWidgetsVector[frameNumber]->setRadiusVertices(mainModel->getRotatedVertices(), mainModel->getRadiusVec());
					glWidgetsVector[frameNumber]->update();
				}
				else {
					//QMessageBox::warning(this, "Warning", QString("Other window has selected same frame as the current one!"), QMessageBox::Ok);
				}
			}
		}
	}
	else {
		//Okno błedu
		cout << "ERROR" << endl;
	}
}

void MainWindow::reloadGLWidgetMenu() {
	/*usun elementy bocznego menu*/
	qDeleteAll(sideRotateMenuLineEditsVector);
	qDeleteAll(sideRotateMenuLabelsVector);
	qDeleteAll(sideRotateMenuButtonsVector);
	qDeleteAll(sideRotateMenuHLayoutsVector);
	qDeleteAll(sideRotateMenuSpinBoxesVector);

	qDeleteAll(sideLengthMenuLabelsVector);
	qDeleteAll(sideLengthMenuHLayoutsVector);
	qDeleteAll(sideLengthMenuButtonsVector);
	qDeleteAll(sideLengthMenuLineEditsVector);
	qDeleteAll(sideLengthMenuSpinBoxesVector);
	qDeleteAll(sideLengthMenuRotLabelsVector);
	qDeleteAll(sideLengthMenuRotCheckBoxesVector);
	qDeleteAll(sideLengthMenuLimVelLabelsVector);
	qDeleteAll(sideLengthMenuLimVelSpinBoxesVector);

	qDeleteAll(sideLengthMenuHLayoutsExtraVector);
	qDeleteAll(sideLengthMenuLabelsExtraVector);
	qDeleteAll(sideLengthMenuButtonsExtraVector);
	qDeleteAll(sideLengthMenuLineEditsExtraVector);
	qDeleteAll(sideLengthMenuSpinBoxesExtraVector);

	qDeleteAll(sideGeometryMenuLabelsVector);
	qDeleteAll(sideGeometryMenuHLayoutsVector);
	qDeleteAll(sideGeometryMenuButtonsVector);
	qDeleteAll(sideGeometryMenuLineEditsVector);
	qDeleteAll(sideGeometryMenuSpinBoxesVector);

	qDeleteAll(rotateMapperVector);
	qDeleteAll(rotateFromLineEditMapperVector);
	qDeleteAll(translateFromLineEditMapperVector);
	qDeleteAll(lengthMapperVector);
	qDeleteAll(lengthFromLineEditMapperVector);
	qDeleteAll(geometryMapperVector);
	qDeleteAll(geometryLengthFromLineEditMapperVector);
	qDeleteAll(geometryTopRadiusFromLineEditMapperVector);
	qDeleteAll(geometryBottomRadiusFromLineEditMapperVector);


	delete saveCurrentModelStateToVector;
	delete goldenRatioGroupBox;
	/*delete setGoldenRatioButton;
	delete setGoldenRatioCheckBox;
	delete goldenRatio1RowLayout;
	delete goldenRatio2RowLayout;
	delete goldenRatioLayout;
	delete goldenRatioHeightLabel;
	delete goldenRatioSpinBox;*/

	sideRotateMenuLineEditsVector.clear();
	sideRotateMenuLabelsVector.clear();
	sideRotateMenuButtonsVector.clear();
	sideRotateMenuHLayoutsVector.clear();
	sideRotateMenuSpinBoxesVector.clear();

	sideLengthMenuLabelsVector.clear();
	sideLengthMenuHLayoutsVector.clear();
	sideLengthMenuButtonsVector.clear();
	sideLengthMenuLineEditsVector.clear();
	sideLengthMenuSpinBoxesVector.clear();
	sideLengthMenuRotLabelsVector.clear();
	sideLengthMenuRotCheckBoxesVector.clear();
	sideLengthMenuLimVelLabelsVector.clear();
	sideLengthMenuLimVelSpinBoxesVector.clear();

	sideLengthMenuHLayoutsExtraVector.clear();
	sideLengthMenuLabelsExtraVector.clear();
	sideLengthMenuButtonsExtraVector.clear();
	sideLengthMenuLineEditsExtraVector.clear();
	sideLengthMenuSpinBoxesExtraVector.clear();

	sideGeometryMenuLabelsVector.clear();
	sideGeometryMenuHLayoutsVector.clear();
	sideGeometryMenuButtonsVector.clear();
	sideGeometryMenuLineEditsVector.clear();
	sideGeometryMenuSpinBoxesVector.clear();

	rotateMapperVector.clear();
	rotateFromLineEditMapperVector.clear();
	translateFromLineEditMapperVector.clear();
	lengthMapperVector.clear();
	lengthFromLineEditMapperVector.clear();
	geometryMapperVector.clear();
	geometryLengthFromLineEditMapperVector.clear();
	geometryTopRadiusFromLineEditMapperVector.clear();
	geometryBottomRadiusFromLineEditMapperVector.clear();

	/*dodaj zakladki*/
	addGLWidgetRotateMenu();
	addGLWidgetLengthMenu();
	addGLWidgetGeometryMenu();
	
}

void MainWindow::reloadGLRotateMenu() {
	/*usun elementy rotate menu*/
	qDeleteAll(sideRotateMenuLineEditsVector);
	qDeleteAll(sideRotateMenuLabelsVector);
	qDeleteAll(sideRotateMenuButtonsVector);
	qDeleteAll(sideRotateMenuHLayoutsVector);
	qDeleteAll(sideRotateMenuSpinBoxesVector);

	qDeleteAll(rotateMapperVector);
	qDeleteAll(rotateFromLineEditMapperVector);

	qDeleteAll(sideLengthMenuHLayoutsExtraVector);
	qDeleteAll(sideLengthMenuLabelsExtraVector);
	qDeleteAll(sideLengthMenuButtonsExtraVector);
	qDeleteAll(sideLengthMenuLineEditsExtraVector);
	qDeleteAll(sideLengthMenuSpinBoxesExtraVector);

	delete saveCurrentModelStateToVector;

	sideRotateMenuLineEditsVector.clear();
	sideRotateMenuLabelsVector.clear();
	sideRotateMenuButtonsVector.clear();
	sideRotateMenuHLayoutsVector.clear();
	sideRotateMenuSpinBoxesVector.clear();

	rotateMapperVector.clear();
	rotateFromLineEditMapperVector.clear();

	sideLengthMenuHLayoutsExtraVector.clear();
	sideLengthMenuLabelsExtraVector.clear();
	sideLengthMenuButtonsExtraVector.clear();
	sideLengthMenuLineEditsExtraVector.clear();
	sideLengthMenuSpinBoxesExtraVector.clear();

	/*dodaj zakladke na nowo*/
	addGLWidgetRotateMenu();
}

void MainWindow::refreshRotateSideMenu() {

	ModelHandler modelHandler;
	//numer wybranego okna
	int frameNumber = comboBoxFrameSelector->currentIndex();
	int backgroundID =  imagesListIterator[frameNumber] - 1;

	/*jesli iterator okna przekracza max okna glownego wylacz mozliwosc rotacji*/
	if (checkIteratorRange(frameNumber)) {
		scrollRotateSideArea->setDisabled(false);
	}
	else {
		setGLWidgetDrawningToFirstModelConfigurtion(frameNumber);
		backgroundID = 0;
		scrollRotateSideArea->setDisabled(true);
	}

	/*wartosci przesuniecia modelu w osiach x,y,z*/
	sideLengthMenuLineEditsExtraVector[0]->setText(QString::number(modelState[backgroundID][0]));
	sideLengthMenuLineEditsExtraVector[1]->setText(QString::number(modelState[backgroundID][1]));
	sideLengthMenuLineEditsExtraVector[2]->setText(QString::number(modelState[backgroundID][2]));

	/*wartosci rotacji kolejnych kosci*/
	int rootID = modelHandler.getModelStateID(mainModel->getNamesMovingBones(), "root", pf::Model3D::axisX);
	for (int i=0; i<sideRotateMenuLineEditsVector.size(); i++)
		sideRotateMenuLineEditsVector[i]->setText(QString::number(modelState[backgroundID][rootID+i]));

	/*jesli modelState o aktualnym id zostal wybrany to zapisz*/
	for (int i = 0; i < glWidgetsVector.size(); i++) {
		if (saveModelState[imagesListIterator[i] - 1] == true)
			frameIDVector[i]->setStyleSheet("color: LawnGreen");
		else
			frameIDVector[i]->setStyleSheet("color: red");
	}
}

void MainWindow::refreshLengthSideMenu() {
	/*odswiez dlugosc kosci*/
	ModelHandler modelHandler;
	for (int i = 0; i < sideLengthMenuLineEditsVector.size(); i++)
		sideLengthMenuLineEditsVector[i]->setText(QString::number(modelHandler.getBoneLength(bonesGeometry, bonesGeometry[i].name)));
}

void MainWindow::refreshGeometrySideMenu() {
	ModelHandler modelHandler;
	//numer wybranego okna
	int frameNumber = comboBoxFrameSelector->currentIndex();

	/*odswiez dlugosc, top i bottom radius kosci*/
	for (int i = 0; i < sideLengthMenuLineEditsVector.size(); i++) {
		sideGeometryMenuLineEditsVector[i*3]->setText(QString::number(modelHandler.getBoneLength(bonesGeometry, bonesGeometry[i].name)));
		sideGeometryMenuLineEditsVector[i*3+1]->setText(QString::number(modelHandler.getBoneTopRadius(bonesGeometry, bonesGeometry[i].name)));
		sideGeometryMenuLineEditsVector[i*3+2]->setText(QString::number(modelHandler.getBoneBottomRadius(bonesGeometry, bonesGeometry[i].name)));
	}
}

void MainWindow::addGLWidgetRotateMenu() {
	ModelHandler modelHandler;
	int rotateMapperCounter = 0;
	vboxRotateLayout->setAlignment(Qt::AlignTop);

	/*ilosc kosci, ktore mozna obracac*/
	int usedBonesSize = bonesConf.size();
	vector<pf::boneConfig> usedBones = bonesConf;

	saveCurrentModelStateToVector = new QPushButton(this);
	saveCurrentModelStateToVector->setText("Save Model State");
	connect(saveCurrentModelStateToVector, SIGNAL(released()), this, SLOT(saveModelStateToBoolVector()));

	/*przesuniecie modelu*/
	for (int i = 0; i < 4; i++) {
		sideLengthMenuHLayoutsExtraVector.push_back(new QHBoxLayout());
		sideLengthMenuHLayoutsExtraVector[i]->setAlignment(Qt::AlignLeft);
		sideLengthMenuLabelsExtraVector.push_back(new QLabel());
	}

	for (int i = 1; i < 4; i++) {
		sideLengthMenuLabelsExtraVector[i]->setFixedWidth(30);
	}

	for (int i = 0; i < 3; i++) {
		sideLengthMenuButtonsExtraVector.push_back(new QPushButton());
		sideLengthMenuButtonsExtraVector.push_back(new QPushButton());
		sideLengthMenuLineEditsExtraVector.push_back(new QLineEdit());
		sideLengthMenuLineEditsExtraVector[i]->setFixedWidth(50);
		sideLengthMenuSpinBoxesExtraVector.push_back(new QSpinBox());
		sideLengthMenuSpinBoxesExtraVector[i]->setFixedWidth(50);
	}

	for (int i = 0; i<sideLengthMenuButtonsExtraVector.size(); i++)
		sideLengthMenuButtonsExtraVector[i]->setFixedWidth(30);

	sideLengthMenuLabelsExtraVector[0]->setText("Model translation");
	sideLengthMenuHLayoutsExtraVector[0]->addWidget(sideLengthMenuLabelsExtraVector[0]);

	sideLengthMenuLabelsExtraVector[1]->setText("OX");
	sideLengthMenuHLayoutsExtraVector[1]->addWidget(sideLengthMenuLabelsExtraVector[1]);
	sideLengthMenuHLayoutsExtraVector[1]->addWidget(sideLengthMenuButtonsExtraVector[0]);
	sideLengthMenuHLayoutsExtraVector[1]->addWidget(sideLengthMenuLineEditsExtraVector[0]);
	sideLengthMenuHLayoutsExtraVector[1]->addWidget(sideLengthMenuButtonsExtraVector[1]);
	sideLengthMenuHLayoutsExtraVector[1]->addWidget(sideLengthMenuSpinBoxesExtraVector[0]);

	sideLengthMenuLabelsExtraVector[2]->setText("OY");
	sideLengthMenuHLayoutsExtraVector[2]->addWidget(sideLengthMenuLabelsExtraVector[2]);
	sideLengthMenuHLayoutsExtraVector[2]->addWidget(sideLengthMenuButtonsExtraVector[2]);
	sideLengthMenuHLayoutsExtraVector[2]->addWidget(sideLengthMenuLineEditsExtraVector[1]);
	sideLengthMenuHLayoutsExtraVector[2]->addWidget(sideLengthMenuButtonsExtraVector[3]);
	sideLengthMenuHLayoutsExtraVector[2]->addWidget(sideLengthMenuSpinBoxesExtraVector[1]);

	sideLengthMenuLabelsExtraVector[3]->setText("OZ");
	sideLengthMenuHLayoutsExtraVector[3]->addWidget(sideLengthMenuLabelsExtraVector[3]);
	sideLengthMenuHLayoutsExtraVector[3]->addWidget(sideLengthMenuButtonsExtraVector[4]);
	sideLengthMenuHLayoutsExtraVector[3]->addWidget(sideLengthMenuLineEditsExtraVector[2]);
	sideLengthMenuHLayoutsExtraVector[3]->addWidget(sideLengthMenuButtonsExtraVector[5]);
	sideLengthMenuHLayoutsExtraVector[3]->addWidget(sideLengthMenuSpinBoxesExtraVector[2]);

	for (int j = 0; j < 6; j++) {
		QString str;
		switch (j) {
		case 0:
			str = QString::fromStdString("-1.0;X;0;0");
			break;

		case 1:
			str = QString::fromStdString("1.0;X;0;0");
			break;

		case 2:
			str = QString::fromStdString("-1.0;Y;1;1");
			break;

		case 3:
			str = QString::fromStdString("1.0;Y;1;1");
			break;

		case 4:
			str = QString::fromStdString("-1.0;Z;2;2");
			break;

		case 5:
			str = QString::fromStdString("1.0;Z;2;2");
			break;

		default:
			str = " ";
			cout << "error" << endl;
			break;
		}

		QSignalMapper *mapper = new QSignalMapper(this);
		QObject::connect(mapper, SIGNAL(mapped(QString)), this, SLOT(translate(QString)));
		mapper->setMapping(sideLengthMenuButtonsExtraVector[j], str);
		connect(sideLengthMenuButtonsExtraVector[j], SIGNAL(released()), mapper, SLOT(map()));
		//QString().swap(str);
	}

	vboxRotateLayout->addWidget(saveCurrentModelStateToVector);

	for (int hLayouts = 0; hLayouts < sideLengthMenuHLayoutsExtraVector.size(); hLayouts++)
		vboxRotateLayout->addLayout(sideLengthMenuHLayoutsExtraVector[hLayouts]);

	for (int i = 0; i < sideLengthMenuButtonsExtraVector.size(); i = i + 2) {
		addSideMenuButtonsImages(sideLengthMenuButtonsExtraVector[i], sideLengthMenuButtonsExtraVector[i + 1]);
	}

	/*rotacja modelu*/
	for (int i = 0; i < usedBonesSize * 4; i++) {
		sideRotateMenuHLayoutsVector.push_back(new QHBoxLayout());
		sideRotateMenuHLayoutsVector[i]->setAlignment(Qt::AlignLeft);
	}
	
	for (int i = 0; i < usedBonesSize * 4; i++) {
		sideRotateMenuLabelsVector.push_back(new QLabel());
		if (i % 4 != 0) {
			sideRotateMenuLabelsVector[i]->setFixedWidth(25);
		}
	}

	for (int i = 0; i < usedBonesSize * 6; i++) {
		sideRotateMenuButtonsVector.push_back(new QPushButton());
		sideRotateMenuButtonsVector[i]->setFixedWidth(30);
	}

	for (int i = 0; i < usedBonesSize * 3; i++) {
		sideRotateMenuLineEditsVector.push_back(new QLineEdit());
		sideRotateMenuLineEditsVector[i]->setFixedWidth(50);
	}

	for (int i = 0; i < usedBonesSize * 3; i++) {
		sideRotateMenuSpinBoxesVector.push_back(new QSpinBox());
		sideRotateMenuSpinBoxesVector[i]->setFixedWidth(50);
	}

	int hLayoutCounter = 0, labelsCounter = 0, buttonsCounter = 0, lineEditsCounter = 0, spinBoxesCounter = 0;
	for (int i = 1; i <= usedBonesSize; i++) {
		sideRotateMenuLabelsVector[labelsCounter]->setText(QString::fromStdString(usedBones[i - 1].name) + " rotation");
		sideRotateMenuLabelsVector[labelsCounter]->setStyleSheet("font-weight: bold");
		sideRotateMenuHLayoutsVector[hLayoutCounter]->addWidget(sideRotateMenuLabelsVector[labelsCounter]);
		labelsCounter++;
		hLayoutCounter++;

		sideRotateMenuLabelsVector[labelsCounter]->setText("OX");
		sideRotateMenuHLayoutsVector[hLayoutCounter]->addWidget(sideRotateMenuLabelsVector[labelsCounter]);
		sideRotateMenuHLayoutsVector[hLayoutCounter]->addWidget(sideRotateMenuButtonsVector[buttonsCounter]);
		sideRotateMenuHLayoutsVector[hLayoutCounter]->addWidget(sideRotateMenuLineEditsVector[lineEditsCounter]);
		sideRotateMenuHLayoutsVector[hLayoutCounter]->addWidget(sideRotateMenuButtonsVector[buttonsCounter+1]);
		sideRotateMenuHLayoutsVector[hLayoutCounter]->addWidget(sideRotateMenuSpinBoxesVector[spinBoxesCounter]);

		lineEditsCounter++;
		buttonsCounter++;
		buttonsCounter++;
		labelsCounter++;
		hLayoutCounter++;
		spinBoxesCounter++;

		sideRotateMenuLabelsVector[labelsCounter]->setText("OY");
		sideRotateMenuHLayoutsVector[hLayoutCounter]->addWidget(sideRotateMenuLabelsVector[labelsCounter]);
		sideRotateMenuHLayoutsVector[hLayoutCounter]->addWidget(sideRotateMenuButtonsVector[buttonsCounter]);
		sideRotateMenuHLayoutsVector[hLayoutCounter]->addWidget(sideRotateMenuLineEditsVector[lineEditsCounter]);
		sideRotateMenuHLayoutsVector[hLayoutCounter]->addWidget(sideRotateMenuButtonsVector[buttonsCounter + 1]);
		sideRotateMenuHLayoutsVector[hLayoutCounter]->addWidget(sideRotateMenuSpinBoxesVector[spinBoxesCounter]);

		lineEditsCounter++;
		buttonsCounter++;
		buttonsCounter++;
		labelsCounter++;
		hLayoutCounter++;
		spinBoxesCounter++;

		sideRotateMenuLabelsVector[labelsCounter]->setText("OZ");
		sideRotateMenuHLayoutsVector[hLayoutCounter]->addWidget(sideRotateMenuLabelsVector[labelsCounter]);
		sideRotateMenuHLayoutsVector[hLayoutCounter]->addWidget(sideRotateMenuButtonsVector[buttonsCounter]);
		sideRotateMenuHLayoutsVector[hLayoutCounter]->addWidget(sideRotateMenuLineEditsVector[lineEditsCounter]);
		sideRotateMenuHLayoutsVector[hLayoutCounter]->addWidget(sideRotateMenuButtonsVector[buttonsCounter + 1]);
		sideRotateMenuHLayoutsVector[hLayoutCounter]->addWidget(sideRotateMenuSpinBoxesVector[spinBoxesCounter]);

		lineEditsCounter++;
		buttonsCounter++;
		buttonsCounter++;
		labelsCounter++;
		hLayoutCounter++;
		spinBoxesCounter++;
	}
	
	//odswiezenie wektora limits
	int frameNumber = comboBoxFrameSelector->currentIndex();

	if (!justLaunched) {
		modelHandler.setLimitsVector(limits, bonesConf, bonesGeometry);
		modelHandler.setVelocityVector(velocity, bonesConf, bonesGeometry);
	}

	/*parametry qspinbox*/
	for (int i = 0; i < sideRotateMenuLineEditsVector.size(); i++) {
		setQSpinBoxSettings(sideRotateMenuSpinBoxesVector[i], 0, 20, 5);
	}

	/*parametry qspinbox - translacja*/
	for (int i = 0; i < sideLengthMenuSpinBoxesExtraVector.size(); i++) {
		setQSpinBoxSettings(sideLengthMenuSpinBoxesExtraVector[i], 0, 50, 25);
	}

	/*zdjecia buttonow*/
	for (int i = 0; i < sideRotateMenuButtonsVector.size(); i=i+2) {
		addSideMenuButtonsImages(sideRotateMenuButtonsVector[i], sideRotateMenuButtonsVector[i + 1]);
	}

	for (int i = 0; i < usedBonesSize; i++) {
		//kazda z kosci moze byc obracana w 3 osiach w 2 strony
		for (int j = 0; j < 6; j++) {
			QString str;
			switch (j) {
			case 0:
				str = QString::fromStdString(usedBones[i].name + ";-1.0;X");
				break;

			case 1:
				str = QString::fromStdString(usedBones[i].name + ";1.0;X");
				break;

			case 2:
				str = QString::fromStdString(usedBones[i].name + ";-1.0;Y");
				break;

			case 3:
				str = QString::fromStdString(usedBones[i].name + ";1.0;Y");
				break;

			case 4:
				str = QString::fromStdString(usedBones[i].name + ";-1.0;Z");
				break;

			case 5:
				str = QString::fromStdString(usedBones[i].name + ";1.0;Z");
				break;

			default:
				str = " ";
				cout << "BLAD" << endl;
				break;
			}

			int tmpVal;
			if (j < 2) tmpVal = 0;
			else if (j>=2 && j<4) tmpVal = 1;
			else tmpVal = 2;
			/*id qLineEdit*/
			str += QString::fromStdString(";"+std::to_string(i*3+tmpVal));

			/*id qSpinBox*/
			str += QString::fromStdString(";" + std::to_string(i * 3 + tmpVal));

			rotateMapperVector.push_back(new QSignalMapper(this));
			QObject::connect(rotateMapperVector[rotateMapperCounter], SIGNAL(mapped(QString)), this, SLOT(rotate(QString)));
			rotateMapperVector[rotateMapperCounter]->setMapping(sideRotateMenuButtonsVector[(i * 6) + j], str);
			connect(sideRotateMenuButtonsVector[(i * 6) + j], SIGNAL(released()), rotateMapperVector[rotateMapperCounter], SLOT(map()));
			rotateMapperCounter++;
		}
	}

	for (int hLayouts = 0; hLayouts < sideRotateMenuHLayoutsVector.size(); hLayouts++)
		vboxRotateLayout->addLayout(sideRotateMenuHLayoutsVector[hLayouts]);

	disableRotatingButtons();

	refreshRotateSideMenu();
	justLaunched = false;

	for (int i = 0; i < usedBonesSize; i++) {
		for (int j = 0; j < 3; j++) {
			rotateFromLineEditMapperVector.push_back(new QSignalMapper(this));
			int id = i * 3 + j;
			QString str = QString::fromStdString(usedBones[i].name);// +";" + QString::number(id);

			if (j == 0)
				str = str + ";X;" + QString::number(id);
			else if(j == 1)
				str = str + ";Y;" + QString::number(id);
			else
				str = str + ";Z;" + QString::number(id);

			QObject::connect(rotateFromLineEditMapperVector[id], SIGNAL(mapped(QString)), this, SLOT(setRotate(QString)));
			rotateFromLineEditMapperVector[id]->setMapping(sideRotateMenuLineEditsVector[id], str);
			connect(sideRotateMenuLineEditsVector[id], SIGNAL(textChanged(const QString &)), rotateFromLineEditMapperVector[id], SLOT(map()));
		}
	}


	for (int j = 0; j < 3; j++) {
		translateFromLineEditMapperVector.push_back(new QSignalMapper(this));
		QString str;
		if (j == 0)
			str = str + "X;" + QString::number(j);
		else if (j == 1)
			str = str + "Y;" + QString::number(j);
		else
			str = str + "Z;" + QString::number(j);

		QObject::connect(translateFromLineEditMapperVector[j], SIGNAL(mapped(QString)), this, SLOT(setTranslate(QString)));
		translateFromLineEditMapperVector[j]->setMapping(sideLengthMenuLineEditsExtraVector[j], str);
		connect(sideLengthMenuLineEditsExtraVector[j], SIGNAL(textChanged(const QString &)), translateFromLineEditMapperVector[j], SLOT(map()));
	}
}

void MainWindow::addGLWidgetLengthMenu() {
	ModelHandler modelHandler;
	int lengthMapperCounter = 0;
	vboxLengthLayout->setAlignment(Qt::AlignTop);

	/*golden ratio*/
	goldenRatioGroupBox = new QGroupBox("Golden ratio", this);
	goldenRatioLayout = new QVBoxLayout();
	goldenRatio1RowLayout = new QHBoxLayout();
	goldenRatio2RowLayout = new QHBoxLayout();
	goldenRatioHeightLabel = new QLabel("Height", this);
	setGoldenRatioButton = new QPushButton("Set golden ratio", this);
	setGoldenRatioButton->setToolTip(QString::number(GOLDEN_RATIO, 'g', 11));
	setGoldenRatioCheckBox = new QCheckBox(this);
	goldenRatioSpinBox = new QDoubleSpinBox(this);
	goldenRatioSpinBox->setMinimum(0.0);
	goldenRatioSpinBox->setMaximum(10000.0);
	goldenRatioSpinBox->setValue(modelHandler.getModelHeightFromMap(bonesLength));

	//readonly checkbox
	setGoldenRatioCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents);
	setGoldenRatioCheckBox->setFocusPolicy(Qt::NoFocus);

	goldenRatio1RowLayout->addWidget(setGoldenRatioButton);
	goldenRatio1RowLayout->addWidget(setGoldenRatioCheckBox);
	goldenRatio2RowLayout->addWidget(goldenRatioHeightLabel);
	goldenRatio2RowLayout->addWidget(goldenRatioSpinBox);

	goldenRatio1RowLayout->setAlignment(Qt::AlignRight);
	goldenRatio2RowLayout->setAlignment(Qt::AlignRight);

	goldenRatioLayout->addLayout(goldenRatio1RowLayout);
	goldenRatioLayout->addLayout(goldenRatio2RowLayout);
	goldenRatioGroupBox->setLayout(goldenRatioLayout);

	connect(setGoldenRatioButton, SIGNAL(released()), this, SLOT(setGoldenRatio()));
	connect(goldenRatioSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setGoldenRatioFromSpinBox()));
	/*dlugosci kosci*/
	int usedBonesSize = bonesGeometry.size();
	vector<pf::boneGeometry> usedBones = bonesGeometry;

	/*dlugosc kosci*/
	for (int i = 0; i < usedBonesSize * 6; i++) {
		sideLengthMenuHLayoutsVector.push_back(new QHBoxLayout());
		sideLengthMenuHLayoutsVector[i]->setAlignment(Qt::AlignLeft);
	}
	/*nazwa*/
	for (int i = 0; i < usedBonesSize; i++) 
		sideLengthMenuLabelsVector.push_back(new QLabel());

	/*isRot*/
	for (int i = 0; i < usedBonesSize * 3; i++) {
		sideLengthMenuRotLabelsVector.push_back(new QLabel());
		sideLengthMenuRotCheckBoxesVector.push_back(new QCheckBox());
	}

	/*Os, limits i velocity*/
	for (int i = 0; i < usedBonesSize * 9; i++) {
		sideLengthMenuLimVelLabelsVector.push_back(new QLabel());
		sideLengthMenuLimVelSpinBoxesVector.push_back(new QSpinBox());
		sideLengthMenuLimVelSpinBoxesVector[i]->setFixedWidth(42);
		sideLengthMenuLimVelSpinBoxesVector[i]->setMinimum(-360);
		sideLengthMenuLimVelSpinBoxesVector[i]->setMaximum(360);
	}

	for (int i = 0; i < usedBonesSize * 2; i++) {
		sideLengthMenuButtonsVector.push_back(new QPushButton());
		sideLengthMenuButtonsVector[i]->setFixedWidth(30);
	}

	for (int i = 0; i < usedBonesSize; i++) {
		sideLengthMenuLineEditsVector.push_back(new QLineEdit());
		sideLengthMenuLineEditsVector[i]->setFixedWidth(50);
	}

	for (int i = 0; i < usedBonesSize; i++) {
		sideLengthMenuSpinBoxesVector.push_back(new QSpinBox());
		sideLengthMenuSpinBoxesVector[i]->setFixedWidth(50);
	}

	/*parametry qspinbox - dlugosc*/
	for (int i = 0; i < sideLengthMenuLineEditsVector.size(); i++) {
		setQSpinBoxSettings(sideLengthMenuSpinBoxesVector[i], 0, 50, 20);
	}

	/*zdjecia buttonow*/
	for (int i = 0; i < sideLengthMenuButtonsVector.size(); i = i + 2) {
		addSideMenuButtonsImages(sideLengthMenuButtonsVector[i], sideLengthMenuButtonsVector[i + 1]);
	}

	/*dodanie kolejnych HLayoutow z widgetami*/
	int hLayoutCounter = 0, labelsCounter = 0, buttonsCounter = 0, lineEditsCounter = 0, spinBoxesCounter = 0, rotLabelsCounter = 0, limitsVelocityLabelsCounter = 0, limitsVelocitySpinBoxesCounter = 0;
	for (int i = 0; i < usedBonesSize; i++) {
		// nazwa
		sideLengthMenuLabelsVector[labelsCounter]->setText(QString::fromStdString(usedBones[i].name));
		sideLengthMenuLabelsVector[labelsCounter]->setStyleSheet("font-weight: bold");
		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuLabelsVector[labelsCounter]);
		hLayoutCounter++;

		//checkboxy
		sideLengthMenuRotLabelsVector[rotLabelsCounter]->setText("isRotX");
		sideLengthMenuRotLabelsVector[rotLabelsCounter + 1]->setText("isRotY");
		sideLengthMenuRotLabelsVector[rotLabelsCounter + 2]->setText("isRotZ");

		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuRotLabelsVector[rotLabelsCounter]);
		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuRotCheckBoxesVector[rotLabelsCounter]);
		sideLengthMenuHLayoutsVector[hLayoutCounter]->addSpacing(10);
		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuRotLabelsVector[rotLabelsCounter+1]);
		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuRotCheckBoxesVector[rotLabelsCounter+1]);
		sideLengthMenuHLayoutsVector[hLayoutCounter]->addSpacing(10);
		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuRotLabelsVector[rotLabelsCounter+2]);
		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuRotCheckBoxesVector[rotLabelsCounter+2]);
		
		hLayoutCounter++;
		
		//edycja kosci
		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuButtonsVector[buttonsCounter]);
		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuLineEditsVector[lineEditsCounter]);
		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuButtonsVector[buttonsCounter + 1]);
		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuSpinBoxesVector[spinBoxesCounter]);
		sideLengthMenuHLayoutsVector[hLayoutCounter]->addSpacing(30);

		lineEditsCounter++;
		buttonsCounter++;
		buttonsCounter++;
		labelsCounter++;
		hLayoutCounter++;
		spinBoxesCounter++;
		rotLabelsCounter = rotLabelsCounter + 3;

		//limits i velocity
		sideLengthMenuLimVelLabelsVector[limitsVelocityLabelsCounter]->setText("OX");
		sideLengthMenuLimVelLabelsVector[limitsVelocityLabelsCounter + 1]->setText("limits");
		sideLengthMenuLimVelLabelsVector[limitsVelocityLabelsCounter + 2]->setText("velocity");
		sideLengthMenuLimVelLabelsVector[limitsVelocityLabelsCounter + 3]->setText("OY");
		sideLengthMenuLimVelLabelsVector[limitsVelocityLabelsCounter + 4]->setText("limits");
		sideLengthMenuLimVelLabelsVector[limitsVelocityLabelsCounter + 5]->setText("velocity");
		sideLengthMenuLimVelLabelsVector[limitsVelocityLabelsCounter + 6]->setText("OZ");
		sideLengthMenuLimVelLabelsVector[limitsVelocityLabelsCounter + 7]->setText("limits");
		sideLengthMenuLimVelLabelsVector[limitsVelocityLabelsCounter + 8]->setText("velocity");

		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuLimVelLabelsVector[limitsVelocityLabelsCounter]);
		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuLimVelLabelsVector[limitsVelocityLabelsCounter + 1]);

		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuLimVelSpinBoxesVector[limitsVelocitySpinBoxesCounter]);
		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuLimVelSpinBoxesVector[limitsVelocitySpinBoxesCounter + 1]);

		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuLimVelLabelsVector[limitsVelocityLabelsCounter + 2]);
		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuLimVelSpinBoxesVector[limitsVelocitySpinBoxesCounter + 2]);

		hLayoutCounter++;

		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuLimVelLabelsVector[limitsVelocityLabelsCounter + 3]);
		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuLimVelLabelsVector[limitsVelocityLabelsCounter + 4]);

		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuLimVelSpinBoxesVector[limitsVelocitySpinBoxesCounter + 3]);
		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuLimVelSpinBoxesVector[limitsVelocitySpinBoxesCounter + 4]);

		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuLimVelLabelsVector[limitsVelocityLabelsCounter + 5]);
		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuLimVelSpinBoxesVector[limitsVelocitySpinBoxesCounter + 5]);

		hLayoutCounter++;

		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuLimVelLabelsVector[limitsVelocityLabelsCounter + 6]);
		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuLimVelLabelsVector[limitsVelocityLabelsCounter + 7]);

		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuLimVelSpinBoxesVector[limitsVelocitySpinBoxesCounter + 6]);
		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuLimVelSpinBoxesVector[limitsVelocitySpinBoxesCounter + 7]);

		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuLimVelLabelsVector[limitsVelocityLabelsCounter + 8]);
		sideLengthMenuHLayoutsVector[hLayoutCounter]->addWidget(sideLengthMenuLimVelSpinBoxesVector[limitsVelocitySpinBoxesCounter + 8]);

		sideLengthMenuHLayoutsVector[hLayoutCounter]->setContentsMargins(0, 0, 0, 10);

		limitsVelocityLabelsCounter = limitsVelocityLabelsCounter + 9;
		limitsVelocitySpinBoxesCounter = limitsVelocitySpinBoxesCounter + 9;
		hLayoutCounter++;
	}

	for (int i = 0; i < usedBonesSize; i++) {
		for (int j = 0; j < 2; j++) {
			QString str;
			switch (j) {
			case 0:
				str = QString::fromStdString(usedBones[i].name + ";-1.0");
				break;

			case 1:
				str = QString::fromStdString(usedBones[i].name + ";1.0");
				break;

			default:
				str = " ";
				cout << "ERROR" << endl;
				break;
			}
			/*qlineEditID*/
			str += QString::fromStdString(";" + std::to_string(i));

			/*id qSpinBox*/
			str += QString::fromStdString(";" + std::to_string(i));

			lengthMapperVector.push_back(new QSignalMapper(this));
			QObject::connect(lengthMapperVector[lengthMapperCounter], SIGNAL(mapped(QString)), this, SLOT(updateBoneLength(QString)));
			lengthMapperVector[lengthMapperCounter]->setMapping(sideLengthMenuButtonsVector[(i*2) + j], str);
			connect(sideLengthMenuButtonsVector[(i * 2) + j], SIGNAL(released()), lengthMapperVector[lengthMapperCounter], SLOT(map()));
			lengthMapperCounter++;
		}
	}

	vboxLengthLayout->addWidget(goldenRatioGroupBox);

	for (int hLayouts = 0; hLayouts < sideLengthMenuHLayoutsVector.size(); hLayouts++) {
		vboxLengthLayout->addLayout(sideLengthMenuHLayoutsVector[hLayouts]);
	}

	setRotCheckBoxes();
	setLimitsSpinBoxes();
	setVelocitySpinBoxes();

	for (int i = 0; i < sideLengthMenuRotCheckBoxesVector.size(); i++) {
		connect(sideLengthMenuRotCheckBoxesVector[i], SIGNAL(stateChanged(int)), this, SLOT(updateBoneConfigFromCB()));
		connect(sideLengthMenuRotCheckBoxesVector[i], SIGNAL(stateChanged(int)), this, SLOT(disableLimitsSpinBoxes()));
	}

	for (int i = 0; i < sideLengthMenuLimVelSpinBoxesVector.size(); i+=3) {
		connect(sideLengthMenuLimVelSpinBoxesVector[i], SIGNAL(valueChanged(int)), this, SLOT(updateBoneConfigFromLimitsSB()));
		connect(sideLengthMenuLimVelSpinBoxesVector[i+1], SIGNAL(valueChanged(int)), this, SLOT(updateBoneConfigFromLimitsSB()));
		connect(sideLengthMenuLimVelSpinBoxesVector[i + 2], SIGNAL(valueChanged(int)), this, SLOT(updateBoneConfigFromVelocitySB()));
	}

	disableLimitsSpinBoxes();
	//updateBoneConfigFromCB();
	refreshLengthSideMenu();

	for (int i = 0; i < usedBonesSize; i++) {
		QString str = QString::fromStdString(usedBones[i].name + ";" + std::to_string(i));
		lengthFromLineEditMapperVector.push_back(new QSignalMapper(this));

		QObject::connect(lengthFromLineEditMapperVector[i], SIGNAL(mapped(QString)), this, SLOT(setBoneLengthFromLineEdit(QString)));
		lengthFromLineEditMapperVector[i]->setMapping(sideLengthMenuLineEditsVector[i], str);
		connect(sideLengthMenuLineEditsVector[i], SIGNAL(textChanged(const QString &)), lengthFromLineEditMapperVector[i], SLOT(map()));
	}

}

void MainWindow::addGLWidgetGeometryMenu() {
	int geometryMapperCounter = 0;
	vboxGeometryLayout->setAlignment(Qt::AlignTop);

	int usedBonesSize = bonesGeometry.size();
	vector<pf::boneGeometry> usedBones = bonesGeometry;

	for (int i = 0; i < usedBonesSize * 4; i++) {
		sideGeometryMenuLabelsVector.push_back(new QLabel());
		sideGeometryMenuHLayoutsVector.push_back(new QHBoxLayout());
		sideGeometryMenuHLayoutsVector[i]->setAlignment(Qt::AlignLeft);
	}
	
	for (int i = 0; i < usedBonesSize * 3; i++) {
		sideGeometryMenuLineEditsVector.push_back(new QLineEdit());
		sideGeometryMenuLineEditsVector[i]->setFixedWidth(50);
		sideGeometryMenuSpinBoxesVector.push_back(new QSpinBox());
		sideGeometryMenuSpinBoxesVector[i]->setFixedWidth(50);
	}

	for (int i = 0; i < usedBonesSize * 6; i++) {
		sideGeometryMenuButtonsVector.push_back(new QPushButton(this));
		sideGeometryMenuButtonsVector[i]->setFixedWidth(30);
	}

	int hLayoutCounter = 0, labelsCounter = 0, buttonsCounter = 0, lineEditsCounter = 0, spinBoxesCounter = 0;
	for (int i = 0; i < usedBonesSize; i++) {
		sideGeometryMenuLabelsVector[labelsCounter]->setText(QString::fromStdString(usedBones[i].name));
		sideGeometryMenuLabelsVector[labelsCounter]->setStyleSheet("font-weight: bold");
		sideGeometryMenuHLayoutsVector[hLayoutCounter]->addWidget(sideGeometryMenuLabelsVector[labelsCounter]);
		labelsCounter++;
		hLayoutCounter++;

		sideGeometryMenuLabelsVector[labelsCounter]->setText("Length");
		sideGeometryMenuHLayoutsVector[hLayoutCounter]->addWidget(sideGeometryMenuLabelsVector[labelsCounter]);
		sideGeometryMenuHLayoutsVector[hLayoutCounter]->addWidget(sideGeometryMenuButtonsVector[buttonsCounter]);
		sideGeometryMenuHLayoutsVector[hLayoutCounter]->addWidget(sideGeometryMenuLineEditsVector[lineEditsCounter]);
		sideGeometryMenuHLayoutsVector[hLayoutCounter]->addWidget(sideGeometryMenuButtonsVector[buttonsCounter + 1]);
		sideGeometryMenuHLayoutsVector[hLayoutCounter]->addWidget(sideGeometryMenuSpinBoxesVector[spinBoxesCounter]);

		lineEditsCounter++;
		buttonsCounter++;
		buttonsCounter++;
		labelsCounter++;
		hLayoutCounter++;
		spinBoxesCounter++;

		sideGeometryMenuLabelsVector[labelsCounter]->setText("Radius1");
		sideGeometryMenuHLayoutsVector[hLayoutCounter]->addWidget(sideGeometryMenuLabelsVector[labelsCounter]);
		sideGeometryMenuHLayoutsVector[hLayoutCounter]->addWidget(sideGeometryMenuButtonsVector[buttonsCounter]);
		sideGeometryMenuHLayoutsVector[hLayoutCounter]->addWidget(sideGeometryMenuLineEditsVector[lineEditsCounter]);
		sideGeometryMenuHLayoutsVector[hLayoutCounter]->addWidget(sideGeometryMenuButtonsVector[buttonsCounter + 1]);
		sideGeometryMenuHLayoutsVector[hLayoutCounter]->addWidget(sideGeometryMenuSpinBoxesVector[spinBoxesCounter]);

		lineEditsCounter++;
		buttonsCounter++;
		buttonsCounter++;
		labelsCounter++;
		hLayoutCounter++;
		spinBoxesCounter++;

		sideGeometryMenuLabelsVector[labelsCounter]->setText("Radius2");
		sideGeometryMenuHLayoutsVector[hLayoutCounter]->addWidget(sideGeometryMenuLabelsVector[labelsCounter]);
		sideGeometryMenuHLayoutsVector[hLayoutCounter]->addWidget(sideGeometryMenuButtonsVector[buttonsCounter]);
		sideGeometryMenuHLayoutsVector[hLayoutCounter]->addWidget(sideGeometryMenuLineEditsVector[lineEditsCounter]);
		sideGeometryMenuHLayoutsVector[hLayoutCounter]->addWidget(sideGeometryMenuButtonsVector[buttonsCounter + 1]);
		sideGeometryMenuHLayoutsVector[hLayoutCounter]->addWidget(sideGeometryMenuSpinBoxesVector[spinBoxesCounter]);

		sideGeometryMenuHLayoutsVector[hLayoutCounter]->setContentsMargins(0, 0, 0, 10);

		lineEditsCounter++;
		buttonsCounter++;
		buttonsCounter++;
		labelsCounter++;
		hLayoutCounter++;
		spinBoxesCounter++;
	}

	for (int i = 0; i < usedBonesSize; i++) {
		//dlugosc, top, bottom radius *2
		for (int j = 0; j < 6; j++) {
			QString str;
			switch (j) {
			case 0:
				str = QString::fromStdString(usedBones[i].name + ";-1.0");
				break;

			case 1:
				str = QString::fromStdString(usedBones[i].name + ";1.0");
				break;

			case 2:
				str = QString::fromStdString(usedBones[i].name + ";-1.0");
				break;

			case 3:
				str = QString::fromStdString(usedBones[i].name + ";1.0");
				break;

			case 4:
				str = QString::fromStdString(usedBones[i].name + ";-1.0");
				break;

			case 5:
				str = QString::fromStdString(usedBones[i].name + ";1.0");
				break;

			default:
				str = " ";
				cout << "BLAD" << endl;
				break;
			}

			int tmpVal;
			if (j < 2) {
				/*id qLineEdit*/
				str += QString::fromStdString(";" + std::to_string(i * 3));

				/*id qSpinBox*/
				str += QString::fromStdString(";" + std::to_string(i * 3));

				geometryMapperVector.push_back(new QSignalMapper(this));
				QObject::connect(geometryMapperVector[geometryMapperCounter], SIGNAL(mapped(QString)), this, SLOT(updateBoneLengthGeometry(QString)));
				geometryMapperVector[geometryMapperCounter]->setMapping(sideGeometryMenuButtonsVector[(i * 6) + j], str);
				connect(sideGeometryMenuButtonsVector[(i * 6) + j], SIGNAL(released()), geometryMapperVector[geometryMapperCounter], SLOT(map()));
				geometryMapperCounter++;
			}
			else if (j>=2 && j<4){
				/*id qLineEdit*/
				str += QString::fromStdString(";" + std::to_string(i * 3 + 1));

				/*id qSpinBox*/
				str += QString::fromStdString(";" + std::to_string(i * 3 + 1));

				geometryMapperVector.push_back(new QSignalMapper(this));
				QObject::connect(geometryMapperVector[geometryMapperCounter], SIGNAL(mapped(QString)), this, SLOT(updateBoneTopRadius(QString)));
				geometryMapperVector[geometryMapperCounter]->setMapping(sideGeometryMenuButtonsVector[(i * 6) + j], str);
				connect(sideGeometryMenuButtonsVector[(i * 6) + j], SIGNAL(released()), geometryMapperVector[geometryMapperCounter], SLOT(map()));
				geometryMapperCounter++;
			}
			else {
				/*id qLineEdit*/
				str += QString::fromStdString(";" + std::to_string(i * 3 + 2));

				/*id qSpinBox*/
				str += QString::fromStdString(";" + std::to_string(i * 3 + 2));

				geometryMapperVector.push_back(new QSignalMapper(this));
				QObject::connect(geometryMapperVector[geometryMapperCounter], SIGNAL(mapped(QString)), this, SLOT(updateBoneBottomRadius(QString)));
				geometryMapperVector[geometryMapperCounter]->setMapping(sideGeometryMenuButtonsVector[(i * 6) + j], str);
				connect(sideGeometryMenuButtonsVector[(i * 6) + j], SIGNAL(released()), geometryMapperVector[geometryMapperCounter], SLOT(map()));
				geometryMapperCounter++;
			}
		}
	}
	
	/*parametry qspinbox*/
	for (int i = 0; i < sideGeometryMenuSpinBoxesVector.size(); i=i+3) {
		setQSpinBoxSettings(sideGeometryMenuSpinBoxesVector[i], 0, 50, 20);
		setQSpinBoxSettings(sideGeometryMenuSpinBoxesVector[i+1], 0, 20, 5);
		setQSpinBoxSettings(sideGeometryMenuSpinBoxesVector[i+2], 0, 20, 5);
	}

	/*zdjecia buttonow*/
	for (int i = 0; i < sideGeometryMenuButtonsVector.size(); i = i + 2) {
		addSideMenuButtonsImages(sideGeometryMenuButtonsVector[i], sideGeometryMenuButtonsVector[i + 1]);
	}

	for (int hLayouts = 0; hLayouts < sideGeometryMenuHLayoutsVector.size(); hLayouts++)
		vboxGeometryLayout->addLayout(sideGeometryMenuHLayoutsVector[hLayouts]);

	refreshGeometrySideMenu();

	for (int i = 0; i < usedBonesSize; i++) {
		geometryLengthFromLineEditMapperVector.push_back(new QSignalMapper(this));
		geometryTopRadiusFromLineEditMapperVector.push_back(new QSignalMapper(this));
		geometryBottomRadiusFromLineEditMapperVector.push_back(new QSignalMapper(this));
		//sideGeometryMenuLineEditsVector
		for (int j = 0; j < 3; j++) {
			int id = i * 3 + j;
			QString str = QString::fromStdString(usedBones[i].name) + QString::fromStdString(";" + std::to_string(id));
			if (j == 0) {
				QObject::connect(geometryLengthFromLineEditMapperVector[i], SIGNAL(mapped(QString)), this, SLOT(setBoneLengthGeometryFromLineEdit(QString)));
				geometryLengthFromLineEditMapperVector[i]->setMapping(sideGeometryMenuLineEditsVector[id], str);
				connect(sideGeometryMenuLineEditsVector[id], SIGNAL(textChanged(const QString &)), geometryLengthFromLineEditMapperVector[i], SLOT(map()));
			}
			else if (j == 1) {
				QObject::connect(geometryTopRadiusFromLineEditMapperVector[i], SIGNAL(mapped(QString)), this, SLOT(setBoneTopRadiusFromLineEdit(QString)));
				geometryTopRadiusFromLineEditMapperVector[i]->setMapping(sideGeometryMenuLineEditsVector[id], str);
				connect(sideGeometryMenuLineEditsVector[id], SIGNAL(textChanged(const QString &)), geometryTopRadiusFromLineEditMapperVector[i], SLOT(map()));
			}
			else {
				QObject::connect(geometryBottomRadiusFromLineEditMapperVector[i], SIGNAL(mapped(QString)), this, SLOT(setBoneBottomRadiusFromLineEdit(QString)));
				geometryBottomRadiusFromLineEditMapperVector[i]->setMapping(sideGeometryMenuLineEditsVector[id], str);
				connect(sideGeometryMenuLineEditsVector[id], SIGNAL(textChanged(const QString &)), geometryBottomRadiusFromLineEditMapperVector[i], SLOT(map()));
			}
		}
	}
}

void MainWindow::addFrameScrollingMenu(QPushButton *button, int id) {
	frameScrollingMenu.push_back(new QMenu());
	selectBackgroundMenuVector.push_back(new QMenu("Add background"));
	selectCameraMenuVector.push_back(new QMenu("Select camera"));
	resizeGLWidgetVector.push_back(new QMenu("Resize"));

	resizeDownActionVector.push_back(new QAction("Scale down")); //pomniejsz glwidget
	resizeUpActionVector.push_back(new QAction("Scale up")); //powieksz glwidget

	/*###*/
	selectImagesBackgroundVector.push_back(new QAction("Load images"));
	selectAviBackgroundVector.push_back(new QAction("Load avi"));

	/*###*/
	deleteFrameMenuVector.push_back(new QAction("Delete frame"));

	selectBackgroundMenuVector[id]->addAction(selectImagesBackgroundVector[id]);
	selectBackgroundMenuVector[id]->addAction(selectAviBackgroundVector[id]);

	resizeGLWidgetVector[id]->addAction(resizeDownActionVector[id]);
	resizeGLWidgetVector[id]->addAction(resizeUpActionVector[id]);
	selectCameraMenuVector[id]->setDisabled(true);

	frameScrollingMenu[id]->addMenu(resizeGLWidgetVector[id]);
	frameScrollingMenu[id]->addSeparator();
	frameScrollingMenu[id]->addMenu(selectBackgroundMenuVector[id]);
	frameScrollingMenu[id]->addMenu(selectCameraMenuVector[id]);
	frameScrollingMenu[id]->addSeparator();
	frameScrollingMenu[id]->addAction(deleteFrameMenuVector[id]);
	button->setMenu(frameScrollingMenu[id]);
}

void MainWindow::setQSpinBoxSettings(QSpinBox * qspinbox, int minRange, int maxRange, int defaultValue) {
	qspinbox->setRange(minRange, maxRange);
	qspinbox->setValue(defaultValue);
}

void MainWindow::setRotCheckBoxes() {
	/*odznacz wszystkie*/
	for (int i = 0; i < sideLengthMenuRotCheckBoxesVector.size(); i++) {
		sideLengthMenuRotCheckBoxesVector[i]->setChecked(false);
	}

	for (int j = 0; j < bonesGeometry.size(); j++) {
		
		for (int i = 0; i < bonesConf.size(); i++) {
			if (bonesConf[i].name == bonesGeometry[j].name) {
				
				string name = bonesConf[i].name;

				if (modelDOF.find(name)->second[0]) {
					sideLengthMenuRotCheckBoxesVector[j * 3]->setChecked(true);
				}
				else {
					sideLengthMenuRotCheckBoxesVector[j * 3]->setChecked(false);
				}

				if (modelDOF.find(name)->second[1])
					sideLengthMenuRotCheckBoxesVector[j * 3 + 1]->setChecked(true);
				else
					sideLengthMenuRotCheckBoxesVector[j * 3 + 1]->setChecked(false);

				if (modelDOF.find(name)->second[2])
					sideLengthMenuRotCheckBoxesVector[j * 3 + 2]->setChecked(true);
				else
					sideLengthMenuRotCheckBoxesVector[j * 3 + 2]->setChecked(false);

				break;
				//std::string().swap(name);
			}
		}
	}
}

void MainWindow::setLimitsSpinBoxes() {
	vector<vector<int>> lims;
	for (int i = 0; i < bonesGeometry.size(); i++) {
		vector<int> tmp;
		string name = bonesGeometry[i].name;
		/*pobierz limity z mapy*/
		tmp = modelLimits.find(name)->second;
		lims.push_back(tmp);
	}

	for (int j = 0; j < bonesGeometry.size(); j++) {
		//OX
		sideLengthMenuLimVelSpinBoxesVector[j * 9]->setValue(lims[j][0]);
		sideLengthMenuLimVelSpinBoxesVector[j * 9 + 1]->setValue(lims[j][1]);
		//OY
		sideLengthMenuLimVelSpinBoxesVector[j * 9 + 3]->setValue(lims[j][2]);
		sideLengthMenuLimVelSpinBoxesVector[j * 9 + 4]->setValue(lims[j][3]);
		//OZ
		sideLengthMenuLimVelSpinBoxesVector[j * 9 + 6]->setValue(lims[j][4]);
		sideLengthMenuLimVelSpinBoxesVector[j * 9 + 7]->setValue(lims[j][5]);		
	}

	/* nie moze byc sytuacji gdy min > max oraz max < min */
	for (int j = 0; j < bonesGeometry.size(); j++) {
		//OX
		sideLengthMenuLimVelSpinBoxesVector[j * 9]->setMaximum(sideLengthMenuLimVelSpinBoxesVector[j * 9 + 1]->value());
		sideLengthMenuLimVelSpinBoxesVector[j * 9 + 1]->setMinimum(sideLengthMenuLimVelSpinBoxesVector[j * 9]->value());
		//OY
		sideLengthMenuLimVelSpinBoxesVector[j * 9 + 3]->setMaximum(sideLengthMenuLimVelSpinBoxesVector[j * 9 + 4]->value());
		sideLengthMenuLimVelSpinBoxesVector[j * 9 + 4]->setMinimum(sideLengthMenuLimVelSpinBoxesVector[j * 9 + 3]->value());
		//OZ
		sideLengthMenuLimVelSpinBoxesVector[j * 9 + 6]->setMaximum(sideLengthMenuLimVelSpinBoxesVector[j * 9 + 7]->value());
		sideLengthMenuLimVelSpinBoxesVector[j * 9 + 7]->setMinimum(sideLengthMenuLimVelSpinBoxesVector[j * 9 + 6]->value());
	}
}

void MainWindow::setVelocitySpinBoxes() {
	vector<vector<float>> velocities;
	for (int i = 0; i < bonesGeometry.size(); i++) {
		vector<float> tmp;
		string name = bonesGeometry[i].name;
		/*pobierz velocity z mapy*/
		tmp = modelVelocity.find(name)->second;
		velocities.push_back(tmp);
	}

	for (int j = 0; j < bonesGeometry.size(); j++) {
		string name = bonesGeometry[j].name;
		//velocity X
		sideLengthMenuLimVelSpinBoxesVector[j * 9 + 2]->setValue(velocities[j][0]);
		//velocity Y
		sideLengthMenuLimVelSpinBoxesVector[j * 9 + 5]->setValue(velocities[j][1]);
		//velocity Z
		sideLengthMenuLimVelSpinBoxesVector[j * 9 + 8]->setValue(velocities[j][2]);
	}
}

void MainWindow::updateBoneConfigFromCB() {
	ModelHandler modelHandler;
	int frameID = comboBoxFrameSelector->currentIndex();

	/*zapisz modelState oraz dlugosci kosci do map*/
	for (int j = 0; j < modelState.size(); j++) {
		modelHandler.saveModelStateToMap(bonesRotations[j], modelState[j], bonesConf);
	}
	
	modelHandler.saveBonesLengthToMap(bonesLength, bonesGeometry);

	bonesConf.clear();

	/*add root to bones config*/

	pf::boneConfig bConf;
	string name = "root";
	bConf.name = name;
	/*dof*/
	bConf.isRotX = true;
	bConf.isRotY = true;
	bConf.isRotZ = true;

	bConf.isTransX = true;
	bConf.isTransY = true;
	bConf.isTransZ = true;

	/*rot*/
	bConf.rotX = 0;
	bConf.rotY = 0;
	bConf.rotZ = 0;

	/*limits*/
	bConf.minRotX = modelLimits.find(name)->second[0];
	bConf.maxRotX = modelLimits.find(name)->second[1];
	bConf.minRotY = modelLimits.find(name)->second[2];
	bConf.maxRotY = modelLimits.find(name)->second[3];
	bConf.minRotZ = modelLimits.find(name)->second[4];
	bConf.maxRotZ = modelLimits.find(name)->second[5];

	/*velocity*/
	bConf.vTransX = modelVelocity.find(name)->second[0];
	bConf.vTransY = modelVelocity.find(name)->second[1];
	bConf.vTransZ = modelVelocity.find(name)->second[2];
	bConf.vRotX = modelVelocity.find(name)->second[3];
	bConf.vRotY = modelVelocity.find(name)->second[4];
	bConf.vRotZ = modelVelocity.find(name)->second[5];

	bonesConf.push_back(bConf);

	for (int i = 0; i < bonesGeometry.size(); i++) {
		//if (sideLengthMenuRotCheckBoxesVector[i*3]->isChecked() || sideLengthMenuRotCheckBoxesVector[i * 3 + 1]->isChecked() || sideLengthMenuRotCheckBoxesVector[i * 3 + 2]->isChecked()) {
			pf::boneConfig bConf;

			string name = bonesGeometry[i].name;
			bConf.name = name;
			
			/*dof*/
			if (sideLengthMenuRotCheckBoxesVector[i*3]->isChecked())
				bConf.isRotX = true;
			else bConf.isRotX = false;

			if (sideLengthMenuRotCheckBoxesVector[i*3+1]->isChecked())
				bConf.isRotY = true;
			else bConf.isRotY = false;

			if (sideLengthMenuRotCheckBoxesVector[i*3+2]->isChecked())
				bConf.isRotZ = true;
			else bConf.isRotZ = false;

			bConf.isTransX = false;
			bConf.isTransY = false;
			bConf.isTransZ = false;
			
			/*rot*/
			bConf.rotX = 0;
			bConf.rotY = 0;
			bConf.rotZ = 0;

			/*limits*/
			bConf.minRotX = modelLimits.find(name)->second[0];
			bConf.maxRotX = modelLimits.find(name)->second[1];
			bConf.minRotY = modelLimits.find(name)->second[2];
			bConf.maxRotY = modelLimits.find(name)->second[3];
			bConf.minRotZ = modelLimits.find(name)->second[4];
			bConf.maxRotZ = modelLimits.find(name)->second[5];

			/*velocity*/
			bConf.vRotX = modelVelocity.find(name)->second[0];
			bConf.vRotY = modelVelocity.find(name)->second[1];
			bConf.vRotZ = modelVelocity.find(name)->second[2];

			if (sideLengthMenuRotCheckBoxesVector[i * 3]->isChecked() || sideLengthMenuRotCheckBoxesVector[i * 3 + 1]->isChecked() || sideLengthMenuRotCheckBoxesVector[i * 3 + 2]->isChecked())
				bonesConf.push_back(bConf);
			
			if (name != "root") {
				vector<bool> tmpDOFs;
				if (bConf.isRotX)
					tmpDOFs.push_back(1);
				else
					tmpDOFs.push_back(0);

				if (bConf.isRotY)
					tmpDOFs.push_back(1);
				else
					tmpDOFs.push_back(0);

				if (bConf.isRotZ)
					tmpDOFs.push_back(1);
				else
					tmpDOFs.push_back(0);

				modelHandler.updateBonesDOFMap(modelDOF, bConf.name, tmpDOFs);
			}
	}

	/*odswiez model*/
	FileHandler fileHandler;// = new FileHandler();

	fileHandler.saveDATToFile(bonesConf, bonesGeometry, "tmpDatFile.dat");

	bonesConf.clear();
	bonesGeometry.clear();

	mainModel = new pf::Model3D(pf::Model3D::Cylinder, ASF_TEMPLATE_PATH, "tmpDatFile.dat");
	mainModel->loadConfig("tmpDatFile.dat", bonesConf, bonesGeometry);

	std::remove("tmpDatFile.dat");


	/*odswiez model oraz mapy*/
	modelHandler.updateModelStateFromMap(modelState, bonesRotations, bonesConf, modelTranslation);
	mainModel->updateModelState(modelState[0]);
	for (int i = 0; i < glWidgetsVector.size(); i++) {
		if (asfLoaded)
			fileHandler.reloadParams(glWidgetsVector[i], bonesConf, bonesGeometry, asfPath);
		else
			fileHandler.reloadParams(glWidgetsVector[i], bonesConf, bonesGeometry, ASF_TEMPLATE_PATH);
	}

	modelHandler.updateBonesLengthFromMap(bonesLength, bonesGeometry);
	modelHandler.setLimitsVector(limits, bonesConf, bonesGeometry);
	modelHandler.setVelocityVector(velocity, bonesConf, bonesGeometry);

	reloadGLRotateMenu();
	refreshLengthSideMenu();
	//delete fileHandler;
}

void MainWindow::updateBoneConfigFromLimitsSB() {
	ModelHandler modelHandler;
	int frameID = comboBoxFrameSelector->currentIndex();

	/*aktualizacja mapy limitow danej kosci*/
	for (int i = 0; i < bonesGeometry.size(); i++) {
		vector<int> tmp;
		string name = bonesGeometry[i].name;

		tmp.push_back(sideLengthMenuLimVelSpinBoxesVector[i * 9]->value());
		tmp.push_back(sideLengthMenuLimVelSpinBoxesVector[i * 9 + 1]->value());
		tmp.push_back(sideLengthMenuLimVelSpinBoxesVector[i * 9 + 3]->value());
		tmp.push_back(sideLengthMenuLimVelSpinBoxesVector[i * 9 + 4]->value());
		tmp.push_back(sideLengthMenuLimVelSpinBoxesVector[i * 9 + 6]->value());
		tmp.push_back(sideLengthMenuLimVelSpinBoxesVector[i * 9 + 7]->value());
		modelLimits.at(name) = tmp;
	}

	for (int i = 1; i < bonesConf.size(); i++) {
		string name = bonesConf[i].name;
		vector <int> tmp = modelLimits.find(name)->second;

		bonesConf[i].minRotX = tmp[0];
		bonesConf[i].maxRotX = tmp[1];
		bonesConf[i].minRotY = tmp[2];
		bonesConf[i].maxRotY = tmp[3];
		bonesConf[i].minRotZ = tmp[4];
		bonesConf[i].maxRotZ = tmp[5];
	}

	/* nie moze byc sytuacji gdy min > max oraz max < min */
	for (int j = 0; j < bonesGeometry.size(); j++) {
		//OX
		sideLengthMenuLimVelSpinBoxesVector[j * 9]->setMaximum(sideLengthMenuLimVelSpinBoxesVector[j * 9 + 1]->value());
		sideLengthMenuLimVelSpinBoxesVector[j * 9 + 1]->setMinimum(sideLengthMenuLimVelSpinBoxesVector[j * 9]->value());
		//OY
		sideLengthMenuLimVelSpinBoxesVector[j * 9 + 3]->setMaximum(sideLengthMenuLimVelSpinBoxesVector[j * 9 + 4]->value());
		sideLengthMenuLimVelSpinBoxesVector[j * 9 + 4]->setMinimum(sideLengthMenuLimVelSpinBoxesVector[j * 9 + 3]->value());
		//OZ
		sideLengthMenuLimVelSpinBoxesVector[j * 9 + 6]->setMaximum(sideLengthMenuLimVelSpinBoxesVector[j * 9 + 7]->value());
		sideLengthMenuLimVelSpinBoxesVector[j * 9 + 7]->setMinimum(sideLengthMenuLimVelSpinBoxesVector[j * 9 + 6]->value());
	}

	modelHandler.setLimitsVector(limits, bonesConf, bonesGeometry);

	//reloadGLRotateMenu();
}

void MainWindow::updateBoneConfigFromVelocitySB() {
	ModelHandler modelHandler;
	/*aktualizacja mapy velocity danej kosci*/
	for (int i = 0; i < bonesGeometry.size(); i++) {
		vector<float> tmp;
		string name = bonesGeometry[i].name;

		tmp.push_back(sideLengthMenuLimVelSpinBoxesVector[i * 9 + 2]->value());
		tmp.push_back(sideLengthMenuLimVelSpinBoxesVector[i * 9 + 5]->value());
		tmp.push_back(sideLengthMenuLimVelSpinBoxesVector[i * 9 + 8]->value());
		modelVelocity.at(name) = tmp;
	}

	for (int i = 1; i < bonesConf.size(); i++) {
		string name = bonesConf[i].name;
		vector<float> tmp = modelVelocity.find(name)->second;

		bonesConf[i].vRotX = tmp[0];
		bonesConf[i].vRotY = tmp[1];
		bonesConf[i].vRotZ = tmp[2];

	}

	modelHandler.setVelocityVector(velocity, bonesConf, bonesGeometry);
	//reloadGLRotateMenu();
}

void MainWindow::disableRotatingButtons() {
	/*limity kosci modelu*/
	vector<pf::range2> bonesLimits = limits;

	/*wylaczenie przyciskow*/
	vector<pf::boneConfig> bConf = bonesConf;
	for (int i = 0; i < bConf.size(); i++) {
		if (!bConf[i].isRotX) {
			sideRotateMenuButtonsVector[i * 6]->setDisabled(true);
			sideRotateMenuButtonsVector[i * 6 + 1]->setDisabled(true);
			sideRotateMenuLineEditsVector[i * 3]->setDisabled(true);
			sideRotateMenuSpinBoxesVector[i * 3]->setDisabled(true);
		} 
		else {
			sideRotateMenuButtonsVector[i * 6]->setToolTip("<b>min:</b> " + QString::number(bonesLimits[i * 3 + 3].min));
			sideRotateMenuButtonsVector[i * 6 + 1]->setToolTip("<b>max:</b> " + QString::number(bonesLimits[i * 3 + 3].max));
		}
		
		if (!bConf[i].isRotY) {
			sideRotateMenuButtonsVector[i * 6 + 2]->setDisabled(true);
			sideRotateMenuButtonsVector[i * 6 + 3]->setDisabled(true);
			sideRotateMenuLineEditsVector[i * 3 + 1]->setDisabled(true);
			sideRotateMenuSpinBoxesVector[i * 3 + 1]->setDisabled(true);
		}
		else {
			sideRotateMenuButtonsVector[i * 6 + 2]->setToolTip("<b>min:</b> " + QString::number(bonesLimits[i * 3 + 4].min));
			sideRotateMenuButtonsVector[i * 6 + 3]->setToolTip("<b>max:</b> " + QString::number(bonesLimits[i * 3 + 4].max));
		}

		if (!bConf[i].isRotZ) {
			sideRotateMenuButtonsVector[i * 6 + 4]->setDisabled(true);
			sideRotateMenuButtonsVector[i * 6 + 5]->setDisabled(true);
			sideRotateMenuLineEditsVector[i * 3 + 2]->setDisabled(true);
			sideRotateMenuSpinBoxesVector[i * 3 + 2]->setDisabled(true);
		}
		else {
			sideRotateMenuButtonsVector[i * 6 + 4]->setToolTip("<b>min:</b> " + QString::number(bonesLimits[i * 3 + 5].min));
			sideRotateMenuButtonsVector[i * 6 + 5]->setToolTip("<b>max:</b> " + QString::number(bonesLimits[i * 3 + 5].max));
		}
	}
}

void MainWindow::disableLimitsSpinBoxes() {
	vector<pf::boneGeometry> bGeo = bonesGeometry;
	vector<pf::boneConfig> bConf = bonesConf;

	/*wylaczenie spinboxes limitow*/
	for (int i = 0; i < bGeo.size(); i++) {
		if (sideLengthMenuRotCheckBoxesVector[i * 3]->checkState() == Qt::Unchecked) {
			sideLengthMenuLimVelSpinBoxesVector[i * 9]->setDisabled(true);
			sideLengthMenuLimVelSpinBoxesVector[i * 9 + 1]->setDisabled(true);
		}
		else {
			sideLengthMenuLimVelSpinBoxesVector[i * 9]->setDisabled(false);
			sideLengthMenuLimVelSpinBoxesVector[i * 9 + 1]->setDisabled(false);
		}

		if (sideLengthMenuRotCheckBoxesVector[i * 3 + 1]->checkState() == Qt::Unchecked) {
			sideLengthMenuLimVelSpinBoxesVector[i * 9 + 3]->setDisabled(true);
			sideLengthMenuLimVelSpinBoxesVector[i * 9 + 4]->setDisabled(true);
		}
		else {
			sideLengthMenuLimVelSpinBoxesVector[i * 9 + 3]->setDisabled(false);
			sideLengthMenuLimVelSpinBoxesVector[i * 9 + 4]->setDisabled(false);
		}

		if (sideLengthMenuRotCheckBoxesVector[i * 3 + 2]->checkState() == Qt::Unchecked) {
			sideLengthMenuLimVelSpinBoxesVector[i * 9 + 6]->setDisabled(true);
			sideLengthMenuLimVelSpinBoxesVector[i * 9 + 7]->setDisabled(true);
		}
		else {
			sideLengthMenuLimVelSpinBoxesVector[i * 9 + 6]->setDisabled(false);
			sideLengthMenuLimVelSpinBoxesVector[i * 9 + 7]->setDisabled(false);
		}

	}
}

void MainWindow::tabSelected() {
	if (sideManuTabWidget->currentIndex() == 1) {
		//reloadGLRotateMenu();
		disableRotatingButtons();
		refreshRotateSideMenu();
	}
	else if (sideManuTabWidget->currentIndex() == 0) {
		refreshLengthSideMenu();
	}
	else {
		refreshGeometrySideMenu();
	}

	if (sideManuTabWidget->currentIndex() == 2) {
		for (int i = 0; i < glWidgetsVector.size(); i++) {
			glWidgetsVector[i]->mode = 2;
			glWidgetsVector[i]->update();
		}
	}
	else {
		for (int i = 0; i < glWidgetsVector.size(); i++) {
			glWidgetsVector[i]->mode = 1;
			glWidgetsVector[i]->update();
		}
	}
}

void MainWindow::addFrameMenuMappers(int id) {
	increaseButtonMapperVector.push_back(new QSignalMapper(this));
	reduceButtonMapperVector.push_back(new QSignalMapper(this));
	minButtonMapperVector.push_back(new QSignalMapper(this));
	maxButtonMapperVector.push_back(new QSignalMapper(this));
	playPauseButtonMapperVector.push_back(new QSignalMapper(this));
	timerMapperVector.push_back(new QSignalMapper(this));
	addImagesBackgroundMapperVector.push_back(new QSignalMapper(this));
	deleteFrameMapperVector.push_back(new QSignalMapper(this));
	addAviMapperVector.push_back(new QSignalMapper(this));
	sliderMapperVector.push_back(new QSignalMapper(this));
	resizeDownGLWidgetMapperVector.push_back(new QSignalMapper(this));
	resizeUpGLWidgetMapperVector.push_back(new QSignalMapper(this));
}

void MainWindow::addFrameMenuButtonsImages(QPushButton *minButton, QPushButton *reduceButton, QPushButton *increaseButton, QPushButton *maxButton, QPushButton *playButton) {
	QPixmap pixIncrease(":/images/ETC/buttonImages/increase.png"),
		pixReduce(":/images/ETC/buttonImages/reduce.png"),
		pixMax(":/images/ETC/buttonImages/max.png"),
		pixMin(":/images/ETC/buttonImages/min.png"),
		pixPlay(":/images/ETC/buttonImages/play.png");
	QIcon iconIncrease(pixIncrease), iconReduce(pixReduce), iconMax(pixMax), iconMin(pixMin), iconPlay(pixPlay);
	minButton->setIcon(iconMin);
	reduceButton->setIcon(iconReduce);
	increaseButton->setIcon(iconIncrease);
	maxButton->setIcon(iconMax);
	playButton->setIcon(pixPlay);
}

void MainWindow::addGlobalSliderButtonsImages(QPushButton *reduceButton, QPushButton *increaseButton) {
	QPixmap pixIncrease(":/images/ETC/buttonImages/increase.png"),
		pixReduce(":/images/ETC/buttonImages/reduce.png");
	QIcon iconIncrease(pixIncrease), iconReduce(pixReduce);
	reduceButton->setIcon(iconReduce);
	increaseButton->setIcon(iconIncrease);
}

void MainWindow::addSideMenuButtonsImages(QPushButton *substractButton, QPushButton *addButton) {
	QPixmap pixAdd(":/images/ETC/buttonImages/plus.png"), pixSubstract(":/images/ETC/buttonImages/substract.png");
	QIcon iconAdd(pixAdd), iconSubstract(pixSubstract);
	substractButton->setIcon(iconSubstract);
	addButton->setIcon(iconAdd);
}

void MainWindow::saveCameras(QString fileName) {
	QSettings config(fileName, QSettings::IniFormat);
	config.clear();
	config.beginGroup("Cameras");
	config.setValue("number", this->cameras.size());
	config.endGroup();
	for (unsigned int i = 0; i<this->cameras.size(); ++i)
	{
		if (cameras[i]->getType() == 3 || cameras[i]->getType() == 4 || cameras[i]->getType() == 6 ) {
			config.beginGroup("Camera" + QString::number(i + 1));
			config.setValue("type", cameras[i]->getType());
			config.setValue("width", QString::number(cameras[i]->getWidth()));
			config.setValue("height", QString::number(cameras[i]->getHeight()));
			config.setValue("position_x", QString::number(0));
			config.setValue("position_y", QString::number(0));
			config.setValue("position_z", QString::number(0));
			config.setValue("use_quat", QString::number(0));
			config.setValue("quat_x", QString::number(0));
			config.setValue("quat_y", QString::number(0));
			config.setValue("quat_z", QString::number(0));
			config.setValue("quat_w", QString::number(1));
			config.setValue("rot_x", QString::number(0));
			config.setValue("rot_y", QString::number(0));
			config.setValue("rot_z", QString::number(0));
			config.setValue("fovy", QString::number(0));
			config.setValue("dis_k1", QString::number(0));
			config.setValue("dis_k2", QString::number(0));
			config.setValue("dis_p1", QString::number(0));
			config.setValue("dis_p2", QString::number(0));
			config.setValue("tsai1_trans_x", QString::number(cameras[i]->getTSAI1Param().trans.x));
			config.setValue("tsai1_trans_y", QString::number(cameras[i]->getTSAI1Param().trans.y));
			config.setValue("tsai1_trans_z", QString::number(cameras[i]->getTSAI1Param().trans.z));
			config.setValue("tsai1_rot_x", QString::number(RAD_TO_DEG*cameras[i]->getTSAI1Param().rot.x));
			config.setValue("tsai1_rot_y", QString::number(RAD_TO_DEG*cameras[i]->getTSAI1Param().rot.y));
			config.setValue("tsai1_rot_z", QString::number(RAD_TO_DEG*cameras[i]->getTSAI1Param().rot.z));
			config.setValue("tsai1_focal", QString::number(cameras[i]->getTSAI1Param().focal));
			config.setValue("tsai1_kappa1", QString::number(cameras[i]->getTSAI1Param().kappa1));
			config.setValue("tsai1_cx", QString::number(cameras[i]->getTSAI1Param().c.x));
			config.setValue("tsai1_cy", QString::number(cameras[i]->getTSAI1Param().c.y));
			config.setValue("tsai1_sx", QString::number(cameras[i]->getTSAI1Param().sx));
			config.setValue("tsai1_ncx", QString::number(cameras[i]->getTSAI1Param().ncx));
			config.setValue("tsai1_nfx", QString::number(cameras[i]->getTSAI1Param().nfx));
			config.setValue("tsai1_dx", QString::number(cameras[i]->getTSAI1Param().d.x));
			config.setValue("tsai1_dy", QString::number(cameras[i]->getTSAI1Param().d.y));
			config.setValue("tsai1_dpx", QString::number(cameras[i]->getTSAI1Param().dp.x));
			config.setValue("tsai1_dpy", QString::number(cameras[i]->getTSAI1Param().dp.y));
			config.setValue("tsai2_trans_x", QString::number(cameras[i]->getTSAI2Param().trans.x));
			config.setValue("tsai2_trans_y", QString::number(cameras[i]->getTSAI2Param().trans.y));
			config.setValue("tsai2_trans_z", QString::number(cameras[i]->getTSAI2Param().trans.z));
			config.setValue("tsai2_rot_x", QString::number(RAD_TO_DEG*cameras[i]->getTSAI2Param().rot.x));
			config.setValue("tsai2_rot_y", QString::number(RAD_TO_DEG*cameras[i]->getTSAI2Param().rot.y));
			config.setValue("tsai2_rot_z", QString::number(RAD_TO_DEG*cameras[i]->getTSAI2Param().rot.z));
			config.setValue("tsai2_focal1", QString::number(cameras[i]->getTSAI2Param().focal1));
			config.setValue("tsai2_focal2", QString::number(cameras[i]->getTSAI2Param().focal2));
			config.setValue("tsai2_k1", QString::number(cameras[i]->getTSAI2Param().K1));
			config.setValue("tsai2_k2", QString::number(cameras[i]->getTSAI2Param().K2));
			config.setValue("tsai2_p1", QString::number(cameras[i]->getTSAI2Param().P1));
			config.setValue("tsai2_p2", QString::number(cameras[i]->getTSAI2Param().P2));
			config.setValue("tsai2_cx", QString::number(cameras[i]->getTSAI2Param().c.x));
			config.setValue("tsai2_cy", QString::number(cameras[i]->getTSAI2Param().c.y));

			config.endGroup();
		}
		else {
			config.beginGroup("Camera" + QString::number(i + 1));
			config.setValue("type", cameras[i]->getType());
			config.setValue("width", QString::number(cameras[i]->getWidth()));
			config.setValue("height", QString::number(cameras[i]->getHeight()));
			config.setValue("position_x", QString::number(cameras[i]->getEye().x()));
			config.setValue("position_y", QString::number(cameras[i]->getEye().y()));
			config.setValue("position_z", QString::number(cameras[i]->getEye().z()));
			//config.setValue("use_quat", QString::number(0));
			config.setValue("use_quat", int(useQuat[i]));
			config.setValue("quat_x", QString::number(cameras[i]->getOrientation().x()));
			config.setValue("quat_y", QString::number(cameras[i]->getOrientation().y()));
			config.setValue("quat_z", QString::number(cameras[i]->getOrientation().z()));
			config.setValue("quat_w", QString::number(cameras[i]->getOrientation().w()));
			config.setValue("rot_x", QString::number(RAD_TO_DEG*cameras[i]->getAlpha()));
			config.setValue("rot_y", QString::number(RAD_TO_DEG*cameras[i]->getBeta()));
			config.setValue("rot_z", QString::number(RAD_TO_DEG*cameras[i]->getGama()));
			config.setValue("fovy", QString::number(cameras[i]->getFovy()));
			config.setValue("dis_k1", QString::number(cameras[i]->getDisortionParams().K1));
			config.setValue("dis_k2", QString::number(cameras[i]->getDisortionParams().K2));
			config.setValue("dis_p1", QString::number(cameras[i]->getDisortionParams().P1));
			config.setValue("dis_p2", QString::number(cameras[i]->getDisortionParams().P2));
			config.setValue("tsai1_trans_x", QString::number(cameras[i]->getTSAI1Param().trans.x));
			config.setValue("tsai1_trans_y", QString::number(cameras[i]->getTSAI1Param().trans.y));
			config.setValue("tsai1_trans_z", QString::number(cameras[i]->getTSAI1Param().trans.z));
			config.setValue("tsai1_rot_x", QString::number(RAD_TO_DEG*cameras[i]->getTSAI1Param().rot.x));
			config.setValue("tsai1_rot_y", QString::number(RAD_TO_DEG*cameras[i]->getTSAI1Param().rot.y));
			config.setValue("tsai1_rot_z", QString::number(RAD_TO_DEG*cameras[i]->getTSAI1Param().rot.z));
			config.setValue("tsai1_focal", QString::number(cameras[i]->getTSAI1Param().focal));
			config.setValue("tsai1_kappa1", QString::number(cameras[i]->getTSAI1Param().kappa1));
			config.setValue("tsai1_cx", QString::number(cameras[i]->getTSAI1Param().c.x));
			config.setValue("tsai1_cy", QString::number(cameras[i]->getTSAI1Param().c.y));
			config.setValue("tsai1_sx", QString::number(cameras[i]->getTSAI1Param().sx));
			config.setValue("tsai1_ncx", QString::number(cameras[i]->getTSAI1Param().ncx));
			config.setValue("tsai1_nfx", QString::number(cameras[i]->getTSAI1Param().nfx));
			config.setValue("tsai1_dx", QString::number(cameras[i]->getTSAI1Param().d.x));
			config.setValue("tsai1_dy", QString::number(cameras[i]->getTSAI1Param().d.y));
			config.setValue("tsai1_dpx", QString::number(cameras[i]->getTSAI1Param().dp.x));
			config.setValue("tsai1_dpy", QString::number(cameras[i]->getTSAI1Param().dp.y));
			config.setValue("tsai2_trans_x", QString::number(cameras[i]->getTSAI2Param().trans.x));
			config.setValue("tsai2_trans_y", QString::number(cameras[i]->getTSAI2Param().trans.y));
			config.setValue("tsai2_trans_z", QString::number(cameras[i]->getTSAI2Param().trans.z));
			config.setValue("tsai2_rot_x", QString::number(RAD_TO_DEG*cameras[i]->getTSAI2Param().rot.x));
			config.setValue("tsai2_rot_y", QString::number(RAD_TO_DEG*cameras[i]->getTSAI2Param().rot.y));
			config.setValue("tsai2_rot_z", QString::number(RAD_TO_DEG*cameras[i]->getTSAI2Param().rot.z));
			config.setValue("tsai2_focal1", QString::number(cameras[i]->getTSAI2Param().focal1));
			config.setValue("tsai2_focal2", QString::number(cameras[i]->getTSAI2Param().focal2));
			config.setValue("tsai2_k1", QString::number(cameras[i]->getTSAI2Param().K1));
			config.setValue("tsai2_k2", QString::number(cameras[i]->getTSAI2Param().K2));
			config.setValue("tsai2_p1", QString::number(cameras[i]->getTSAI2Param().P1));
			config.setValue("tsai2_p2", QString::number(cameras[i]->getTSAI2Param().P2));
			config.setValue("tsai2_cx", QString::number(cameras[i]->getTSAI2Param().c.x));
			config.setValue("tsai2_cy", QString::number(cameras[i]->getTSAI2Param().c.y));

			config.endGroup();
		}
	}

}

void MainWindow::loadCameras(QString fileName) {
	QSettings config(fileName, QSettings::IniFormat);
	config.beginGroup("Cameras");
	float number = config.value("number", 0).toFloat();
	config.endGroup();
	if (number == 0) QMessageBox::critical(this, "Error", QString("Wrong cameras number"), QMessageBox::Ok);
	qDeleteAll(cameras);
	cameras.clear();
	useQuat.clear();
	//cameras = vector<pf::Camera*>(number);

	for (unsigned int i = 0; i < number; ++i)
	{
		config.beginGroup("Camera" + QString::number(i + 1));
		pf::Camera::Type type = (pf::Camera::Type)config.value("type", "0").toInt();
		int width = config.value("width", "0").toInt();
		int height = config.value("height", "0").toInt();
		float pos_x = config.value("position_x", "0").toFloat();
		float pos_y = config.value("position_y", "0").toFloat();
		float pos_z = config.value("position_z", "0").toFloat();
		bool useQuat = config.value("use_quat", "0").toBool();
		float quat_x = config.value("quat_x", "0").toFloat();
		float quat_y = config.value("quat_y", "0").toFloat();
		float quat_z = config.value("quat_z", "0").toFloat();
		float quat_w = config.value("quat_w", "0").toFloat();
		float rot_x = DEG_TO_RAD*config.value("rot_x", "0").toFloat();
		float rot_y = DEG_TO_RAD*config.value("rot_y", "0").toFloat();
		float rot_z = DEG_TO_RAD*config.value("rot_z", "0").toFloat();

		switch (type)
		{
		case pf::Camera::Perspective:
		{
			float fovy = config.value("fovy", "0").toFloat();

			pf::Camera *camTmp;
			if (useQuat) camTmp = new pf::Camera(pf::Vec3f(pos_x, pos_y, pos_z), pf::Quat(quat_x, quat_y, quat_z, quat_w), fovy, width, height);
			else camTmp = new pf::Camera(pf::Vec3f(pos_x, pos_y, pos_z), rot_x, rot_y, rot_z, fovy, width, height);
			cameras.push_back(camTmp);
			this->useQuat.push_back(useQuat);
			break;
		}
		case pf::Camera::TSAI1:
		{
			pf::TSAI1Camera params;
			params.trans.x = config.value("tsai1_trans_x", "0").toFloat();
			params.trans.y = config.value("tsai1_trans_y", "0").toFloat();
			params.trans.z = config.value("tsai1_trans_z", "0").toFloat();
			params.rot.x = DEG_TO_RAD*config.value("tsai1_rot_x", "0").toFloat();
			params.rot.y = DEG_TO_RAD*config.value("tsai1_rot_y", "0").toFloat();
			params.rot.z = DEG_TO_RAD*config.value("tsai1_rot_z", "0").toFloat();
			params.focal = config.value("tsai1_focal", "0").toFloat();
			params.kappa1 = config.value("tsai1_kappa1", "0").toFloat();
			params.c.x = config.value("tsai1_cx", "0").toFloat();
			params.c.y = config.value("tsai1_cy", "0").toFloat();
			params.sx = config.value("tsai1_sx", "0").toFloat();
			params.ncx = config.value("tsai1_ncx", "0").toFloat();
			params.nfx = config.value("tsai1_nfx", "0").toFloat();
			params.d.x = config.value("tsai1_dx", "0").toFloat();
			params.d.y = config.value("tsai1_dy", "0").toFloat();
			params.dp.x = config.value("tsai1_dpx", "0").toFloat();
			params.dp.y = config.value("tsai1_dpy", "0").toFloat();

			pf::Camera *camTmp;
			camTmp = new pf::Camera(params, width, height);
			cameras.push_back(camTmp);
			this->useQuat.push_back(useQuat);
			break;
		}
		case pf::Camera::TSAI2:
		case pf::Camera::TSAI2Rodrigues:
		{
			pf::TSAI2Camera params;
			params.trans.x = config.value("tsai2_trans_x", "0").toFloat();
			params.trans.y = config.value("tsai2_trans_y", "0").toFloat();
			params.trans.z = config.value("tsai2_trans_z", "0").toFloat();
			params.rot.x = DEG_TO_RAD*config.value("tsai2_rot_x", "0").toFloat();
			params.rot.y = DEG_TO_RAD*config.value("tsai2_rot_y", "0").toFloat();
			params.rot.z = DEG_TO_RAD*config.value("tsai2_rot_z", "0").toFloat();
			params.focal1 = config.value("tsai2_focal1", "0").toFloat();
			params.focal2 = config.value("tsai2_focal2", "0").toFloat();
			params.c.x = config.value("tsai2_cx", "0").toFloat();
			params.c.y = config.value("tsai2_cy", "0").toFloat();
			params.K1 = config.value("tsai2_k1", "0").toFloat();
			params.K2 = config.value("tsai2_k2", "0").toFloat();
			params.P1 = config.value("tsai2_p1", "0").toFloat();
			params.P2 = config.value("tsai2_p2", "0").toFloat();

			pf::Camera *camTmp;
			camTmp = new pf::Camera(params, width, height, type == pf::Camera::TSAI2Rodrigues);
			cameras.push_back(camTmp);
			this->useQuat.push_back(useQuat);
			break;
		}
		case pf::Camera::PerspectiveDisortion:
		{
			float fovy = config.value("fovy", "0").toFloat();
			pf::disortions dis;
			dis.K1 = config.value("dis_k1", "0").toFloat();
			dis.K2 = config.value("dis_k2", "0").toFloat();
			dis.P1 = config.value("dis_p1", "0").toFloat();
			dis.P2 = config.value("dis_p2", "0").toFloat();

			pf::Camera *camTmp;

			if (useQuat) camTmp = new  pf::Camera(pf::Vec3f(pos_x, pos_y, pos_z), pf::Quat(quat_x, quat_y, quat_z, quat_w), fovy, width, height, dis);
			else camTmp = new pf::Camera(pf::Vec3f(pos_x, pos_y, pos_z), rot_x, rot_y, rot_z, fovy, width, height, dis);
			cameras.push_back(camTmp);
			this->useQuat.push_back(useQuat);
			break;
		}
		}

		config.endGroup();
	}
	/*aktywacja buttona do wyboru kamery*/
	if (cameras.size() > 0) {
		setCamerasMenu();
		for (int i = 0; i < glWidgetsVector.size(); i++)
			selectCameraMenuVector[i]->setDisabled(false);
	}

	/*wyzeruj uzywanie kamery dla kazdego okna*/
	for (int j = 0; j < glWidgetsVector.size(); j++) {
		glWidgetsVector[j]->cast = false;
		/*odznacz wszystkie kamery*/
		if (cameras.size() > 0) {
			for (int i = 0; i < selectCameraActionVector[j].size(); i++) {
				selectCameraActionVector[j][i]->setChecked(false);
			}
		}
	}

	/*dopasuj id kamery do id glwidget*/
	for (int i = 0; i < cameras.size(); i++) {

		if (i < glWidgetsVector.size()) {
			glWidgetsVector[i]->setGLWidgetCamera(cameras[i]);
			glWidgetsVector[i]->cast = true;
			glWidgetsVector[i]->cameraID = i;

			/*zaznacz wybrana kamere*/
			selectCameraActionVector[i][i]->setChecked(true);

			if (bonesConf.size() != 0) {
				setGLWidgetDrawing(i);

			}
		}
	}
	
}

void MainWindow::updateSliders() {
	//numer wybranego okna
	ModelHandler modelHandler;
	int frameNumber = comboBoxFrameSelector->currentIndex();
	int backgroundID = imagesListIterator[frameNumber];

	for (int i = 0; i < glWidgetsVector.size(); i++) {
		/*jesli checkbox slidera zaznaczony*/
		if (sliderCheckBoxesVector[i]->isChecked()) {
			/*gdy wartosc glownego slidera przekroczy max okna*/
			if (globalSlider->value() > slidersVector[i]->maximum())
				slidersVector[i]->setValue(slidersVector[i]->maximum());
			/*w przeciwnym przypadku*/
			else
				slidersVector[i]->setValue(globalSlider->value());

			imagesListIterator[i] = slidersVector[i]->value();
			if (glWidgetsVector[i]->isAvi) {
				setTextIteratorLabel(countersVector[i], imagesListIterator[i], glWidgetsVector[i]->aviFrames.size());

				glWidgetsVector[i]->aviFrames[imagesListIterator[i] - 1].copyTo(glWidgetsVector[i]->aviFrame);
			}
			else {
				setTextIteratorLabel(countersVector[i], imagesListIterator[i], glWidgetsVector[i]->list.size());

				QString imgString = glWidgetsVector[i]->list[imagesListIterator[i] - 1].absoluteFilePath();
				glWidgetsVector[i]->imgPath = imgString.toUtf8().constData();
			}

			setGLWidgetDrawing(i);
		}

		/*jesli zaakceptowano konfiguracje modelu ustaw kolor numeru okna*/
		if (saveModelState[imagesListIterator[i] - 1] == true)
			frameIDVector[i]->setStyleSheet("color: LawnGreen");
		else
			frameIDVector[i]->setStyleSheet("color: red");
	
	}

}

void MainWindow::playBackground(int i) {
	/*jesli wcisnieto play*/
	if (playPressedVector[i] == true) {
		/*zwieksz iterator*/
		increaseButtonsVector[i]->click();
		if (glWidgetsVector[i]->isAvi) {
			if (imagesListIterator[i] >= glWidgetsVector[i]->aviFrames.size()) {
				/*pauza gdy max*/
				playPauseButtonsVector[i]->click();
			}
		}
		else {
			if (imagesListIterator[i] >= glWidgetsVector[i]->list.size()) {
				/*pauza gdy max*/
				playPauseButtonsVector[i]->click();
			}
		}
	}
}

void MainWindow::saveModelStateToBoolVector() {
	/*po zaakceptowaniu konfiguracji modelu zaaktualizuj kolor numeru okna*/
	int id = comboBoxFrameSelector->currentIndex();
	saveModelState[imagesListIterator[id] - 1] = true;

	frameIDVector[id]->setStyleSheet("color: LawnGreen");
	for (int i = 0; i < glWidgetsVector.size(); i++) {
		if (id != i) {
			if(imagesListIterator[id] - 1 == imagesListIterator[i] - 1)
				frameIDVector[i]->setStyleSheet("color: LawnGreen");
		}
	}
}

void MainWindow::setCamerasMenu() {
	/*wyczysc menu kamer*/
	if (selectCameraActionVector.size() > 0) {
		for (int i = 0; i < glWidgetsVector.size(); i++) {
			for (int j = 0; j < selectCameraActionVector[i].size(); j++) {
				selectCameraMenuVector[i]->removeAction(selectCameraActionVector[i][j]);
			}
		}
	}

	for (int i = 0; i < selectCameraMapperVector.size(); i++) {
		qDeleteAll(selectCameraMapperVector[i]);
	}
	selectCameraMapperVector.clear();
	selectCameraActionVector.clear();
	

	for (int i = 0; i < glWidgetsVector.size(); i++) {
		QVector<QAction*> tmp;
		/*dodaj kamery do menu*/
		for (int j = 0; j < cameras.size(); j++) {
			tmp.push_back(new QAction("Camera " + QString::number(j)));
			tmp[j]->setCheckable(true);
		}

		selectCameraActionVector.push_back(tmp);

		/*dla kazdej akcji z menu dodaj mapper*/
		QVector<QSignalMapper*> tmpMapperVector;
		for (int j = 0; j < selectCameraActionVector[i].size(); j++) {
			selectCameraMenuVector[i]->addAction(selectCameraActionVector[i][j]);
			tmpMapperVector.push_back(new QSignalMapper(this));
		}

		selectCameraMapperVector.push_back(tmpMapperVector);

		for (int j = 0; j < selectCameraMapperVector[i].size(); j++) {
			QString str = QString::number(i) + ";" + QString::number(j);
			QObject::connect(selectCameraMapperVector[i][j], SIGNAL(mapped(QString)), this, SLOT(selectCamera(QString)));
			selectCameraMapperVector[i][j]->setMapping(selectCameraActionVector[i][j], str);
			connect(selectCameraActionVector[i][j], SIGNAL(triggered()), selectCameraMapperVector[i][j], SLOT(map()));
		}

		/*jesli wczesniej okno bylo polaczone z wybrana kamera - dodaj ponownie*/
		if (glWidgetsVector[i]->cast) {
			QString str = QString::number(i) + ";" + QString::number(glWidgetsVector[i]->cameraID);
			selectCamera(str);
		}
	}
	
}

void MainWindow::addCameraMenuToFrame() {
	if (selectCameraActionVector.size() > 0) {
		for (int i = 0; i < glWidgetsVector.size()-1; i++) {
			for (int j = 0; j < selectCameraActionVector[i].size(); j++) {
				selectCameraMenuVector[i]->removeAction(selectCameraActionVector[i][j]);
			}
		}
	}

	for (int i = 0; i < selectCameraMapperVector.size(); i++) {
		qDeleteAll(selectCameraMapperVector[i]);
	}
	selectCameraMapperVector.clear();
	selectCameraActionVector.clear();


	for (int i = 0; i < glWidgetsVector.size(); i++) {
		QVector<QAction*> tmp;
		for (int j = 0; j < cameras.size(); j++) {
			tmp.push_back(new QAction("Camera " + QString::number(j)));
			tmp[j]->setCheckable(true);
		}

		selectCameraActionVector.push_back(tmp);

		QVector<QSignalMapper*> tmpMapperVector;
		for (int j = 0; j < selectCameraActionVector[i].size(); j++) {
			selectCameraMenuVector[i]->addAction(selectCameraActionVector[i][j]);
			tmpMapperVector.push_back(new QSignalMapper(this));
		}

		selectCameraMapperVector.push_back(tmpMapperVector);

		for (int j = 0; j < selectCameraMapperVector[i].size(); j++) {
			QString str = QString::number(i) + ";" + QString::number(j);
			QObject::connect(selectCameraMapperVector[i][j], SIGNAL(mapped(QString)), this, SLOT(selectCamera(QString)));
			selectCameraMapperVector[i][j]->setMapping(selectCameraActionVector[i][j], str);
			connect(selectCameraActionVector[i][j], SIGNAL(triggered()), selectCameraMapperVector[i][j], SLOT(map()));
		}
	}
}

void MainWindow::selectCamera(QString str) {

	QStringList values = str.split(";");
	if (values.count() == 2) {
		/*numer okna*/
		int	glWidgetID = values.at(0).toInt();
		/*numer kamery*/
		int	cameraID = values.at(1).toInt();
		
		glWidgetsVector[glWidgetID]->setGLWidgetCamera(cameras[cameraID]);
		glWidgetsVector[glWidgetID]->cast = true;
		glWidgetsVector[glWidgetID]->cameraID = cameraID;

		/*odznacz wszystkie kamery*/
		for (int i = 0; i < selectCameraActionVector[glWidgetID].size(); i++) {
			selectCameraActionVector[glWidgetID][i]->setChecked(false);
		}
		/*zaznacz wybrana kamere*/
		selectCameraActionVector[glWidgetID][cameraID]->setChecked(true);

		glWidgetsVector[glWidgetID]->update();
	}
	else {
		cerr << "Camera cast error" << endl;
	}
}

void MainWindow::setGoldenRatio() {
	ModelHandler modelHandler;
	modelHandler.saveBonesLengthToMap(bonesLength, bonesGeometry);

	float height = goldenRatioSpinBox->value();
	modelHandler.setGoldenRatio(bonesLength, height);

	modelHandler.updateBonesLengthFromMap(bonesLength, bonesGeometry);
	bool containsSpine1 = false;
	bool containsSpine1_dum1 = false;

	/*sprawdzenie czy model zawiera spine1 oraz spine1_dum1 - potrzebne do rysowania modelu*/
	for (int i = 0; i < bonesGeometry.size(); i++) {
		if (bonesGeometry[i].name == "Spine1")
			containsSpine1 = true;

		if (bonesGeometry[i].name == "Spine1_dum1")
			containsSpine1_dum1 = true;
	}

	if (containsSpine1 && containsSpine1_dum1) {
			float spine1Length, spine1_dum1Length;
			spine1Length = bonesLength.find("Spine1_dum1")->second;
			spine1_dum1Length = bonesLength.find("Spine1")->second;

			bonesLength.at("Spine1_dum1") = spine1_dum1Length;
			bonesLength.at("Spine1") = spine1Length;
			modelHandler.updateBonesLengthFromMap(bonesLength, bonesGeometry);
	}

	modelHandler.updateBonesLengthFromMap(bonesLength, bonesGeometry);

	refreshLengthSideMenu();

	setGoldenRatioCheckBox->setChecked(true);
}

void MainWindow::setGoldenRatioFromSpinBox() {
	if (setGoldenRatioCheckBox->isChecked()) {
		ModelHandler modelHandler;

		modelHandler.saveBonesLengthToMap(bonesLength, bonesGeometry);
		//float height = modelHandler.getModelHeightFromMap(bonesLength);
		float height = goldenRatioSpinBox->value();
		modelHandler.setGoldenRatio(bonesLength, height);

		modelHandler.updateBonesLengthFromMap(bonesLength, bonesGeometry);
		bool containsSpine1 = false;
		bool containsSpine1_dum1 = false;

		/*sprawdzenie czy model zawiera spine1 oraz spine1_dum1 - potrzebne do rysowania modelu*/
		for (int i = 0; i < bonesGeometry.size(); i++) {
			if (bonesGeometry[i].name == "Spine1")
				containsSpine1 = true;

			if (bonesGeometry[i].name == "Spine1_dum1")
				containsSpine1_dum1 = true;
		}

		if (containsSpine1 && containsSpine1_dum1) {
			float spine1Length, spine1_dum1Length;
			spine1Length = bonesLength.find("Spine1_dum1")->second;
			spine1_dum1Length = bonesLength.find("Spine1")->second;

			bonesLength.at("Spine1_dum1") = spine1_dum1Length;
			bonesLength.at("Spine1") = spine1Length;

			modelHandler.updateBonesLengthFromMap(bonesLength, bonesGeometry);
		}

		modelHandler.updateBonesLengthFromMap(bonesLength, bonesGeometry);

		refreshLengthSideMenu();
		setGoldenRatioCheckBox->setChecked(true);
	}
}

void MainWindow::updateModelStateVectorSize(int frames) {
	/*jesli wczesniej wczytano jakies tlo - usun ustawione modelState*/
	if (modelState.size() > 1) {
		modelState.erase(modelState.begin() + 1, modelState.end());
		bonesRotations.erase(bonesRotations.begin() + 1, bonesRotations.end());
		modelTranslation.erase(modelTranslation.begin() + 1, modelTranslation.end());
		saveModelState.erase(saveModelState.begin() + 1, saveModelState.end());
	}

	for (int i = 0; i < frames - 1; i++) {
		modelState.push_back(modelState[0]);
		bonesRotations.push_back(bonesRotations[0]);
		modelTranslation.push_back(modelTranslation[0]);
		saveModelState.push_back(false);
	}
}

void MainWindow::updateGLWidgetDrawning(int i) {
	mainModel->setModelState(modelState[imagesListIterator[i] - 1]);
	glWidgetsVector[i]->setGLWidgetModelState(imagesListIterator[i] - 1, modelState);
	glWidgetsVector[i]->setRadiusVertices(mainModel->getRotatedVertices(), mainModel->getRadiusVec());
	glWidgetsVector[i]->update();
}

void MainWindow::setGLWidgetDrawningToFirstModelConfigurtion(int i) {
	mainModel->setModelState(modelState[0]);
	glWidgetsVector[i]->setGLWidgetModelState(0, modelState);
	glWidgetsVector[i]->setRadiusVertices(mainModel->getRotatedVertices(), mainModel->getRadiusVec());
	glWidgetsVector[i]->update();
}

void MainWindow::setIteratorToFrames() {
	int frameID = comboBoxFrameSelector->currentIndex();
	/*ilosc klatek tla*/
	int widgetSize;

	if (glWidgetsVector[frameID]->isAvi)
		widgetSize = glWidgetsVector[frameID]->aviFrames.size();
	else
		widgetSize = glWidgetsVector[frameID]->list.size();

	for (int i = 0; i < glWidgetsVector.size(); i++) {
		if (frameID != i) {
			if (!glWidgetsVector[i]->aviFrames.isEmpty() || !glWidgetsVector[i]->list.isEmpty()) { 

				if (glWidgetsVector[i]->isAvi) {
					/*czy ilosc wczytanych klatek okna jest mniejsza od aktualnego indeksu*/
					if (imagesListIterator[frameID] <= glWidgetsVector[i]->aviFrames.size()) {
						imagesListIterator[i] = imagesListIterator[frameID];
						slidersVector[i]->setValue(imagesListIterator[i]);

						setTextIteratorLabel(countersVector[i], imagesListIterator[i], glWidgetsVector[i]->aviFrames.size());

						glWidgetsVector[i]->aviFrames[imagesListIterator[i] - 1].copyTo(glWidgetsVector[i]->aviFrame);
					}
				}
				else {
					if (imagesListIterator[frameID] <= glWidgetsVector[i]->list.size()) {
						imagesListIterator[i] = imagesListIterator[frameID];
						slidersVector[i]->setValue(imagesListIterator[i]);

						setTextIteratorLabel(countersVector[i], imagesListIterator[i], glWidgetsVector[i]->list.size());

						QString imgString = glWidgetsVector[i]->list[imagesListIterator[i] - 1].absoluteFilePath();
						glWidgetsVector[i]->imgPath = imgString.toUtf8().constData();
					}
				}
				updateGLWidgetDrawning(i);

				if (saveModelState[imagesListIterator[i] - 1] == true)
					frameIDVector[i]->setStyleSheet("color: LawnGreen");
				else
					frameIDVector[i]->setStyleSheet("color: red");
			}
		}
	}
}

void MainWindow::reduceGlobalSliderPressed() {
	int value = globalSlider->value();
	globalSlider->setValue(value - 1);
	refreshRotateSideMenu();
}

void MainWindow::increaseGlobalSliderPressed() {
	int value = globalSlider->value();
	globalSlider->setValue(value + 1);
	refreshRotateSideMenu();
}

void MainWindow::dropEvent(QDropEvent *e){
	if (e->mimeData()->hasUrls()) {

		if (e->mimeData()->urls().size() > 1) {
			cout << "to many files loaded" << endl;
		}
		else {
			QUrl url = e->mimeData()->urls()[0];
			QString fileName = url.toLocalFile();

			QString suffix = QFileInfo(fileName).completeSuffix();
			if (!suffix.isEmpty()) {
				/*obrazy*/
				if (suffix == "png" || suffix == "jpg" || suffix == "jpeg" || suffix == "bmp") {
					/*wybierz model*/
					SelectModelID *modelID = new SelectModelID(this);
					modelID->resize(100, 100);
					modelID->setComboBox(glWidgetsVector.size());
					modelID->exec();
					int id = 0;
					if (modelID->selected) {
						id = modelID->getID();
						glWidgetsVector[id]->loadFiles(fileName);
						loadImagesBackgroundDropped(id);
					}

					delete modelID;
				}
				/*avi*/
				else if (suffix == "avi") {
					/*wybierz model*/
					SelectModelID *modelID = new SelectModelID(this);
					modelID->resize(100, 100);
					modelID->setComboBox(glWidgetsVector.size());
					modelID->exec();
					int id = 0;
					if (modelID->selected) {
						id = modelID->getID();
						glWidgetsVector[id]->loadAviFile(fileName);
						loadAviDropped(id);
					}
				}
				else if (suffix == "dat") {
					loadDatDropped(fileName);
				}
				else if (suffix == "amc") {
					loadFromDroppedAMC(fileName);
				}
				else if (suffix == "asf") {
					loadFromDroppedASF(fileName);
				}
				else if (suffix == "ini") {
					loadCamerasFromDroppedINI(fileName);
				}
			}
			else {
				cout << "no suffix" << endl;
			}
		}

	}
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e){
//	cout << "entered Main Window" << endl;
	if (e->mimeData()->hasUrls()) {
		e->acceptProposedAction();
	}
	
} 

void MainWindow::dragLeaveEvent(QDragLeaveEvent *e) {
	//cout << "left Main Window" << endl;
}

bool MainWindow::checkIteratorRange(int i) {
	int id = imagesListIterator[i];
	int maxRange = saveModelState.size();
	if (imagesListIterator[i] <= maxRange)
		return true;
	else
		return false;
}

void MainWindow::saveProject(QString fileName, QString asfFilePath, QString amcFilePath, QString datFilePath, QString camerasFilePath) {
	saveAMCFromQStringToFile(amcFilePath);
	saveDatFromQStringToFile(datFilePath);
	saveAsfFromQStringToFile(asfFilePath);
	saveCameras(camerasFilePath);

	QSettings config(fileName, QSettings::IniFormat);
	config.clear();
	config.beginGroup("AppConfig");
	config.setValue("glWidgetsNumber", this->glWidgetsVector.size());
	config.setValue("asfFilePath", asfFilePath);
	config.setValue("amcFilePath", amcFilePath);
	config.setValue("datFilePath", datFilePath);
	config.setValue("camerasFilePath", camerasFilePath);
	config.setValue("resizeBackground", QString::number(this->resizeBackground));
	config.setValue("backgroundWidth", QString::number(this->backgroundWidth));
	config.setValue("backgroundHeight", QString::number(this->backgroundHeight));
	config.setValue("timerValue", QString::number(this->timerValue));
	config.setValue("justLaunched", QString::number(this->justLaunched));

	config.endGroup();

	for (int i = 0; i < glWidgetsVector.size(); i++) {
		config.beginGroup("Frame" + QString::number(i));

		
		config.setValue("mode", glWidgetsVector[i]->mode);
		config.setValue("iterator", imagesListIterator[i]);
		config.setValue("sliderCheckBoxChecked", QString::number(sliderCheckBoxesVector[i]->isChecked()));
		config.setValue("drawJoints", QString::number(glWidgetsVector[i]->drawJoints));
		config.setValue("scaleFrame", QString::number(glWidgetsVector[i]->scaleFrame));
		config.setValue("frameWidth", QString::number(glWidgetsVector[i]->frameWidth));
		config.setValue("frameHeight", QString::number(glWidgetsVector[i]->frameHeight));
		config.setValue("isAvi", QString::number(glWidgetsVector[i]->isAvi));
		if (!glWidgetsVector[i]->list.isEmpty()) {
			try {
				config.setValue("loadedImagesPath", glWidgetsVector[i]->list[0].absoluteFilePath());
				throw MyException("Brak pliku tla.");
			}
			catch (MyException& e) {
				QMessageBox::warning(this, "Warning", "Missing background file.", QMessageBox::Ok);
				exit(-1);
			}
		}
		else 
			config.setValue("loadedImagesPath", "");

		config.setValue("aviFilePath", glWidgetsVector[i]->aviFilePath);
		config.setValue("isInMainWindow", QString::number(glWidgetsVector[i]->isInMainWindow));
		config.setValue("aviMode", QString::number(glWidgetsVector[i]->aviMode));
		config.setValue("aviHz", QString::number(glWidgetsVector[i]->aviHz));
		config.setValue("aviLower", QString::number(glWidgetsVector[i]->aviLower));
		config.setValue("aviHigher", QString::number(glWidgetsVector[i]->aviHigher));

		config.setValue("cast", QString::number(glWidgetsVector[i]->cast));
		config.setValue("cameraID", QString::number(glWidgetsVector[i]->cameraID));

		config.endGroup();
	}
}

void MainWindow::loadProject(QString fileName) {

	/*wektory przechowujace zmienne glWidget*/
	vector<int> modeVector;
	vector<int> iteratorVector;
	vector<int> drawJointsVector;
	vector<bool> scaleFrameVector;
	vector<int> frameWidthVector;
	vector<int> frameHeightVector;
	vector<bool> isAviVector;
	vector<QString> loadedImagesFolderPathVector;
	vector<QString> aviFilePathVector;
	vector<bool> isInMainWindowVector;
	vector<int> aviModeVector;
	vector<int> aviLowerVector;
	vector<int> aviHigherVector;
	vector<int> aviHzVector;
	vector<bool> castVector;
	vector<int> cameraIDVector;
	vector<bool> sliderCheckBoxVector;

	QSettings config(fileName, QSettings::IniFormat);
	config.beginGroup("AppConfig");
	int number = config.value("glWidgetsNumber", 0).toInt();
	QString asfFilePath = config.value("asfFilePath").toString();
	QString amcFilePath = config.value("amcFilePath").toString();
	QString datFilePath = config.value("datFilePath").toString();
	QString camerasFilePath = config.value("camerasFilePath").toString();
	resizeBackground = config.value("resizeBackground", 0).toBool();
	backgroundWidth = config.value("backgroundWidth", 0).toInt();
	backgroundHeight = config.value("backgroundHeight", 0).toInt();
	timerValue = config.value("timerValue", 0).toInt();
	justLaunched = config.value("justLaunched", 0).toBool();
	config.endGroup();
	if (number == 0) QMessageBox::critical(this, "Error", QString("Wrong widgets number"), QMessageBox::Ok);

	for (unsigned int i = 0; i < number; ++i)	{
		config.beginGroup("Frame" + QString::number(i));
		int mode = config.value("mode", 0).toInt();
		int iterator = config.value("iterator", 0).toInt();
		bool sliderCheckBoxChecked = config.value("sliderCheckBoxChecked", 0).toBool();
		int drawJoints = config.value("drawJoints", 0).toInt();
		bool scaleFrame = config.value("scaleFrame", 0).toBool();
		int frameWidth = config.value("frameWidth", 0).toInt();
		int frameHeight = config.value("frameHeight", 0).toInt();
		bool isAvi = config.value("isAvi", 0).toBool();
		QString loadedImagesFolderPath = config.value("loadedImagesPath").toString();
		QString aviFilePath = config.value("aviFilePath", 0).toString();
		bool isInMainWindow = config.value("isInMainWindow", 0).toBool();
		int aviMode = config.value("aviMode", 0).toInt();
		int aviHz = config.value("aviHz", 0).toInt();
		int aviLower = config.value("aviLower", 0).toInt();
		int aviHigher = config.value("aviHigher", 0).toInt();
		bool cast = config.value("case", 0).toBool();
		int cameraID = config.value("cameraID", 0).toInt();
		config.endGroup();

		modeVector.push_back(mode);
		iteratorVector.push_back(iterator);
		sliderCheckBoxVector.push_back(sliderCheckBoxChecked);
		drawJointsVector.push_back(drawJoints);
		scaleFrameVector.push_back(scaleFrame);
		frameWidthVector.push_back(frameWidth);
		frameHeightVector.push_back(frameHeight);
		isAviVector.push_back(isAvi);
		loadedImagesFolderPathVector.push_back(loadedImagesFolderPath);
		aviFilePathVector.push_back(aviFilePath);
		isInMainWindowVector.push_back(isInMainWindow);
		aviModeVector.push_back(aviMode);
		aviHzVector.push_back(aviHz);
		aviLowerVector.push_back(aviLower);
		aviHigherVector.push_back(aviHigher);
		castVector.push_back(cast);
		cameraIDVector.push_back(cameraID);
	}

	deleteGridChildLayout();
	deleteSideMenus();
	for (int i = 0; i < number; i++) {
		addFrameButton->click();
		if (sliderCheckBoxVector[i] == true)
			sliderCheckBoxesVector[i]->setChecked(true);
	}

	loadFromDroppedASF(asfFilePath);
	loadDatDropped(datFilePath);
	loadFromDroppedAMC(amcFilePath);

	for (int j = 0; j < glWidgetsVector.size(); j++) {
		glWidgetsVector[j]->mode = modeVector[j];
		imagesListIterator[j] = iteratorVector[j];
		glWidgetsVector[j]->drawJoints = drawJointsVector[j];
		glWidgetsVector[j]->scaleFrame = scaleFrameVector[j];
		glWidgetsVector[j]->frameWidth = frameWidthVector[j];
		glWidgetsVector[j]->frameHeight = frameHeightVector[j];
		glWidgetsVector[j]->isAvi = isAviVector[j];

		if (glWidgetsVector[j]->isAvi) {
			glWidgetsVector[j]->loadAviFile(aviFilePathVector[j], aviModeVector[j], aviHzVector[j], aviLowerVector[j], aviHigherVector[j]);
			//glWidgetsVector[j]->aviFrames = aviFramesVector[j];

			glWidgetsVector[j]->aviFrames[imagesListIterator[j] - 1].copyTo(glWidgetsVector[j]->aviFrame);

			if (!glWidgetsVector[j]->aviFrames.isEmpty()) {
				if (globalSlider->maximum() < glWidgetsVector[j]->aviFrames.size())
					globalSlider->setMaximum(glWidgetsVector[j]->aviFrames.size());
			}

			setTextIteratorLabel(countersVector[j], imagesListIterator[j], glWidgetsVector[j]->aviFrames.size());
			glWidgetsVector[j]->drawBckg = true;
			prepareSlider(slidersVector[j], j);

			slidersVector[j]->setValue(imagesListIterator[j]);
		}
		else {
			glWidgetsVector[j]->loadFiles(loadedImagesFolderPathVector[j]);

			if (!glWidgetsVector[j]->list.isEmpty()) {
				if (globalSlider->maximum() < glWidgetsVector[j]->list.size())
					globalSlider->setMaximum(glWidgetsVector[j]->list.size());
			}

			setTextIteratorLabel(countersVector[j], imagesListIterator[j], glWidgetsVector[j]->list.size());
			if (!glWidgetsVector[j]->list.isEmpty()) {
				glWidgetsVector[j]->imgPath = glWidgetsVector[j]->list[imagesListIterator[j] - 1].absoluteFilePath().toUtf8().constData();
				glWidgetsVector[j]->drawBckg = true;
			}

			prepareSlider(slidersVector[j], j);

			slidersVector[j]->setValue(imagesListIterator[j]);
		}

		glWidgetsVector[j]->isInMainWindow = isInMainWindowVector[j];
		//glWidgetsVector[j]->drawBckg = drawBckgVector[j];
		if (glWidgetsVector[j]->drawBckg) {
			hMenuWidgetVector[j]->show();
		}

		//glWidgetsVector[j]->hz = hzVector[j];
		glWidgetsVector[j]->cast = castVector[j];
		glWidgetsVector[j]->cameraID = cameraIDVector[j];

		//reloadGLWidgetMenu();

	}
	loadCameras(camerasFilePath);
	if (cameras.size() > 0) {
		for (int i = 0; i < glWidgetsVector.size(); i++)
			selectCameraMenuVector[i]->setDisabled(false);
	}


}

void MainWindow::deleteGridChildLayout() {
	qDeleteAll(frameIDVector);
	qDeleteAll(glWidgetsVector);
	//qDeleteAll(glWidgetLayoutVector);
	qDeleteAll(glWidgetScrollAreaVector);
	//qDeleteAll(glWidgetsWidgetVector);
	qDeleteAll(bordersWidgetsVector);
	qDeleteAll(slidersVector);
	qDeleteAll(sliderCheckBoxesVector);
	qDeleteAll(minButtonsVector);
	qDeleteAll(maxButtonsVector);
	qDeleteAll(reduceButtonsVector);
	qDeleteAll(increaseButtonsVector);
	qDeleteAll(playPauseButtonsVector);
	qDeleteAll(countersVector);
	qDeleteAll(menuButtonsVector);
	qDeleteAll(menuButtonsLayoutVector);
	qDeleteAll(horizontalGLWidgetMenuVector);
	qDeleteAll(hMenuWidgetVector);
	qDeleteAll(backgroundTimersVector);

	frameIDVector.clear();
	glWidgetsVector.clear();
	glWidgetLayoutVector.clear();
	glWidgetScrollAreaVector.clear();
	glWidgetsWidgetVector.clear();
	bordersWidgetsVector.clear();
	slidersVector.clear();
	sliderCheckBoxesVector.clear();
	minButtonsVector.clear();
	maxButtonsVector.clear();
	reduceButtonsVector.clear();
	increaseButtonsVector.clear();
	playPauseButtonsVector.clear();
	countersVector.clear();
	imagesListIterator.clear();
	menuButtonsVector.clear();
	menuButtonsLayoutVector.clear();
	horizontalGLWidgetMenuVector.clear();
	hMenuWidgetVector.clear();
	backgroundTimersVector.clear();

	qDeleteAll(frameScrollingMenu);
	qDeleteAll(selectBackgroundMenuVector); 
	qDeleteAll(selectCameraMenuVector); 
	qDeleteAll(resizeGLWidgetVector);

	frameScrollingMenu.clear();
	selectBackgroundMenuVector.clear();
	selectCameraMenuVector.clear();
	resizeGLWidgetVector.clear();
	

	/*
	The sender parameter is the same object that you gave as the first parameter to setMapping()

	Additionally, it is worth noting what the documentation has to say under removeMappings():

	This is done automatically when mapped objects are destroyed.

	So if you're destroying objects that you've given to a SignalMapper, it will clean itself up when those objects are deleted.
	*/
	qDeleteAll(increaseButtonMapperVector);
	qDeleteAll(reduceButtonMapperVector);
	qDeleteAll(minButtonMapperVector);
	qDeleteAll(maxButtonMapperVector);
	qDeleteAll(playPauseButtonMapperVector);
	qDeleteAll(timerMapperVector);
	qDeleteAll(addImagesBackgroundMapperVector);
	qDeleteAll(resizeUpGLWidgetMapperVector);
	qDeleteAll(resizeDownGLWidgetMapperVector);
	for (int k = 0; k < deleteFrameMapperVector.size(); k++) {
		deleteFrameMapperVector[k]->deleteLater();
	}

	qDeleteAll(addAviMapperVector);
	qDeleteAll(sliderMapperVector);

	increaseButtonMapperVector.clear();
	reduceButtonMapperVector.clear();
	minButtonMapperVector.clear();
	maxButtonMapperVector.clear();
	playPauseButtonMapperVector.clear();
	timerMapperVector.clear();
	addImagesBackgroundMapperVector.clear();
	resizeUpGLWidgetMapperVector.clear();
	resizeDownGLWidgetMapperVector.clear();
	deleteFrameMapperVector.clear();
	addAviMapperVector.clear();
	sliderMapperVector.clear();

	addFrameButtonCounter = 0;
	globalSlider->setMaximum(1);
	globalSlider->setMinimum(1);
	while (comboBoxFrameSelector->count() > 0)
		comboBoxFrameSelector->removeItem(0);

	delete scrollWidget;
	scrollWidget = new QWidget();
	gridChildLayout = new QGridLayout();
	scrollWidget->setLayout(gridChildLayout);
	scrollArea->setWidget(scrollWidget);

}

void MainWindow::deleteSideMenus() {
	qDeleteAll(sideRotateMenuLineEditsVector);
	qDeleteAll(sideRotateMenuLabelsVector);
	qDeleteAll(sideRotateMenuButtonsVector);
	qDeleteAll(sideRotateMenuHLayoutsVector);
	qDeleteAll(sideRotateMenuSpinBoxesVector);

	qDeleteAll(sideLengthMenuLabelsVector);
	qDeleteAll(sideLengthMenuHLayoutsVector);
	qDeleteAll(sideLengthMenuButtonsVector);
	qDeleteAll(sideLengthMenuLineEditsVector);
	qDeleteAll(sideLengthMenuSpinBoxesVector);
	qDeleteAll(sideLengthMenuRotLabelsVector);
	qDeleteAll(sideLengthMenuRotCheckBoxesVector);
	qDeleteAll(sideLengthMenuLimVelLabelsVector);
	qDeleteAll(sideLengthMenuLimVelSpinBoxesVector);

	qDeleteAll(sideLengthMenuHLayoutsExtraVector);
	qDeleteAll(sideLengthMenuLabelsExtraVector);
	qDeleteAll(sideLengthMenuButtonsExtraVector);
	qDeleteAll(sideLengthMenuLineEditsExtraVector);
	qDeleteAll(sideLengthMenuSpinBoxesExtraVector);

	qDeleteAll(sideGeometryMenuLabelsVector);
	qDeleteAll(sideGeometryMenuHLayoutsVector);
	qDeleteAll(sideGeometryMenuButtonsVector);
	qDeleteAll(sideGeometryMenuLineEditsVector);
	qDeleteAll(sideGeometryMenuSpinBoxesVector);

	qDeleteAll(rotateMapperVector);
	qDeleteAll(rotateFromLineEditMapperVector);
	qDeleteAll(translateFromLineEditMapperVector);
	qDeleteAll(lengthMapperVector);
	qDeleteAll(lengthFromLineEditMapperVector);
	qDeleteAll(geometryMapperVector);
	qDeleteAll(geometryLengthFromLineEditMapperVector);
	qDeleteAll(geometryTopRadiusFromLineEditMapperVector);
	qDeleteAll(geometryBottomRadiusFromLineEditMapperVector);

	delete saveCurrentModelStateToVector;
	delete goldenRatioGroupBox;
	/*delete setGoldenRatioButton;
	delete setGoldenRatioCheckBox;
	delete goldenRatio1RowLayout;
	delete goldenRatio2RowLayout;
	delete goldenRatioLayout;
	delete goldenRatioHeightLabel;
	delete goldenRatioSpinBox;*/

	sideRotateMenuLineEditsVector.clear();
	sideRotateMenuLabelsVector.clear();
	sideRotateMenuButtonsVector.clear();
	sideRotateMenuHLayoutsVector.clear();
	sideRotateMenuSpinBoxesVector.clear();

	sideLengthMenuLabelsVector.clear();
	sideLengthMenuHLayoutsVector.clear();
	sideLengthMenuButtonsVector.clear();
	sideLengthMenuLineEditsVector.clear();
	sideLengthMenuSpinBoxesVector.clear();
	sideLengthMenuRotLabelsVector.clear();
	sideLengthMenuRotCheckBoxesVector.clear();
	sideLengthMenuLimVelLabelsVector.clear();
	sideLengthMenuLimVelSpinBoxesVector.clear();

	sideLengthMenuHLayoutsExtraVector.clear();
	sideLengthMenuLabelsExtraVector.clear();
	sideLengthMenuButtonsExtraVector.clear();
	sideLengthMenuLineEditsExtraVector.clear();
	sideLengthMenuSpinBoxesExtraVector.clear();

	sideGeometryMenuLabelsVector.clear();
	sideGeometryMenuHLayoutsVector.clear();
	sideGeometryMenuButtonsVector.clear();
	sideGeometryMenuLineEditsVector.clear();
	sideGeometryMenuSpinBoxesVector.clear();

	rotateMapperVector.clear();
	rotateFromLineEditMapperVector.clear();
	translateFromLineEditMapperVector.clear();
	lengthMapperVector.clear();
	lengthFromLineEditMapperVector.clear();
	geometryMapperVector.clear();
	geometryLengthFromLineEditMapperVector.clear();
	geometryTopRadiusFromLineEditMapperVector.clear();
	geometryBottomRadiusFromLineEditMapperVector.clear();

	addFrameButtonCounter = 0;
}

void MainWindow::reserveVectorsMemory() {
	glWidgetsVector.reserve(10);
	bordersWidgetsVector.reserve(10);
	frameIDVector.reserve(10);
	horizontalGLWidgetMenuVector.reserve(10);
	hMenuWidgetVector.reserve(10);
	minButtonsVector.reserve(10);
	reduceButtonsVector.reserve(10);
	maxButtonsVector.reserve(10);
	increaseButtonsVector.reserve(10);
	playPauseButtonsVector.reserve(10);
	playPressedVector.reserve(10);
	countersVector.reserve(10);
	slidersVector.reserve(10);
	sliderCheckBoxesVector.reserve(10);
	imagesListIterator.reserve(10);
	menuButtonsVector.reserve(10);
	menuButtonsLayoutVector.reserve(10);
	frameScrollingMenu.reserve(10);
	selectBackgroundMenuVector.reserve(10);
	selectCameraMenuVector.reserve(10);
	resizeGLWidgetVector.reserve(10);
	selectAviBackgroundVector.reserve(10);
	selectImagesBackgroundVector.reserve(10);
	deleteFrameMenuVector.reserve(10);
	selectCameraActionVector.reserve(10);
	backgroundTimersVector.reserve(10);
	resizeUpActionVector.reserve(10);
	resizeDownActionVector.reserve(10);

	increaseButtonMapperVector.reserve(10);
	reduceButtonMapperVector.reserve(10);
	minButtonMapperVector.reserve(10);
	maxButtonMapperVector.reserve(10);
	playPauseButtonMapperVector.reserve(10);
	timerMapperVector.reserve(10);
	addImagesBackgroundMapperVector.reserve(10);
	addAviMapperVector.reserve(10);
	deleteFrameMapperVector.reserve(10);
	sliderMapperVector.reserve(10);
	selectCameraMapperVector.reserve(10);
	resizeDownGLWidgetMapperVector.reserve(10);
	resizeUpGLWidgetMapperVector.reserve(10);

	rotateMapperVector.reserve(100);
	rotateFromLineEditMapperVector.reserve(100);
	lengthMapperVector.reserve(100);
	lengthFromLineEditMapperVector.reserve(100);
	geometryMapperVector.reserve(100);
	geometryLengthFromLineEditMapperVector.reserve(100);
	geometryTopRadiusFromLineEditMapperVector.reserve(100);
	geometryBottomRadiusFromLineEditMapperVector.reserve(100);
	translateFromLineEditMapperVector.reserve(100);
}

void MainWindow::setGLWidgetDrawing(int i){
	if (checkIteratorRange(i)) {
		updateGLWidgetDrawning(i);
		refreshRotateSideMenu();
	}
	else {
		setGLWidgetDrawningToFirstModelConfigurtion(i);
		int frameNumber = comboBoxFrameSelector->currentIndex();
		if (frameNumber == i)
			scrollRotateSideArea->setDisabled(true);
		frameIDVector[i]->setStyleSheet("color: red");

	}
}