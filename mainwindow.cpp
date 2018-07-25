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
    ui->setupUi(this);

	prepareLayouts();
	prepareMenus();
	prepareMainModel();

	addFrameButton->setVisible(true);
	addFrameButton->setText(QString("Add window"));

	globalSlider->setVisible(true);
	globalSlider->setOrientation(Qt::Horizontal);
	globalSlider->setMinimum(1);
	globalSlider->setMaximum(1);

	modelHandler.initializeBonesDOFMap(modelDOF, asfBones);
	modelHandler.initializeBonesRadiusMap(modelRadius, asfBones, bonesGeometry);
	modelHandler.initializeBonesLengthMap(bonesLength, asfBones);
	modelHandler.initializeBonesLimitsMap(modelLimits, asfBones);
	modelHandler.initializeBonesVelocityMap(modelVelocity, asfBones);

	for (int i = 0; i < 4; i++) {
		addFrameButton->click();
	}

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

	loadFromAmcAction = new QAction("Load .amc File", this);
	connect(loadFromAmcAction, &QAction::triggered, this, &MainWindow::loadFromAmc);
	loadMenu->addAction(loadFromAmcAction);

	loadMenu = plikMenu->addMenu(tr("&Save"));

	loadDatAction = new QAction("Load model config", this);
	connect(loadDatAction, &QAction::triggered, this, &MainWindow::loadDatFromFile);
	loadMenu->addAction(loadDatAction);

	/****/
	saveMenu = plikMenu->addMenu(tr("&Save"));

	saveDatAction = new QAction("Model config file", this);
	connect(saveDatAction, &QAction::triggered, this, &MainWindow::saveDatToFile);
	saveMenu->addAction(saveDatAction);

	saveMenu->addSeparator();

	saveAllAmcSequenceAction = new QAction("Save all amc sequences", this);
	connect(saveAllAmcSequenceAction, &QAction::triggered, this, &MainWindow::saveAllFramesSequenceToFile);
	saveMenu->addAction(saveAllAmcSequenceAction);

	saveAmcSequenceAction = new QAction("Save amc sequence", this);
	connect(saveAmcSequenceAction, &QAction::triggered, this, &MainWindow::saveOneFrameSequenceToFile);
	saveMenu->addAction(saveAmcSequenceAction);

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
	/*help*/
	helpAction = new QAction(tr("&Help"));
	menuBar->addAction(helpAction);
}

void MainWindow::prepareLayouts() {

	addFrameButton = new QPushButton;
	addFrameButton->setVisible(false);
	connect(addFrameButton, SIGNAL(released()), this, SLOT(buttonAddFramePressed()));

	globalSlider = new QSlider;
	globalSlider->setVisible(false);
	connect(globalSlider, SIGNAL(sliderReleased()), this, SLOT(updateSliders()));

	comboBoxFrameSelector = new QComboBox;
	connect(comboBoxFrameSelector, SIGNAL(activated(int)), this, SLOT(refreshRotateSideMenu()));

	gridLayout = new QGridLayout;
	vboxRotateLayout = new QVBoxLayout;
	vboxLengthLayout = new QVBoxLayout;
	vboxGeometryLayout = new QVBoxLayout;
	//sideGridLayout = new QGridLayout;
	gridChildLayout = new QGridLayout;
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

	connect(sideManuTabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabSelected()));

	gridLayout->addWidget(addFrameButton, 0, 0, 1, 1);
	gridLayout->addWidget(globalSlider, 0, 1, 1, 8);
	gridLayout->addLayout(selectFrameLayout, 0, 10, 1, 1);
	gridLayout->addWidget(scrollArea, 1, 0, 9, 9);
	gridLayout->addWidget(sideManuTabWidget, 1, 9, 9, 2);

	windowLayout->setLayout(gridLayout);

	setCentralWidget(windowLayout);
}

void MainWindow::prepareMainModel() {
	FileHandler * fileHandler;

	mainModel = new pf::Model3D(pf::Model3D::Cylinder, ASF_TEMPLATE_PATH, DAT_TEMPLATE_PATH);
	modelState = fileHandler->getAMCTemplate(mainModel);
	mainModel->setModelState(modelState[0]);
	limits = mainModel->getLimits();
	usedBones = mainModel->getNamesMovingBones();
	velocity = mainModel->getVelocity();
	allBones = mainModel->getNamesBones();
	pf::MotionData::loadASF(ASF_TEMPLATE_PATH, idxBonesMap, asfBones);
	mainModel->loadConfig(DAT_TEMPLATE_PATH, bonesConf, bonesGeometry);

	delete fileHandler;
}

void MainWindow::resetVariables() {
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
	qDeleteAll(lengthMapperVector);
	qDeleteAll(geometryMapperVector);

	delete saveCurrentModelStateToVector;
	delete setGoldenRatioButton;
	delete setGoldenRatioCheckBox;
	delete goldenRatioLayout;

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
	lengthMapperVector.clear();
	geometryMapperVector.clear();

	addFrameButtonCounter = 0;

	showLayouts();
}

void MainWindow::hideLayouts() {
	windowLayout->hide();
}

void MainWindow::showLayouts() {
	windowLayout->show();
}

void MainWindow::saveDatToFile() {
	FileHandler * fileHandler = new FileHandler();

	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save as .dat"), "",
		tr("DAT (*.dat);;All Files(*)"));

	if (!fileName.isEmpty()) {

		/*zapis pliku dat*/
		fileHandler->saveDATToFile(
			bonesConf,
			bonesGeometry,
			fileName
		);
	}

	delete fileHandler;
}

void MainWindow::loadDatFromFile() {
	FileHandler * fileHandler = new FileHandler();

	QString filePath = QFileDialog::getOpenFileName(this,
		tr("Open .dat file"), "",
		tr("DAT (*.dat);;All Files(*)"));

	if (!filePath.isEmpty()) {
		fileHandler->loadDatFromFile(mainModel, filePath.toUtf8().constData(), bonesConf, bonesGeometry);

		for (int i = 0; i < glWidgetsVector.size(); i++) {
			fileHandler->reloadParams(glWidgetsVector[i], bonesConf, bonesGeometry);

			glWidgetsVector[i]->limits = limits;
			glWidgetsVector[i]->usedBones = usedBones;
			glWidgetsVector[i]->allBones = allBones;
			glWidgetsVector[i]->asfBones = asfBones;
			glWidgetsVector[i]->idxBonesMap = idxBonesMap;

			update();
			glWidgetsVector[i]->rotate("root", 1.0, pf::Model3D::axisX, 0, bonesConf, limits, 0);
		}
		reloadGLWidgetMenu();
	}	

	delete fileHandler;

}

void MainWindow::saveOneFrameSequenceToFile() {
	FileHandler * fileHandler = new FileHandler();
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save as .amc"), "",
		tr("AMC (*.amc);;All Files(*)"));

	if (!fileName.isEmpty()) {
		SelectModelID *modelID = new SelectModelID(this);
		modelID->resize(100, 100);
		modelID->setComboBox(glWidgetsVector.size());
		modelID->exec();
		int id = 0;
		if (modelID->selected) {
			id = modelID->getID();
			glWidgetsVector[id]->updateUsedBones(glWidgetsVector[id]->usedBones, bonesConf);

			fileHandler->saveAMCSeq(
				glWidgetsVector[id]->modelState,
				asfBones,
				glWidgetsVector[id]->usedBones,
				fileName,
				glWidgetsVector[id]->saveModelState
			);
		}
		delete modelID;
	}

	delete fileHandler;
}

void MainWindow::saveAllFramesSequenceToFile() {
	FileHandler * fileHandler = new FileHandler();
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save as .amc"), "",
		tr("AMC (*.amc);;All Files(*)"));

	if (!fileName.isEmpty()) {
		for (int i = 0; i < glWidgetsVector.size(); i++) {
			QString baseName = QFileInfo(fileName).baseName();
			QString filepath = QFileInfo(fileName).absolutePath();
			baseName += "_" + QString::number(i);
			filepath += "/" + baseName + ".amc";

			glWidgetsVector[i]->updateUsedBones(glWidgetsVector[i]->usedBones, bonesConf);

			fileHandler->saveAMCSeq(
				glWidgetsVector[i]->modelState,
				asfBones,
				glWidgetsVector[i]->usedBones,
				filepath,
				glWidgetsVector[i]->saveModelState
			);
		}
	}

	delete fileHandler;
}

void MainWindow::loadFromAmc() {
	FileHandler * fileHandler = new FileHandler();

	QString filePath = QFileDialog::getOpenFileName(this,
		tr("Open .amc file"), "",
		tr("AMC (*.amc);;All Files(*)"));

	if (!filePath.isEmpty()) {
		SelectModelID *modelID = new SelectModelID(this);
		modelID->resize(100, 100);
		modelID->comboBox->setDisabled(true);
		vector<int> tmp;
		for (int i = 0; i < glWidgetsVector.size(); i++) {
			if (!glWidgetsVector[i]->aviFrames.isEmpty() || !glWidgetsVector[i]->list.isEmpty()) {
				tmp.push_back(i);
				modelID->comboBox->setDisabled(false);
			}
		}

		
		modelID->setComboBox(tmp);
		modelID->exec();
		int id = 0;
		
		if (modelID->selected) {
			
			id = modelID->getID();
			if (id != -1) {
				vector<string> bones;
				vector<vector<float>> states = fileHandler->loadAmcFromFile(filePath, allBones, glWidgetsVector[id]->saveModelState, bones);

				/*zapis do mapy*/
				/*wektor state zawiera tylko zaakceptowane wczesniej konfiguracje*/
				int currentModelState = 0;
				int imagesCounter;
				/*jesli liczba klatek w danym oknie jest wieksza nic liczba wczytanych modelState to zostaja domyslne,
				w przeciwnym przypadku modelState wczytywane jest do momentu przekroczenia iteratora klatek*/
				for (int j = 0; j < glWidgetsVector[id]->saveModelState.size(); j++) {
					if (glWidgetsVector[id]->saveModelState[j]) {
						glWidgetsVector[id]->saveModelStateToMap(glWidgetsVector[id]->bonesRotations[j], states[currentModelState], bones);
						currentModelState++;

						/*odczyt z mapy do glwidget modelState*/
						glWidgetsVector[id]->updateModelStateFromMap(glWidgetsVector[id]->modelState, glWidgetsVector[id]->bonesRotations, bonesConf);
					}
				}

				glWidgetsVector[id]->rotate("root", 1.0, pf::Model3D::axisX, 0, bonesConf, limits, imagesListIterator[id] - 1);
			}
			refreshRotateSideMenu();
		}
		delete modelID;
	}

	delete fileHandler;
}

void MainWindow::buttonAddFramePressed(){
    addFrameMenu(addFrameButtonCounter);
	comboBoxFrameSelector->addItem(tr(std::to_string(addFrameButtonCounter).c_str()));

	glWidgetsVector[addFrameButtonCounter]->limits = limits;
	glWidgetsVector[addFrameButtonCounter]->usedBones = usedBones;
	glWidgetsVector[addFrameButtonCounter]->allBones = allBones;
	glWidgetsVector[addFrameButtonCounter]->modelState = modelState;
	glWidgetsVector[addFrameButtonCounter]->asfBones = asfBones;
	glWidgetsVector[addFrameButtonCounter]->idxBonesMap = idxBonesMap;
	glWidgetsVector[addFrameButtonCounter]->bonesConf = bonesConf;
	glWidgetsVector[addFrameButtonCounter]->bonesGeometry = bonesGeometry;

	modelHandler.updateLength(glWidgetsVector[addFrameButtonCounter]->model, glWidgetsVector[addFrameButtonCounter]->bonesGeometry, bonesGeometry[0].name, 1.0, 0);
	glWidgetsVector[addFrameButtonCounter]->update();

	if (frameDeleted) {
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

    glWidgetsVector[addFrameButtonCounter]->setMinimumSize(100,300);
    glWidgetsVector[addFrameButtonCounter]->setMaximumHeight(400);
    glWidgetsVector[addFrameButtonCounter]->setMaximumWidth(1200);

	addFrameScrollingMenu(menuButtonsVector[addFrameButtonCounter], addFrameButtonCounter);
    prepareFrameMenu(addFrameButtonCounter);

    /*rows, cols, rowspan, colspan*/
    if(i%2==0){
        if(i>1) j=(i/2)*2;
		gridChildLayout->addWidget(bordersWidgetsVector[addFrameButtonCounter], (i / 2) * 10 + j, 0, 12, 10);
        gridChildLayout->addWidget(glWidgetsVector[addFrameButtonCounter], (i / 2) * 10 + j, 0, 10, 10);
		gridChildLayout->addWidget(hMenuWidgetVector[addFrameButtonCounter], (i / 2) * 10 + 11 + j, 0, 1, 9);
		gridChildLayout->addLayout(menuButtonsLayoutVector[addFrameButtonCounter], (i / 2) * 10 + 11 + j, 9, 1, 1);
		gridChildLayout->addWidget(frameIDVector[addFrameButtonCounter], (i / 2) * 10 + j, 0, 2, 2);
    }else {
        if(i>1) j=(i/2)*2;
		gridChildLayout->addWidget(bordersWidgetsVector[addFrameButtonCounter], (i / 2) * 10 + j, 10, 12, 10);
        gridChildLayout->addWidget(glWidgetsVector[addFrameButtonCounter], (i / 2)* 10 + j, 10, 10, 10);
		gridChildLayout->addWidget(hMenuWidgetVector[addFrameButtonCounter], (i / 2) * 10 + 11 + j, 10, 1, 9);
		gridChildLayout->addLayout(menuButtonsLayoutVector[addFrameButtonCounter], (i / 2) * 10 + 11 + j, 19, 1, 1);
		gridChildLayout->addWidget(frameIDVector[addFrameButtonCounter], (i / 2) * 10 + j, 10, 2, 2);
    }

	if (comboBoxFrameSelector->count() == 0) {
		//saveAmcAction->setEnabled(true);
		manageModelBonesAction->setEnabled(true);
	}

	if (comboBoxFrameSelector->count() == 1)
		saveAmcSequenceAction->setEnabled(true);

	/*ustawienie kosci dodanego modelu*/
	FileHandler *fileHandler = new FileHandler();
	fileHandler->reloadParams(glWidgetsVector[addFrameButtonCounter], bonesConf, bonesGeometry);

	glWidgetsVector[addFrameButtonCounter]->limits = limits;
	glWidgetsVector[addFrameButtonCounter]->usedBones = usedBones;
	glWidgetsVector[addFrameButtonCounter]->allBones = allBones;
	glWidgetsVector[addFrameButtonCounter]->asfBones = asfBones;
	glWidgetsVector[addFrameButtonCounter]->idxBonesMap = idxBonesMap;
	
	update();
	delete fileHandler;

	if (cameras.size() > 0) {
		addCameraMenuToFrame();
		//for (int i = 0; i < glWidgetsVector.size(); i++)
		selectCameraMenuVector[i]->setDisabled(false);
	}

	if (glWidgetsVector.size() > 1) {
		for(int id = 0;id < glWidgetsVector.size();id++)
			deleteFrameMenuVector[id]->setDisabled(false);
	}
}

void MainWindow::refreshGridChildLayout() {

	
}

void MainWindow::setTextIteratorLabel(QLabel *label, int iterator, int listSize) {
	QString string = QString("%1 / %2").arg(iterator).arg(listSize);
	label->setText(string);
	label->setAlignment(Qt::AlignCenter);
	label->setFixedWidth(50);
}

void MainWindow::increaseIteratorPressed(int i) {
	int listIterator = imagesListIterator[i];
	if (!glWidgetsVector[i]->isAvi) {
		if (listIterator < glWidgetsVector[i]->list.size())
			listIterator++;
		else
			listIterator = glWidgetsVector[i]->list.size();

		setTextIteratorLabel(countersVector[i], listIterator, glWidgetsVector[i]->list.size());
		imagesListIterator[i] = listIterator;

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

		glWidgetsVector[i]->aviFrames[listIterator - 1].copyTo(glWidgetsVector[i]->aviFrame);
	}

	slidersVector[i]->setValue(imagesListIterator[i]);
	glWidgetsVector[i]->setGLWidgetModelState(imagesListIterator[i] - 1);
	glWidgetsVector[i]->rotate("root", 1.0, pf::Model3D::axisX, 0, bonesConf, limits, imagesListIterator[i] - 1);
	refreshRotateSideMenu();
	glWidgetsVector[i]->update();
}

void MainWindow::reduceIteratorPressed(int i) {
	int listIterator = imagesListIterator[i];
	if (!glWidgetsVector[i]->isAvi) {
		if (listIterator > 1)
			listIterator--;
		else
			listIterator = 1;

		setTextIteratorLabel(countersVector[i], listIterator, glWidgetsVector[i]->list.size());
		imagesListIterator[i] = listIterator;

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

		glWidgetsVector[i]->aviFrames[listIterator - 1].copyTo(glWidgetsVector[i]->aviFrame);
	}

	slidersVector[i]->setValue(imagesListIterator[i]);
	glWidgetsVector[i]->setGLWidgetModelState(imagesListIterator[i] - 1);
	glWidgetsVector[i]->rotate("root", 1.0, pf::Model3D::axisX, 0, bonesConf, limits, imagesListIterator[i] - 1);
	refreshRotateSideMenu();
	glWidgetsVector[i]->update();
}

void MainWindow::minIteratorPressed(int i) {
	imagesListIterator[i] = 1;
	if (!glWidgetsVector[i]->isAvi) {
		setTextIteratorLabel(countersVector[i], imagesListIterator[i], glWidgetsVector[i]->list.size());

		QString imgString = glWidgetsVector[i]->list[imagesListIterator[i] - 1].absoluteFilePath();
		glWidgetsVector[i]->imgPath = imgString.toUtf8().constData();
	}
	else {
		setTextIteratorLabel(countersVector[i], imagesListIterator[i], glWidgetsVector[i]->aviFrames.size());
		glWidgetsVector[i]->aviFrames[imagesListIterator[i] - 1].copyTo(glWidgetsVector[i]->aviFrame);
	}

	slidersVector[i]->setValue(imagesListIterator[i]);
	glWidgetsVector[i]->setGLWidgetModelState(imagesListIterator[i] - 1);
	glWidgetsVector[i]->rotate("root", 1.0, pf::Model3D::axisX, 0, bonesConf, limits, imagesListIterator[i] - 1);
	refreshRotateSideMenu();
	glWidgetsVector[i]->update();
}

void MainWindow::maxIteratorPressed(int i) {
	if (!glWidgetsVector[i]->isAvi) {
		imagesListIterator[i] = glWidgetsVector[i]->list.size();
		setTextIteratorLabel(countersVector[i], imagesListIterator[i], glWidgetsVector[i]->list.size());

		QString imgString = glWidgetsVector[i]->list[imagesListIterator[i] - 1].absoluteFilePath();
		glWidgetsVector[i]->imgPath = imgString.toUtf8().constData();
	}
	else {
		imagesListIterator[i] = glWidgetsVector[i]->aviFrames.size();
		setTextIteratorLabel(countersVector[i], imagesListIterator[i], glWidgetsVector[i]->aviFrames.size());
		glWidgetsVector[i]->aviFrames[imagesListIterator[i] - 1].copyTo(glWidgetsVector[i]->aviFrame);
	}

	slidersVector[i]->setValue(imagesListIterator[i]);
	glWidgetsVector[i]->setGLWidgetModelState(imagesListIterator[i] - 1);
	glWidgetsVector[i]->rotate("root", 1.0, pf::Model3D::axisX, 0, bonesConf, limits, imagesListIterator[i] - 1);
	refreshRotateSideMenu();
	glWidgetsVector[i]->update();
}

void MainWindow::playPausePressed(int i) {
	QPixmap pixPause(":/images/ETC/buttonImages/pause.png"), pixPlay(":/images/ETC/buttonImages/play.png");
	QIcon iconPause(pixPause), iconPlay(pixPlay);

	if (playPressedVector[i] == false) {
		playPauseButtonsVector[i]->setIcon(pixPause);

		if (glWidgetsVector[i]->isAvi) {
			if (imagesListIterator[i] == glWidgetsVector[i]->aviFrames.size()) {
				backgroundTimersVector[i]->start(1);
			}
			else {
				backgroundTimersVector[i]->start(2000);
			}
		}
		else {
			if (imagesListIterator[i] == glWidgetsVector[i]->list.size()) {
				backgroundTimersVector[i]->start(1);
			}
			else {
				backgroundTimersVector[i]->start(2000);
			}
		}
		
	}
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

	vector<int> usedBonesIDs = boneManagmentWindow->getUsedBonesIDs(bonesGeometry, allBones);
	boneManagmentWindow->setUsedBones(asfBones, usedBonesIDs);
	
	boneManagmentWindow->resize(500, 400);
	boneManagmentWindow->exec();

	vector<int> bonesIDs = boneManagmentWindow->selectedBonesIDs;
	vector<string> bonesNames = boneManagmentWindow->selectedBonesNames;

	boneManagmentWindow->matchBonesWithASF(bonesIDs, bonesNames, asfBones);
	if (!bonesIDs.empty() && !bonesNames.empty()) {

		FileHandler *fileHandler = new FileHandler();
		/*aktualizacja map*/
		for (int i = 0; i < glWidgetsVector.size(); i++) {
			for (int j = 0; j < glWidgetsVector[i]->modelState.size(); j++) {
				glWidgetsVector[i]->saveModelStateToMap(glWidgetsVector[i]->bonesRotations[j], glWidgetsVector[i]->modelState[j], bonesConf);
			}
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
			fileHandler->addBoneConf(bonesConf,
				idxBonesMap,
				asfBones,
				bonesIDs[j]);
				
			/*Dodanie nowego obiektu boneGeometry*/
			fileHandler->addBoneGeo(bonesGeometry,
				idxBonesMap,
				asfBones,
				bonesIDs[j]);
		}

		fileHandler->saveDATToFile(bonesConf, bonesGeometry, "tmpDatFile.dat");
		bonesConf.clear();
		bonesGeometry.clear();
		mainModel = new pf::Model3D(pf::Model3D::Cylinder, ASF_TEMPLATE_PATH, "tmpDatFile.dat");
		mainModel->loadConfig("tmpDatFile.dat", bonesConf, bonesGeometry);

		limits = mainModel->getLimits();
		usedBones = mainModel->getNamesMovingBones();
		allBones = mainModel->getNamesBones();

		for (int j = 0; j < glWidgetsVector.size(); j++) {

			glWidgetsVector[j]->model = new pf::Model3D(pf::Model3D::Cylinder, ASF_TEMPLATE_PATH, "tmpDatFile.dat");
			glWidgetsVector[j]->model->loadConfig("tmpDatFile.dat", glWidgetsVector[j]->bonesConf, glWidgetsVector[j]->bonesGeometry);

			glWidgetsVector[j]->limits = limits;
			glWidgetsVector[j]->usedBones = usedBones;
			glWidgetsVector[j]->allBones = allBones;
			glWidgetsVector[j]->asfBones = asfBones;
			glWidgetsVector[j]->idxBonesMap = idxBonesMap;
		}
		std::remove("tmpDatFile.dat");

		/*ustawienie dlugosci wybranych kosci*/
		for (int i = 0; i < glWidgetsVector.size(); i++) {
			for (int j = 0; j < bonesGeometry.size(); j++) {

				pf::boneGeometry bGeo = bonesGeometry[j];

				if (fileHandler->isBoneChecked(bGeo.name, bonesNames)) {
					//bGeo.length = glWidgetsVector[i]->bonesLength.find(bGeo.name)->second;
					glWidgetsVector[i]->model->updateBoneGeometry(bGeo.name, bGeo);
				}
				else {}
			}
		}
	
		modelHandler.updateBonesLengthFromMap(bonesLength, bonesGeometry);
		modelHandler.updateBonesRadiusFromMap(modelRadius, bonesGeometry);
		modelHandler.updateDOFFromMap(modelDOF, bonesConf);
		modelHandler.updateLimitsFromMap(modelLimits, bonesConf);
		modelHandler.updateVelocityFromMap(modelVelocity, bonesConf);

		/*aktualizacja model state*/
		for (int i = 0; i < glWidgetsVector.size(); i++) {
			for (int j = 0; j < glWidgetsVector[i]->modelState.size(); j++) {
				glWidgetsVector[i]->updateModelStateFromMap(glWidgetsVector[i]->modelState, glWidgetsVector[i]->bonesRotations, bonesConf);
			}
			
		}
		
		
		if (std::find(bonesNames.begin(), bonesNames.end(), "Spine1") != bonesNames.end()) {
			if (std::find(bonesNames.begin(), bonesNames.end(), "Spine1_dum1") != bonesNames.end()) {
				float spine1Length, spine1_dum1Length;
				spine1Length = bonesLength.find("Spine1_dum1")->second;
				spine1_dum1Length = bonesLength.find("Spine1")->second;

				bonesLength.at("Spine1_dum1") = spine1_dum1Length;
				bonesLength.at("Spine1") = spine1Length;
				/*cout << spine1Length << endl;
				cout << spine1_dum1Length << endl;

				cout << bonesLength.find("Spine1_dum1")->second << endl;
				cout << bonesLength.find("Spine1")->second << endl;
				*/
				modelHandler.updateBonesLengthFromMap(bonesLength, bonesGeometry);
			}
		}

		reloadGLWidgetMenu();
		updateBoneConfigFromCB();
		updateBoneConfigFromLimitsSB();
		updateBoneConfigFromVelocitySB();
		delete fileHandler;

		/*aktualizacja dlugosci kosci*/
		for (int j = 0; j < bonesGeometry.size(); j++) {
			modelHandler.updateLength(mainModel, bonesGeometry, bonesGeometry[j].name, 1.0, 0.0);

			for (int i = 0; i < comboBoxFrameSelector->count(); i++) {
				modelHandler.updateLength(glWidgetsVector[i]->model, glWidgetsVector[i]->bonesGeometry, bonesGeometry[j].name, 1.0, 0.0);
				glWidgetsVector[i]->update();
			}
		}

	}
	delete boneManagmentWindow;

}

void MainWindow::openCameraConfigPressed() {
	CamerasConfig *cfg = new CamerasConfig(this);
	cfg->firstTime = cameraFirstTime;
	cfg->resize(500, 400);
	/*jesli jakies kamery zostaly wczesniej dodane*/
	if (cameras.size() > 0) {
		cfg->getCamerasFromMainWindow(cameras);
	}
	cfg->exec();
	if (cfg->okClicked) {
		cfg->returnCameras(cameras);
		if (cameras.size() > 0) {
			cameraFirstTime = false;
			for (int i = 0; i < glWidgetsVector.size(); i++) 
				selectCameraMenuVector[i]->setDisabled(false);

			setCamerasMenu();
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
	}
}

void MainWindow::loadCamerasPressed() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Load cameras configuration"), "", tr("(*.ini)"));

	if (!fileName.isEmpty()) {
		loadCameras(fileName);
		cameraFirstTime = false;
		//reloadGLWidgetMenu();
	}
}

void MainWindow::setModelPressed() {
	modelInDialog = new ModelInDialog(this);

	modelInDialog->setWindowTitle("Model");
	modelInDialog->setAttribute(Qt::WA_DeleteOnClose);
	modelInDialog->resize(500, 400);
	scrollArea->setDisabled(true);
	modelInDialog->setGLWidget(modelState, mainModel, idxBonesMap, bonesGeometry, bonesConf);

	//modelInDialog->exec();
	modelInDialog->show();
	connect(modelInDialog, SIGNAL(destroyed()), this, SLOT(deleteModelInDialog()));
}

void MainWindow::showModelInDialogPressed(int i) {
	ModelInDialog *modelInDialog = new ModelInDialog(this);

	modelInDialog->setWindowTitle("Model");

	modelInDialog->resize(500, 400);
	//modelInDialog->setGLWidget(glWidgetsVector[i]->modelState, glWidgetsVector[i]->model, glWidgetsVector[i]->idxBonesMap, glWidgetsVector[i]->bonesGeometry, glWidgetsVector[i]->bonesConf);
	modelInDialog->exec();

	delete modelInDialog;
}

void MainWindow::setConfigFromModelPressed(int i) {

}

void MainWindow::setConfigToAllModelsPressed(int i) {
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, "", "Konfiguracja modelu " + QString::number(i) + " zostanie zapisana do wszystkich modeli.\n"
											+ "Operacja jest nieodwracalna.\nKontynuowac?", QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Yes) {
		for (int j = 0; j < glWidgetsVector.size(); j++) {
			if (i != j) {
				glWidgetsVector[j]->copyConfigToGlWidget(glWidgetsVector[j], glWidgetsVector[i]);
				/*zmiena dlugosci kosci aktualizuje dlugosci kosci wyswietlane na ekranie*/
				if (glWidgetsVector[j]->bonesGeometry.size() != 0) {
					modelHandler.updateLength(glWidgetsVector[i]->model, glWidgetsVector[i]->bonesGeometry, glWidgetsVector[i]->bonesGeometry[0].name, 1.0, 0);
					glWidgetsVector[i]->update();
				}

				/*rotacja modelu o 0 stopni aktualizuje obrot na ekranie */
				if (glWidgetsVector[j]->bonesConf.size() != 0)
					glWidgetsVector[j]->rotate(glWidgetsVector[i]->bonesConf[0].name, 1.0, pf::Model3D::axisX, 0, bonesConf, limits, imagesListIterator[i] - 1);

			}
		}
	}
	else {
		cout << "no clicked" << endl;
	}
}

void MainWindow::selectAviBackgroundPressed(int i) {
	glWidgetsVector[i]->loadAviFile();
	if (glWidgetsVector[i]->isAvi) {
		imagesListIterator[i] = 1;
		sliderCheckBoxesVector[i]->setChecked(false);

		if (globalSlider->maximum() < glWidgetsVector[i]->aviFrames.size())
			globalSlider->setMaximum(glWidgetsVector[i]->aviFrames.size());

		setTextIteratorLabel(countersVector[i], imagesListIterator[i], glWidgetsVector[i]->aviFrames.size());
		glWidgetsVector[i]->aviFrames[imagesListIterator[i] - 1].copyTo(glWidgetsVector[i]->aviFrame);
		glWidgetsVector[i]->drawBckg = true;

		prepareSlider(slidersVector[i], i);

		hMenuWidgetVector[i]->show();

		glWidgetsVector[i]->updateModelStateVector(glWidgetsVector[i]->aviFrames.size());
	}
	else {
		hMenuWidgetVector[i]->hide();
	}
}

void MainWindow::selectImagesBackgroundPressed(int i) {
	glWidgetsVector[i]->loadFiles();
	if (!glWidgetsVector[i]->list.isEmpty()) {
		if (!glWidgetsVector[i]->isAvi) {
			imagesListIterator[i] = 1;
			sliderCheckBoxesVector[i]->setChecked(false);

			if (globalSlider->maximum() < glWidgetsVector[i]->list.size())
				globalSlider->setMaximum(glWidgetsVector[i]->list.size());

			setTextIteratorLabel(countersVector[i], imagesListIterator[i], glWidgetsVector[i]->list.size());
			glWidgetsVector[i]->imgPath = glWidgetsVector[i]->list[imagesListIterator[i] - 1].absoluteFilePath().toUtf8().constData();
			glWidgetsVector[i]->drawBckg = true;

			prepareSlider(slidersVector[i], i);

			hMenuWidgetVector[i]->show();

			glWidgetsVector[i]->updateModelStateVector(glWidgetsVector[i]->list.size());
		}
		else {
			hMenuWidgetVector[i]->hide();
		}
	}
}

void MainWindow::deleteFramePressed(int i) {
	frameDeleted = false;
	int howManyWidnowsLeft = glWidgetsVector.size() - 1;
	
	if (howManyWidnowsLeft > 0) {
		QVector<int> modeVector;
		QVector<bool> isAviVector;
		QVector<QString> loadedImagesFolderPathVector;
		QVector<QString> aviFilePathVector;
		QVector<QVector<cv::Mat>> aviFramesVector;
		QVector<QFileInfoList> listVector;
		QVector<string> imgPathVector;
		QVector<cv::Mat> aviFrameVector;
		//model ???????
		QVector<bool> isInMainWindowVector;
		QVector<bool> drawBckgVector;
		//QVector<int> hzVector;
		QVector<bool> castVector;
		QVector<int> cameraIDVector;
		QVector<vector<vector<float>>> modelStateVector;
		QVector<vector<bool>> saveModelStateVector;
		QVector<vector<map<string, pf::Vec3f>>> bonesRotationsVector;
		QVector<vector<vector<float>>> modelTranslationVector;
		QVector<int> imagesListIteratorVector;
		
		for (int j = 0; j < glWidgetsVector.size(); j++) {
			if (j != i) {
				imagesListIteratorVector.push_back(imagesListIterator[j]);
				modeVector.push_back(glWidgetsVector[j]->mode);
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

					if(!glWidgetsVector[j]->imgPath.empty())
						imgPathVector.push_back(glWidgetsVector[j]->imgPath);
					else 
						imgPathVector.push_back("");

					cv::Mat matTmp;
					aviFrameVector.push_back(glWidgetsVector[j]->aviFrame);
				}
				isInMainWindowVector.push_back(glWidgetsVector[j]->isInMainWindow);
				drawBckgVector.push_back(glWidgetsVector[j]->drawBckg);
				//hzVector.push_back(glWidgetsVector[j]->hz);
				castVector.push_back(glWidgetsVector[j]->cast);
				cameraIDVector.push_back(glWidgetsVector[j]->cameraID);
				modelStateVector.push_back(glWidgetsVector[j]->modelState);
				saveModelStateVector.push_back(glWidgetsVector[j]->saveModelState);
				bonesRotationsVector.push_back(glWidgetsVector[j]->bonesRotations);
				modelTranslationVector.push_back(glWidgetsVector[j]->modelTranslation);
			}
		}
		

		/*qDeleteAll(frameScrollingMenu);
		qDeleteAll(selectBackgroundMenuVector); 
		qDeleteAll(selectCameraMenuVector);
		qDeleteAll(showModelInDialogVector); 
		qDeleteAll(selectAviBackgroundVector); 
		qDeleteAll(selectImagesBackgroundVector); 
		qDeleteAll(setConfigFromModelVector); 
		qDeleteAll(setConfigToAllModelsVector); 
		qDeleteAll(deleteFrameMenuVector); 
		for (int k = 0; k < selectCameraActionVector.size(); k++) {
			qDeleteAll(selectCameraActionVector[k]);
		}

		frameScrollingMenu.clear();
		selectBackgroundMenuVector.clear();
		selectCameraMenuVector.clear();
		showModelInDialogVector.clear();
		selectAviBackgroundVector.clear();
		selectImagesBackgroundVector.clear();
		setConfigFromModelVector.clear();
		setConfigToAllModelsVector.clear();
		deleteFrameMenuVector.clear();
		selectCameraActionVector.clear();
		*/
		qDeleteAll(frameIDVector);
		qDeleteAll(glWidgetsVector);
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
		qDeleteAll(showModelInDialogMapperVector);
		qDeleteAll(setConfigFromModelMapperVector);
		qDeleteAll(setConfigToAllModelsMapperVector);
		qDeleteAll(addImagesBackgroundMapperVector);

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
		showModelInDialogMapperVector.clear();
		setConfigFromModelMapperVector.clear();
		setConfigToAllModelsMapperVector.clear();
		addImagesBackgroundMapperVector.clear();
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

		for (int j = 0; j < howManyWidnowsLeft; j++) {
			addFrameButton->click();
		}
		
		for (int j = 0; j < glWidgetsVector.size(); j++) {

			glWidgetsVector[j]->mode = modeVector[j];
			imagesListIterator.push_back(imagesListIteratorVector[j]);
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

			//glWidgetsVector[j]->hz = hzVector[j];
			glWidgetsVector[j]->cast = castVector[j];
			glWidgetsVector[j]->cameraID = cameraIDVector[j];

			if (glWidgetsVector[j]->cast) {
				glWidgetsVector[j]->setGLWidgetCamera(cameras[glWidgetsVector[j]->cameraID]);
				//odznacz wszystkie kamery
				for (int k = 0; k < selectCameraActionVector[j].size(); k++) 
					selectCameraActionVector[j][k]->setChecked(false);
				
				//zaznacz wybrana kamer
				selectCameraActionVector[j][glWidgetsVector[j]->cameraID]->setChecked(true);
			}
			glWidgetsVector[j]->modelState = modelStateVector[j];
			glWidgetsVector[j]->saveModelState = saveModelStateVector[j];
			glWidgetsVector[j]->bonesRotations = bonesRotationsVector[j];
			glWidgetsVector[j]->modelTranslation = modelTranslationVector[j];
		}
		
		for (int j = 0; j < glWidgetsVector.size(); j++) {
			//rotacja modelu o 0 stopni aktualizuje obrot na ekranie 
			if (bonesConf.size() != 0) {
				glWidgetsVector[j]->rotate(bonesConf[0].name, 1.0, pf::Model3D::axisX, 0, bonesConf, limits, imagesListIterator[j] - 1);

				glWidgetsVector[j]->update();
			}
		}

		frameDeleted = true;

		refreshRotateSideMenu();
		if (cameras.size() > 0) {
			for (int i = 0; i < glWidgetsVector.size(); i++)
				selectCameraMenuVector[i]->setDisabled(false);
		}
	}
	/*wylacz mozliwosc usuniecia klatki*/
	if(howManyWidnowsLeft == 1)
		deleteFrameMenuVector[0]->setDisabled(true);
	else {
		deleteFrameMenuVector[0]->setDisabled(false);
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
	imagesListIterator[i] = slidersVector[i]->value();
	if (!glWidgetsVector[i]->isAvi) {

		setTextIteratorLabel(countersVector[i], imagesListIterator[i], glWidgetsVector[i]->list.size());

		QString imgString = glWidgetsVector[i]->list[imagesListIterator[i] - 1].absoluteFilePath();
		glWidgetsVector[i]->imgPath = imgString.toUtf8().constData();
	}
	else {
		setTextIteratorLabel(countersVector[i], imagesListIterator[i], glWidgetsVector[i]->aviFrames.size());
		glWidgetsVector[i]->aviFrames[imagesListIterator[i] - 1].copyTo(glWidgetsVector[i]->aviFrame);
	}

	glWidgetsVector[i]->setGLWidgetModelState(imagesListIterator[i] - 1);
	glWidgetsVector[i]->rotate("root", 1.0, pf::Model3D::axisX, 0, bonesConf, limits, imagesListIterator[i] - 1);
	refreshRotateSideMenu();
	glWidgetsVector[i]->update();
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
				glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate(boneName, direction, pf::Model3D::axisX, sideRotateMenuSpinBoxesVector[qSpinBoxID]->value(), bonesConf, limits, imagesListIterator[frameNumber] - 1);
			if (axis == "Y")
				glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate(boneName, direction, pf::Model3D::axisY, sideRotateMenuSpinBoxesVector[qSpinBoxID]->value(), bonesConf, limits, imagesListIterator[frameNumber] - 1);
			if (axis == "Z")
				glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate(boneName, direction, pf::Model3D::axisZ, sideRotateMenuSpinBoxesVector[qSpinBoxID]->value(), bonesConf, limits, imagesListIterator[frameNumber] - 1);
		
			/* jesli wybrano okno nr 0, a iteratory zdjec innych okien sa takie same jak okna 0 - rotacja kosci obejmuje wszystkie te okna*/
			for (int i = 1; i < glWidgetsVector.size(); i++) {
				if (imagesListIterator[0] - 1 == imagesListIterator[i] - 1) {
					if (axis == "X")
						glWidgetsVector[i]->rotate(boneName, direction, pf::Model3D::axisX, sideRotateMenuSpinBoxesVector[qSpinBoxID]->value(), bonesConf, limits, imagesListIterator[i] - 1);
					if (axis == "Y")
						glWidgetsVector[i]->rotate(boneName, direction, pf::Model3D::axisY, sideRotateMenuSpinBoxesVector[qSpinBoxID]->value(), bonesConf, limits, imagesListIterator[i] - 1);
					if (axis == "Z")
						glWidgetsVector[i]->rotate(boneName, direction, pf::Model3D::axisZ, sideRotateMenuSpinBoxesVector[qSpinBoxID]->value(), bonesConf, limits, imagesListIterator[i] - 1);

				}
			}
		}
		/*dla pozostalyhc okien*/
		else {
			if (axis == "X")
				glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate(boneName, direction, pf::Model3D::axisX, sideRotateMenuSpinBoxesVector[qSpinBoxID]->value(), bonesConf, limits, imagesListIterator[frameNumber] - 1);
			if (axis == "Y")
				glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate(boneName, direction, pf::Model3D::axisY, sideRotateMenuSpinBoxesVector[qSpinBoxID]->value(), bonesConf, limits, imagesListIterator[frameNumber] - 1);
			if (axis == "Z")
				glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate(boneName, direction, pf::Model3D::axisZ, sideRotateMenuSpinBoxesVector[qSpinBoxID]->value(), bonesConf, limits, imagesListIterator[frameNumber] - 1);
		}

		sideRotateMenuLineEditsVector[qLineEditID]->setText(QString::number(glWidgetsVector[comboBoxFrameSelector->currentIndex()]->modelState[backgroundID][qLineEditID+3]));
	}

	else {
		//Okno błedu
		cout << "ERROR" << endl;
	}
}

void MainWindow::updateBoneLength(QString str) {
	QStringList values = str.split(";");
	if (values.count() == 4) {
		string boneName = values.at(0).toStdString();
		float direction = values.at(1).toFloat();
		int qLineEditID = values.at(2).toInt();
		int qSpinBoxID = values.at(3).toInt();

		modelHandler.updateLength(mainModel, bonesGeometry, boneName, direction, sideLengthMenuSpinBoxesVector[qSpinBoxID]->value());

		for (int i = 0; i < comboBoxFrameSelector->count(); i++) {
			modelHandler.updateLength(glWidgetsVector[i]->model, glWidgetsVector[i]->bonesGeometry, boneName, direction, sideLengthMenuSpinBoxesVector[qSpinBoxID]->value());
			glWidgetsVector[i]->update();
		}

		sideLengthMenuLineEditsVector[qLineEditID]->setText(QString::number(modelHandler.getBoneLength(bonesGeometry, boneName)));
		sideGeometryMenuLineEditsVector[qLineEditID * 3]->setText(QString::number(modelHandler.getBoneLength(bonesGeometry, boneName)));
		setGoldenRatioCheckBox->setChecked(false);
	}
	else {
		//Okno błedu
		cout << "ERROR" << endl;
	}
}

void MainWindow::updateBoneLengthGeometry(QString str) {
	QStringList values = str.split(";");
	if (values.count() == 4) {
		string boneName = values.at(0).toStdString();
		float direction = values.at(1).toFloat();
		int qLineEditID = values.at(2).toInt();
		int qSpinBoxID = values.at(3).toInt();

		modelHandler.updateLength(mainModel, bonesGeometry, boneName, direction, sideLengthMenuSpinBoxesVector[qSpinBoxID]->value());

		for (int i = 0; i < comboBoxFrameSelector->count(); i++) {
			modelHandler.updateLength(glWidgetsVector[i]->model, glWidgetsVector[i]->bonesGeometry, boneName, direction, sideLengthMenuSpinBoxesVector[qSpinBoxID]->value());
			glWidgetsVector[i]->update();
		}

		sideGeometryMenuLineEditsVector[qLineEditID]->setText(QString::number(modelHandler.getBoneLength(bonesGeometry, boneName)));
		sideLengthMenuLineEditsVector[qLineEditID/3]->setText(QString::number(modelHandler.getBoneLength(bonesGeometry, boneName)));
	}
	else {
		//Okno błedu
		cout << "ERROR" << endl;
	}
}

void MainWindow::updateBoneTopRadius(QString str) {
	QStringList values = str.split(";");
	if (values.count() == 4) {
		string boneName = values.at(0).toStdString();
		float direction = values.at(1).toFloat();
		int qLineEditID = values.at(2).toInt();
		int qSpinBoxID = values.at(3).toInt();

		modelHandler.updateTopRadius(mainModel, bonesGeometry, boneName, direction, sideGeometryMenuSpinBoxesVector[qSpinBoxID]->value());

		for (int i = 0; i < comboBoxFrameSelector->count(); i++) {
			modelHandler.updateTopRadius(glWidgetsVector[i]->model, glWidgetsVector[i]->bonesGeometry, boneName, direction, sideGeometryMenuSpinBoxesVector[qSpinBoxID]->value());
			glWidgetsVector[i]->update();
		}

		sideGeometryMenuLineEditsVector[qLineEditID]->setText(QString::number(modelHandler.getBoneTopRadius(bonesGeometry, bonesGeometry[qLineEditID / 3].name)));
		update();
	}
	else {
		//Okno błedu
		cout << "ERROR" << endl;
	}
}

void MainWindow::updateBoneBottomRadius(QString str) {
	QStringList values = str.split(";");
	if (values.count() == 4) {
		string boneName = values.at(0).toStdString();
		float direction = values.at(1).toFloat();
		int qLineEditID = values.at(2).toInt();
		int qSpinBoxID = values.at(3).toInt();

		modelHandler.updateBottomRadius(mainModel, bonesGeometry, boneName, direction, sideGeometryMenuSpinBoxesVector[qSpinBoxID]->value());

		for (int i = 0; i < comboBoxFrameSelector->count(); i++) {
			modelHandler.updateBottomRadius(glWidgetsVector[i]->model, glWidgetsVector[i]->bonesGeometry, boneName, direction, sideGeometryMenuSpinBoxesVector[qSpinBoxID]->value());
			glWidgetsVector[i]->update();
		}

		sideGeometryMenuLineEditsVector[qLineEditID]->setText(QString::number(modelHandler.getBoneTopRadius(bonesGeometry, bonesGeometry[qLineEditID / 3].name)));
		update();
	}
	else {
		//Okno błedu
		cout << "ERROR" << endl;
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
				glWidgetsVector[comboBoxFrameSelector->currentIndex()]->translate(direction, pf::Model3D::axisX, sideLengthMenuSpinBoxesExtraVector[qSpinBoxID]->value(), imagesListIterator[frameNumber] - 1);
			if (axis == "Y")
				glWidgetsVector[comboBoxFrameSelector->currentIndex()]->translate(direction, pf::Model3D::axisY, sideLengthMenuSpinBoxesExtraVector[qSpinBoxID]->value(), imagesListIterator[frameNumber] - 1);
			if (axis == "Z")
				glWidgetsVector[comboBoxFrameSelector->currentIndex()]->translate(direction, pf::Model3D::axisZ, sideLengthMenuSpinBoxesExtraVector[qSpinBoxID]->value(), imagesListIterator[frameNumber] - 1);

			/* jesli wybrano okno nr 0, a iteratory zdjec innych okien sa takie same jak okna 0 - translacja modelu obejmuje wszystkie te okna*/
			for (int i = 1; i < glWidgetsVector.size(); i++) {
				if (imagesListIterator[0] - 1 == imagesListIterator[i] - 1) {
					if (axis == "X")
						glWidgetsVector[i]->translate(direction, pf::Model3D::axisX, sideLengthMenuSpinBoxesExtraVector[qSpinBoxID]->value(), imagesListIterator[i] - 1);
					if (axis == "Y")
						glWidgetsVector[i]->translate(direction, pf::Model3D::axisY, sideLengthMenuSpinBoxesExtraVector[qSpinBoxID]->value(), imagesListIterator[i] - 1);
					if (axis == "Z")
						glWidgetsVector[i]->translate(direction, pf::Model3D::axisZ, sideLengthMenuSpinBoxesExtraVector[qSpinBoxID]->value(), imagesListIterator[i] - 1);

				}
			}
		}
		/*dla pozostalych okien*/
		else {
			if (axis == "X")
				glWidgetsVector[comboBoxFrameSelector->currentIndex()]->translate(direction, pf::Model3D::axisX, sideLengthMenuSpinBoxesExtraVector[qSpinBoxID]->value(), imagesListIterator[frameNumber] - 1);
			if (axis == "Y")
				glWidgetsVector[comboBoxFrameSelector->currentIndex()]->translate(direction, pf::Model3D::axisY, sideLengthMenuSpinBoxesExtraVector[qSpinBoxID]->value(), imagesListIterator[frameNumber] - 1);
			if (axis == "Z")
				glWidgetsVector[comboBoxFrameSelector->currentIndex()]->translate(direction, pf::Model3D::axisZ, sideLengthMenuSpinBoxesExtraVector[qSpinBoxID]->value(), imagesListIterator[frameNumber] - 1);
		}

		sideLengthMenuLineEditsExtraVector[qLineEditID]->setText(QString::number(glWidgetsVector[comboBoxFrameSelector->currentIndex()]->modelState[backgroundID][qLineEditID]));
	}
	else {
		//Okno błedu
		cout << "ERROR" << endl;
	}
}

void MainWindow::scale(QString str) {

	QStringList values = str.split(";");
	if (values.count() == 2) {
		float direction = values.at(0).toFloat();
		int qSpinBoxID = values.at(1).toInt();

		glWidgetsVector[comboBoxFrameSelector->currentIndex()]->scale(direction, sideLengthMenuSpinBoxesExtraVector[qSpinBoxID]->value());

		refreshLengthSideMenu();
		//sideLengthMenuLineEditsExtraVector[qLineEditID]->setText(QString::number(glWidgetsVector[comboBoxFrameSelector->currentIndex()]->modelState[0][qLineEditID]));
	}
	else {
		//Okno błedu
		cout << "ERROR" << endl;
	}
}

void MainWindow::reloadGLWidgetMenu() {

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
	qDeleteAll(lengthMapperVector);
	qDeleteAll(geometryMapperVector);

	delete saveCurrentModelStateToVector;
	delete setGoldenRatioButton;
	delete setGoldenRatioCheckBox;
	delete goldenRatioLayout;

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
	lengthMapperVector.clear();
	geometryMapperVector.clear();

	addGLWidgetRotateMenu();
	addGLWidgetLengthMenu();
	addGLWidgetGeometryMenu();

	/*for (int i = 0; i < comboBoxFrameSelector->count(); i++) {
		if(comboBoxFrameSelector->itemText(i).toInt() == comboBoxFrameSelector->currentIndex())
			frameIDVector[comboBoxFrameSelector->currentIndex()]->setStyleSheet("color: LawnGreen");
		else 
			frameIDVector[i]->setStyleSheet("color: red");
	}*/
	
}

void MainWindow::reloadGLRotateMenu() {
	qDeleteAll(sideRotateMenuLineEditsVector);
	qDeleteAll(sideRotateMenuLabelsVector);
	qDeleteAll(sideRotateMenuButtonsVector);
	qDeleteAll(sideRotateMenuHLayoutsVector);
	qDeleteAll(sideRotateMenuSpinBoxesVector);

	qDeleteAll(rotateMapperVector);

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

	sideLengthMenuHLayoutsExtraVector.clear();
	sideLengthMenuLabelsExtraVector.clear();
	sideLengthMenuButtonsExtraVector.clear();
	sideLengthMenuLineEditsExtraVector.clear();
	sideLengthMenuSpinBoxesExtraVector.clear();

	addGLWidgetRotateMenu();
}

void MainWindow::refreshRotateSideMenu() {
	//numer wybranego okna
	int frameNumber = comboBoxFrameSelector->currentIndex();
	int backgroundID =  imagesListIterator[frameNumber] - 1;

	sideLengthMenuLineEditsExtraVector[0]->setText(QString::number(glWidgetsVector[frameNumber]->modelState[backgroundID][0]));
	sideLengthMenuLineEditsExtraVector[1]->setText(QString::number(glWidgetsVector[frameNumber]->modelState[backgroundID][1]));
	sideLengthMenuLineEditsExtraVector[2]->setText(QString::number(glWidgetsVector[frameNumber]->modelState[backgroundID][2]));

	int rootID = modelHandler.getModelStateID(mainModel, "root", pf::Model3D::axisX);
	for (int i=0; i<sideRotateMenuLineEditsVector.size(); i++)
		sideRotateMenuLineEditsVector[i]->setText(QString::number(glWidgetsVector[frameNumber]->modelState[backgroundID][rootID+i]));

	for (int i = 0; i < glWidgetsVector.size(); i++) {
		if (glWidgetsVector[i]->saveModelState[imagesListIterator[i] - 1] == true)
			frameIDVector[i]->setStyleSheet("color: LawnGreen");
		else
			frameIDVector[i]->setStyleSheet("color: red");
	}
}

void MainWindow::refreshLengthSideMenu() {
	for (int i = 0; i < sideLengthMenuLineEditsVector.size(); i++)
		sideLengthMenuLineEditsVector[i]->setText(QString::number(modelHandler.getBoneLength(bonesGeometry, bonesGeometry[i].name)));

	//delete model;
}

void MainWindow::refreshGeometrySideMenu() {
	//numer wybranego okna
	int frameNumber = comboBoxFrameSelector->currentIndex();

	for (int i = 0; i < sideLengthMenuLineEditsVector.size(); i++) {
		sideGeometryMenuLineEditsVector[i*3]->setText(QString::number(modelHandler.getBoneLength(bonesGeometry, bonesGeometry[i].name)));
		sideGeometryMenuLineEditsVector[i*3+1]->setText(QString::number(modelHandler.getBoneTopRadius(bonesGeometry, bonesGeometry[i].name)));
		sideGeometryMenuLineEditsVector[i*3+2]->setText(QString::number(modelHandler.getBoneBottomRadius(bonesGeometry, bonesGeometry[i].name)));
	}
}

void MainWindow::addGLWidgetRotateMenu() {
	int rotateMapperCounter = 0;
	vboxRotateLayout->setAlignment(Qt::AlignTop);

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

	for (int lineEdits = 0; lineEdits < sideRotateMenuLineEditsVector.size(); lineEdits++)
		sideRotateMenuLineEditsVector[lineEdits]->setReadOnly(true);

	for (int hLayouts = 0; hLayouts < sideRotateMenuHLayoutsVector.size(); hLayouts++)
		vboxRotateLayout->addLayout(sideRotateMenuHLayoutsVector[hLayouts]);

	disableRotatingButtons();

	refreshRotateSideMenu();
	justLaunched = false;
}

void MainWindow::addGLWidgetLengthMenu() {
	int lengthMapperCounter = 0;
	vboxLengthLayout->setAlignment(Qt::AlignTop);

	/*golden ratio*/
	goldenRatioLayout = new QHBoxLayout();
	setGoldenRatioButton = new QPushButton("Set golden ratio", this);
	setGoldenRatioButton->setToolTip(QString::number(GOLDEN_RATIO, 'g', 11));
	setGoldenRatioCheckBox = new QCheckBox(this);

	/*readonly checkbox*/
	setGoldenRatioCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents);
	setGoldenRatioCheckBox->setFocusPolicy(Qt::NoFocus);

	goldenRatioLayout->addWidget(setGoldenRatioButton);
	goldenRatioLayout->addWidget(setGoldenRatioCheckBox);
	goldenRatioLayout->setAlignment(Qt::AlignRight);
	setGoldenRatioCheckBox->setStyleSheet("padding-right: 10px");

	connect(setGoldenRatioButton, SIGNAL(released()), this, SLOT(setGoldenRatio()));

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

	for (int lineEdits = 0; lineEdits < sideLengthMenuLineEditsVector.size(); lineEdits++)
		sideLengthMenuLineEditsVector[lineEdits]->setReadOnly(true);

	vboxLengthLayout->addLayout(goldenRatioLayout);

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

	//vector<pf::boneGeometry>().swap(usedBones);

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
		//kazda z kosci moze byc obracana w 3 osiach w 2 strony
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

	for (int lineEdits = 0; lineEdits < sideGeometryMenuLineEditsVector.size(); lineEdits++)
		sideGeometryMenuLineEditsVector[lineEdits]->setReadOnly(true);

	for (int hLayouts = 0; hLayouts < sideGeometryMenuHLayoutsVector.size(); hLayouts++)
		vboxGeometryLayout->addLayout(sideGeometryMenuHLayoutsVector[hLayouts]);

	refreshGeometrySideMenu();
}

void MainWindow::addFrameScrollingMenu(QPushButton *button, int id) {
	frameScrollingMenu.push_back(new QMenu());
	selectBackgroundMenuVector.push_back(new QMenu("Add background"));
	selectCameraMenuVector.push_back(new QMenu("Select camera"));

	selectImagesBackgroundVector.push_back(new QAction("Load images"));
	selectAviBackgroundVector.push_back(new QAction("Load avi"));

	/*###*/
	deleteFrameMenuVector.push_back(new QAction("Delete frame"));

	selectBackgroundMenuVector[id]->addAction(selectImagesBackgroundVector[id]);
	selectBackgroundMenuVector[id]->addAction(selectAviBackgroundVector[id]);

	selectCameraMenuVector[id]->setDisabled(true);

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
	int frameID = comboBoxFrameSelector->currentIndex();
	for (int i = 0; i < glWidgetsVector.size(); i++) {
		for (int j = 0; j < glWidgetsVector[i]->modelState.size(); j++) {
			glWidgetsVector[i]->saveModelStateToMap(glWidgetsVector[i]->bonesRotations[j], glWidgetsVector[i]->modelState[j], bonesConf);
		}
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

	modelHandler.updateBonesLengthFromMap(bonesLength, bonesGeometry);
	modelHandler.setLimitsVector(limits, bonesConf, bonesGeometry);
	modelHandler.setVelocityVector(velocity, bonesConf, bonesGeometry);

	FileHandler *fileHandler = new FileHandler();

	for (int i = 0; i < glWidgetsVector.size(); i++) {
		fileHandler->reloadParams(glWidgetsVector[i], bonesConf, bonesGeometry);
	}

	for (int i = 0; i < glWidgetsVector.size(); i++) {
		//modelHandler.updateBonesLengthFromMap(bonesLength, glWidgetsVector[i]->bonesGeometry);

		for (int j = 0; j < bonesGeometry.size(); j++) {
			modelHandler.updateLength(glWidgetsVector[i]->model, glWidgetsVector[i]->bonesGeometry, bonesGeometry[j].name, 1.0, 0);

			glWidgetsVector[i]->update();
		}
	}
	reloadGLRotateMenu();
	delete fileHandler;
}

void MainWindow::updateBoneConfigFromLimitsSB() {
	int frameID = comboBoxFrameSelector->currentIndex();

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

		/*for (int i = 0; i < glWidgetsVector.size(); i++) {
			//modelHandler.updateBonesLengthFromMap(bonesLength, glWidgetsVector[i]->bonesGeometry);

			for (int j = 0; j < bonesGeometry.size(); j++) {
				modelHandler.updateLength(glWidgetsVector[i]->model, glWidgetsVector[i]->bonesGeometry, bonesGeometry[j].name, 1.0, 0);

				glWidgetsVector[i]->update();
			}

		}*/
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
	showModelInDialogMapperVector.push_back(new QSignalMapper(this));
	setConfigFromModelMapperVector.push_back(new QSignalMapper(this));
	setConfigToAllModelsMapperVector.push_back(new QSignalMapper(this));
	addImagesBackgroundMapperVector.push_back(new QSignalMapper(this));
	deleteFrameMapperVector.push_back(new QSignalMapper(this));
	addAviMapperVector.push_back(new QSignalMapper(this));
	sliderMapperVector.push_back(new QSignalMapper(this));

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
			config.setValue("use_quat", QString::number(0));
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

	}
}

void MainWindow::updateModelAfterCameraComboBoxChanged() {
	Qt::CheckState state = sideLengthMenuCameraCheckBox->checkState();
	if (state == Qt::Checked) {
		sideLengthMenuCameraCheckBox->setChecked(false);
		sideLengthMenuCameraCheckBox->setChecked(true);
	}
	else {
		sideLengthMenuCameraCheckBox->setChecked(true);
		sideLengthMenuCameraCheckBox->setChecked(false);
	}
}

void MainWindow::deleteModelInDialog() {
	scrollArea->setDisabled(false);
}

void MainWindow::updateSliders() {
	//numer wybranego okna
	int frameNumber = comboBoxFrameSelector->currentIndex();
	int backgroundID = imagesListIterator[frameNumber] - 1;

	for (int i = 0; i < glWidgetsVector.size(); i++) {
		if (sliderCheckBoxesVector[i]->isChecked()) {
			if (globalSlider->value() > slidersVector[i]->maximum())
				slidersVector[i]->setValue(slidersVector[i]->maximum());
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
			glWidgetsVector[i]->setGLWidgetModelState(imagesListIterator[i] - 1);
			glWidgetsVector[i]->rotate("root", 1.0, pf::Model3D::axisX, 0, bonesConf, limits, imagesListIterator[i] - 1);
			glWidgetsVector[i]->update();
		}

		if (glWidgetsVector[i]->saveModelState[imagesListIterator[i] - 1] == true)
			frameIDVector[i]->setStyleSheet("color: LawnGreen");
		else
			frameIDVector[i]->setStyleSheet("color: red");
	
	}
	
	int rootID = modelHandler.getModelStateID(mainModel, "root", pf::Model3D::axisX);
	for (int i = 0; i < sideRotateMenuLineEditsVector.size(); i++)
		sideRotateMenuLineEditsVector[i]->setText(QString::number(glWidgetsVector[frameNumber]->modelState[backgroundID][rootID + i]));
	
}

void MainWindow::playBackground(int i) {
	
	if (playPressedVector[i] == true) {
		increaseButtonsVector[i]->click();

		if (glWidgetsVector[i]->isAvi) {
			if (imagesListIterator[i] >= glWidgetsVector[i]->aviFrames.size()) {
				playPauseButtonsVector[i]->click();
			}
		}
		else {
			if (imagesListIterator[i] >= glWidgetsVector[i]->list.size()) {
				playPauseButtonsVector[i]->click();
			}
		}
	}
}

void MainWindow::saveModelStateToBoolVector() {
	int id = comboBoxFrameSelector->currentIndex();
	glWidgetsVector[id]->saveModelState[imagesListIterator[id] - 1] = true;

	frameIDVector[id]->setStyleSheet("color: LawnGreen");
}

void MainWindow::setCamerasMenu() {
	//qDeleteAll(selectCameraActionVector);
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
		glWidgetsVector[glWidgetID]->rotate("root", 1.0, pf::Model3D::axisX, 0, bonesConf, limits, 0);
	}
	else {
		cout << "Camera cast error" << endl;
	}
}

void MainWindow::setGoldenRatio() {
	modelHandler.saveBonesLengthToMap(bonesLength, bonesGeometry);
	modelHandler.setGoldenRatio(bonesLength);

	modelHandler.updateBonesLengthFromMap(bonesLength, bonesGeometry);
	bool containsSpine1 = false;
	bool containsSpine1_dum1 = false;

	/*sprawdzenie czy model zawiera spine1 oraz spine1_dum1 - potrzebne do rysowania modelu*/
	for (int i = 0; i < bonesGeometry.size(); i++) {
		cout << bonesGeometry[i].name << endl;
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
			/*cout << spine1Length << endl;
			cout << spine1_dum1Length << endl;

			cout << bonesLength.find("Spine1_dum1")->second << endl;
			cout << bonesLength.find("Spine1")->second << endl;
			*/
			modelHandler.updateBonesLengthFromMap(bonesLength, bonesGeometry);
	}

	for (int i = 0; i < glWidgetsVector.size(); i++) {
		modelHandler.updateBonesLengthFromMap(bonesLength, glWidgetsVector[i]->bonesGeometry);

		for (int j = 0; j < bonesGeometry.size(); j++) {
			modelHandler.updateLength(glWidgetsVector[i]->model, glWidgetsVector[i]->bonesGeometry, bonesGeometry[j].name, 1.0, 0);

			glWidgetsVector[i]->update();
		}

	}
	setGoldenRatioCheckBox->setChecked(true);
	refreshLengthSideMenu();
}