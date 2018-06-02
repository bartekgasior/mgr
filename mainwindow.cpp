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
	addFrameButton->setText(QString("Dodaj okno"));
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

	plikMenu = menuBar->addMenu("&Plik");
	plikMenu->addSeparator();

	loadMenu = plikMenu->addMenu(tr("&Wczytaj"));

	loadFromAmcAction = new QAction("Wczytaj plik .amc", this);
	connect(loadFromAmcAction, &QAction::triggered, this, &MainWindow::loadFromAmc);
	loadMenu->addAction(loadFromAmcAction);

	/*########################*/

	saveMenu = plikMenu->addMenu(tr("&Zapisz"));

	saveAmcAction = new QAction("Zapisz klatkę", this);
	connect(saveAmcAction, &QAction::triggered, this, &MainWindow::saveOneFrameToFile);
	saveMenu->addAction(saveAmcAction);
	if (comboBoxFrameSelector->count() == 0)
		saveAmcAction->setDisabled(true);

	saveAmcSequenceAction = new QAction("Zapisz sekwencje amc", this);
	connect(saveAmcSequenceAction, &QAction::triggered, this, &MainWindow::saveSequenceToFile);
	saveMenu->addAction(saveAmcSequenceAction);
	if (comboBoxFrameSelector->count() < 2)
		saveAmcSequenceAction->setDisabled(true);

	plikMenu->addSeparator();

	manageModelBonesAction = new QAction("Zarządzaj koścmi", this);
	connect(manageModelBonesAction, &QAction::triggered, this, &MainWindow::manageBonesPressed);
	plikMenu->addAction(manageModelBonesAction);
	if (comboBoxFrameSelector->count() == 0)
		manageModelBonesAction->setDisabled(true);

	/******************************/
	cameraMenu = menuBar->addMenu("&Camera");
	openCameraConfigAction = new QAction(tr("&Konfiguracja kamery"));
	connect(openCameraConfigAction, &QAction::triggered, this, &MainWindow::openCameraConfigPressed);
	cameraMenu->addAction(openCameraConfigAction);
}

void MainWindow::prepareLayouts() {
	globalSlider = new QSlider;
	globalSlider->setVisible(false);
	connect(globalSlider, SIGNAL(valueChanged(int)), this, SLOT(updateGlobalListIterator(int)));

	addFrameButton = new QPushButton;
	addFrameButton->setVisible(false);
	connect(addFrameButton, SIGNAL(released()), this, SLOT(buttonAddFramePressed()));

	comboBoxFrameSelector = new QComboBox;
	connect(comboBoxFrameSelector, SIGNAL(activated(int)), this, SLOT(reloadGLWidgetMenu()));

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

	selectFrameLabel->setText("Wybierz okno");
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
	mainModel = new pf::Model3D(pf::Model3D::Cylinder, ASF_TEMPLATE_PATH, DAT_TEMPLATE_PATH);
	limits = mainModel->getLimits();
	usedBones = mainModel->getNamesMovingBones();
	allBones = mainModel->getNamesBones();
	pf::MotionData::loadASF(ASF_TEMPLATE_PATH, idxBonesMap, asfBones);
	mainModel->loadConfig(DAT_TEMPLATE_PATH, bonesConf, bonesGeometry);
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

	globalListIterator = 1;
	addFrameButtonCounter = 0;

	showLayouts();
}

void MainWindow::hideLayouts() {
	windowLayout->hide();
}

void MainWindow::showLayouts() {
	windowLayout->show();
}

void MainWindow::saveOneFrameToFile() {
	FileHandler * fileHandler = new FileHandler();
	int id = comboBoxFrameSelector->currentIndex();

	QString folderPath = QFileDialog::getExistingDirectory(this, tr("Model files folder"));
	if (!folderPath.isEmpty()) {

		/*sciezki plikow*/
		QString amcFileName;
		QString asfFileName;
		QString datFileName;
		QString helperFile;

		/*scieżka utworzonego folderu zapisywanej klatki*/
		/*czy jest tlo*/
		if (glWidgetsVector[id]->drawBckg) {
			/*czy tlo jest zdjeciem*/
			if (!glWidgetsVector[id]->isAvi) {
				folderPath += ("/" + glWidgetsVector[id]->list[imagesListIterator[id] - 1].baseName());
				amcFileName = folderPath + "/" + glWidgetsVector[id]->list[imagesListIterator[id] - 1].baseName() + ".amc";
				asfFileName = folderPath + "/" + glWidgetsVector[id]->list[imagesListIterator[id] - 1].baseName() + ".asf";
				datFileName = folderPath + "/" + glWidgetsVector[id]->list[imagesListIterator[id] - 1].baseName() + ".dat";
				helperFile = folderPath + "/" + "helperFile.dat";
			}
			/*czy tlo jest klatka avi*/
			else {/*sciezka do zapisu klatki avi*/
				folderPath += ("/" + QFileInfo(glWidgetsVector[id]->aviFilePath).baseName());
				amcFileName = folderPath + "/" + QFileInfo(glWidgetsVector[id]->aviFilePath).baseName() + ".amc";
				asfFileName = folderPath + "/" + QFileInfo(glWidgetsVector[id]->aviFilePath).baseName() + ".asf";
				datFileName = folderPath + "/" + QFileInfo(glWidgetsVector[id]->aviFilePath).baseName() + ".dat";
				helperFile = folderPath + "/" + "helperFile.dat";
			}
		}/*gdy nie ma tla*/
		else {
			folderPath += "/emptyBckg";
			amcFileName = folderPath + "/emptyBckg.amc";
			asfFileName = folderPath + "/emptyBckg.asf";
			datFileName = folderPath + "/emptyBckg.dat";
			helperFile = folderPath + "/helperFile.dat";
		}

		if (QDir(folderPath).exists()) {
			QMessageBox::StandardButton reply;
			reply = QMessageBox::question(this,
				"?",
				"Konfiguracja dla wybranej klatki została wcześniej zapisana. Nadpisać?",
				QMessageBox::Yes | QMessageBox::No);
			if (reply = QMessageBox::Yes) {
				QDir().mkdir(folderPath);

				/*pobranie wykorzystanych kosci z boneConfig*/
				glWidgetsVector[id]->updateUsedBones(glWidgetsVector[id]->usedBones, glWidgetsVector[id]->bonesConf);

				/*zapis pliku amc*/
				fileHandler->saveAMCToFile(
					glWidgetsVector[id]->modelState[0],
					glWidgetsVector[id]->allBones,
					glWidgetsVector[id]->usedBones,
					amcFileName
				);

				/*zapis pliku asf*/
				fileHandler->saveASFToFile(
					glWidgetsVector[id]->asfBones,
					asfFileName
				);

				/*zapis pliku dat*/
				fileHandler->saveDATToFile(
					glWidgetsVector[id]->bonesConf,
					glWidgetsVector[id]->bonesGeometry,
					datFileName
				);

				/*zapis pliku pomocniczego*/
				if (glWidgetsVector[id]->drawBckg) {
					if (!glWidgetsVector[id]->isAvi)
						fileHandler->saveHelperFileFromImagesFolder(glWidgetsVector[id]->list[imagesListIterator[id] - 1].absoluteFilePath(), helperFile);
					else
						fileHandler->saveHelperFileFromAvi(glWidgetsVector[id]->aviFilePath, helperFile, imagesListIterator[id], glWidgetsVector[id]->hz);
				}
				else {
					fileHandler->saveHelperFileWithoutBackground(helperFile);
				}

				QMessageBox::information(NULL, QObject::tr("app"),
					QObject::tr("Zapisano pomyślnie"),
					QMessageBox::Cancel,
					QMessageBox::Cancel);
			}
			else {}
		}
		else {
			QDir().mkdir(folderPath);

			/*pobranie wykorzystanych kosci z boneConfig*/
			glWidgetsVector[id]->updateUsedBones(glWidgetsVector[id]->usedBones, glWidgetsVector[id]->bonesConf);

			/*zapis pliku amc*/
			fileHandler->saveAMCToFile(
				glWidgetsVector[id]->modelState[0],
				glWidgetsVector[id]->allBones,
				glWidgetsVector[id]->usedBones,
				amcFileName
			);

			/*zapis pliku asf*/
			fileHandler->saveASFToFile(
				glWidgetsVector[id]->asfBones,
				asfFileName
			);

			/*zapis pliku dat*/
			fileHandler->saveDATToFile(
				glWidgetsVector[id]->bonesConf,
				glWidgetsVector[id]->bonesGeometry,
				datFileName
			);
			
			/*zapis pliku pomocniczego*/
			if (glWidgetsVector[id]->drawBckg) {
				if (!glWidgetsVector[id]->isAvi)
					fileHandler->saveHelperFileFromImagesFolder(glWidgetsVector[id]->list[imagesListIterator[id] - 1].absoluteFilePath(), helperFile);
				else
					fileHandler->saveHelperFileFromAvi(glWidgetsVector[id]->aviFilePath, helperFile, imagesListIterator[id], glWidgetsVector[id]->hz);
			}
			else {
				fileHandler->saveHelperFileWithoutBackground(helperFile);
			}

			QMessageBox::information(NULL, QObject::tr("app"),
				QObject::tr("Zapisano pomyślnie"),
				QMessageBox::Cancel,
				QMessageBox::Cancel);
		}
	}
	delete fileHandler;
}

void MainWindow::saveSequenceToFile() {
	FileHandler * fileHandler = new FileHandler();
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Zapisz model w formacie .amc"), "",
		tr("AMC (*.amc);;All Files(*)"));
	
	if (!fileName.isEmpty()) {
		/*sciezka do folderu zapisywanej sekwencji*/
		QString folderPath = QFileInfo(fileName).absolutePath();
		folderPath += ("/" + QFileInfo(fileName).baseName());

		/*sciezka do folderu pomocniczego*/
		QString helperFile = folderPath + "/" + "helperFile.dat";

		/*sciezka do pliku amc sekwencji*/
		QString amcFileName = folderPath + "/" + QFileInfo(fileName).baseName() + ".amc";

		QDir().mkdir(folderPath);

			vector<int> framesID; /* wektor indeksów utworzonych obiektow glWidget */
			vector<vector<string>> imagesPath; /* wektor sciezek do plikow tla */

			/*Stany kolejnych modeli*/
			vector<vector<float>> states;
			int frames = comboBoxFrameSelector->count();
			for (int i = 0; i < frames; i++) {
				states.push_back(glWidgetsVector[i]->modelState[0]);
				glWidgetsVector[i]->updateUsedBones(glWidgetsVector[i]->usedBones, glWidgetsVector[i]->bonesConf);
			}

			/*Zapis plików*/
			/*Zapis pliku AMC*/
			fileHandler->saveAMCSeq(
				states,
				asfBones,
				glWidgetsVector[0]->usedBones,
				amcFileName
			);

			vector<QString> asfFiles;
			vector<QString> datFiles;

			for (int i = 0; i < frames; i++) {
				QString asfFileName;
				QString datFileName;
				if (glWidgetsVector[i]->drawBckg) {
					if (!glWidgetsVector[i]->isAvi) {
						asfFileName = folderPath + "/" + glWidgetsVector[i]->list[imagesListIterator[i] - 1].baseName();
						datFileName = folderPath + "/" + glWidgetsVector[i]->list[imagesListIterator[i] - 1].baseName();
					}
					else {
						asfFileName = folderPath + "/" + QFileInfo(glWidgetsVector[i]->aviFilePath).baseName();
						datFileName = folderPath + "/" + QFileInfo(glWidgetsVector[i]->aviFilePath).baseName();
					}
				}
				else {
					asfFileName = folderPath + "/emptyBckg";
					datFileName = folderPath + "/emptyBckg";
				}

				fileHandler->checkExistingFiles(asfFiles, datFiles, asfFileName, datFileName);
			}

			for (int i = 0; i < frames; i++) {
				/*Zapis pliku ASF*/
				fileHandler->saveASFToFile(
					glWidgetsVector[i]->asfBones,
					asfFiles[i]
				);
				/*Zapis pliku DAT*/
				fileHandler->saveDATToFile(
					glWidgetsVector[i]->bonesConf,
					glWidgetsVector[i]->bonesGeometry,
					datFiles[i]
				);
			}

			for (int i = 0; i < comboBoxFrameSelector->count(); i++) {
				vector<string> glWidgetBackground;
				framesID.push_back(i);
				if (glWidgetsVector[i]->drawBckg) {
					if (!glWidgetsVector[i]->isAvi) {
						glWidgetBackground.push_back("img");
						glWidgetBackground.push_back(glWidgetsVector[i]->list[imagesListIterator[i] - 1].absoluteFilePath().toUtf8().constData());
					}
					else {
						glWidgetBackground.push_back("avi");
						glWidgetBackground.push_back(glWidgetsVector[i]->aviFilePath.toUtf8().constData());
						glWidgetBackground.push_back(std::to_string(imagesListIterator[i] - 1));
						glWidgetBackground.push_back(std::to_string(glWidgetsVector[i]->hz));
					}
				}
				else {
					glWidgetBackground.push_back("empty");
					glWidgetBackground.push_back("emptyBckg");
				}
				imagesPath.push_back(glWidgetBackground);
			}
			/*Zapis pliku pomocniczego*/
			fileHandler->saveHelperFile(helperFile, framesID, imagesPath);
			/*Koniec zapisu plików*/

			QMessageBox::information(NULL, QObject::tr("app"),
				QObject::tr("Zapisano pomyślnie"),
				QMessageBox::Cancel,
				QMessageBox::Cancel);
		}
	delete fileHandler;
}

void MainWindow::loadFromAmc() {
	QString amcFilePath = QFileDialog::getOpenFileName(this,
		tr("Wczytaj .amc"), "",
		tr("AMC (*.amc);;All Files(*)"));

	FileHandler * fileHandler = new FileHandler();
	cout << "ASD" << endl;
	/*wektor nazw plików asf*/
	vector<QString> asfFilesPaths;

	/*wektor nazw plików dat*/
	vector<QString> datFilesPaths;

	/*ściezka do pliku pomocniczego*/
	QString helperFilePath;

	if (!amcFilePath.isEmpty()) {
		//for(int i=0;i<glWidgetsVector())

		if (!windowLayout->layout()->isEmpty())
			delete windowLayout->layout();

		prepareLayouts();
		resetVariables();
		fileHandler->getFilesAsQStrings(amcFilePath, asfFilesPaths, datFilesPaths);

		QString folderPath = QFileInfo(amcFilePath).absolutePath();
		helperFilePath = folderPath + "/helperFile.dat";

		//wektor zawierajacy sciezki do zdjec tla kolejnych klatek
		vector<QString> loadedImages = fileHandler->getGlWidgetBackgroudFromFile(helperFilePath);

		//pobranie typow tla -img lub avi
		vector<string> loadedBackgroundTypes = fileHandler->getGlWidgetBackgroudTypeFromFile(helperFilePath);

		/*wektory przechowujace numer klatki oraz wybrana czestotliwosc avi*/
		vector<int> framesID;
		vector<int> aviHz;

		fileHandler->getGlWidgetBackgroudAviInfoFromFile(helperFilePath, framesID, aviHz);

		/*wczytanie pliku amc do wektora stanow*/
		vector<vector<float>> states = fileHandler->loadAmcFromFile(amcFilePath, asfFilesPaths, datFilesPaths);

		if (!states.empty()) {
			int aviCounter = 0;
			addFrameButton->setVisible(true);
			addFrameButton->setText(QString("Dodaj okno"));

			for (int i = 0; i < loadedBackgroundTypes.size(); i++) {
				if (loadedBackgroundTypes[i] == "img") {
					cout << "img" << endl;
					QDir dir(QFileInfo(loadedImages[i]).absolutePath());

					if (dir.exists()) {

						addFrameButton->click();

						QStringList filter;
						filter << QLatin1String("*.png");
						filter << QLatin1String("*.jpg");
						filter << QLatin1String("*.jpeg");
						filter << QLatin1String("*.bmp");
						dir.setNameFilters(filter);

						glWidgetsVector[i]->list = dir.entryInfoList();

						glWidgetsVector[i]->bonesConf.clear();
						glWidgetsVector[i]->bonesGeometry.clear();

						fileHandler->loadDatFromFile(datFilesPaths[i].toUtf8().constData(), glWidgetsVector[i]->bonesConf, glWidgetsVector[i]->bonesGeometry);
						glWidgetsVector[i]->setLimitsVector(glWidgetsVector[i]->limits, glWidgetsVector[i]->bonesConf, glWidgetsVector[i]->bonesGeometry);
						glWidgetsVector[i]->setVelocityVector(glWidgetsVector[i]->velocity, glWidgetsVector[i]->bonesConf, glWidgetsVector[i]->bonesGeometry);
						glWidgetsVector[i]->model = new pf::Model3D(pf::Model3D::Cylinder, asfFilesPaths[i].toUtf8().constData(), datFilesPaths[i].toUtf8().constData());
						glWidgetsVector[i]->modelState[0] = states[i];

						imagesListIterator[i] = fileHandler->getImageID(glWidgetsVector[i]->list, loadedImages[i]);
						setTextIteratorLabel(countersVector[i], imagesListIterator[i], glWidgetsVector[i]->list.size());

						/*tlo zdjecia*/
						//QString imgString = glWidgetsVector[i]->list[imagesListIterator[i] - 1].absoluteFilePath();
						glWidgetsVector[i]->imgPath = loadedImages[i].toUtf8().constData();
						slidersVector[i]->setValue(imagesListIterator[i]);
						prepareSlider(slidersVector[i], i);
						glWidgetsVector[i]->drawBckg = true;
						
						glWidgetsVector[i]->rotate("root", 1.0, pf::Model3D::axisX, 0);
						hMenuWidgetVector[i]->show();
						glWidgetsVector[i]->update();
					}
				}
				else if (loadedBackgroundTypes[i] == "avi") {
					cout << "avi" << endl;
					addFrameButton->click();

					glWidgetsVector[i]->aviFilePath = loadedImages[i];

					glWidgetsVector[i]->loadAviFile(glWidgetsVector[i]->aviFilePath.toUtf8().constData(), aviHz[aviCounter]);

					glWidgetsVector[i]->bonesConf.clear();
					glWidgetsVector[i]->bonesGeometry.clear();

					fileHandler->loadDatFromFile(datFilesPaths[i].toUtf8().constData(), glWidgetsVector[i]->bonesConf, glWidgetsVector[i]->bonesGeometry);
					glWidgetsVector[i]->setLimitsVector(glWidgetsVector[i]->limits, glWidgetsVector[i]->bonesConf, glWidgetsVector[i]->bonesGeometry);
					glWidgetsVector[i]->setVelocityVector(glWidgetsVector[i]->velocity, glWidgetsVector[i]->bonesConf, glWidgetsVector[i]->bonesGeometry);
					glWidgetsVector[i]->model = new pf::Model3D(pf::Model3D::Cylinder, asfFilesPaths[i].toUtf8().constData(), datFilesPaths[i].toUtf8().constData());
					glWidgetsVector[i]->modelState[0] = states[i];
					glWidgetsVector[i]->hz = aviHz[aviCounter];

					imagesListIterator[i] = framesID[aviCounter]+1;
					setTextIteratorLabel(countersVector[i], imagesListIterator[i], glWidgetsVector[i]->aviFrames.size());
					glWidgetsVector[i]->aviFrames[imagesListIterator[i]].copyTo(glWidgetsVector[i]->aviFrame);

					slidersVector[i]->setValue(imagesListIterator[i]);
					prepareSlider(slidersVector[i], i);
					glWidgetsVector[i]->drawBckg = true;

					glWidgetsVector[i]->rotate("root", 1.0, pf::Model3D::axisX, 0);
					hMenuWidgetVector[i]->show();
					glWidgetsVector[i]->update();
					aviCounter++;
				}
				else {
					cout << "empty" << endl;
					addFrameButton->click();

					glWidgetsVector[i]->bonesConf.clear();
					glWidgetsVector[i]->bonesGeometry.clear();

					fileHandler->loadDatFromFile(datFilesPaths[i].toUtf8().constData(), glWidgetsVector[i]->bonesConf, glWidgetsVector[i]->bonesGeometry);
					glWidgetsVector[i]->setLimitsVector(glWidgetsVector[i]->limits, glWidgetsVector[i]->bonesConf, glWidgetsVector[i]->bonesGeometry);
					glWidgetsVector[i]->setVelocityVector(glWidgetsVector[i]->velocity, glWidgetsVector[i]->bonesConf, glWidgetsVector[i]->bonesGeometry);
					glWidgetsVector[i]->model = new pf::Model3D(pf::Model3D::Cylinder, asfFilesPaths[i].toUtf8().constData(), datFilesPaths[i].toUtf8().constData());
					glWidgetsVector[i]->modelState[0] = states[i];

					glWidgetsVector[i]->rotate("root", 1.0, pf::Model3D::axisX, 0);
					glWidgetsVector[i]->update();
				}
			}
		}

		/*odswiezenie wartosci qlineedit*/
		reloadGLWidgetMenu();
		/*refreshRotateSideMenu();
		refreshLengthSideMenu();
		refreshGeometrySideMenu();*/
	}
	delete fileHandler;
}

void MainWindow::buttonAddFramePressed(){
    addFrameMenu(addFrameButtonCounter);
	comboBoxFrameSelector->addItem(tr(std::to_string(addFrameButtonCounter).c_str()));
	if (addFrameButtonCounter == 0) {
		addGLWidgetRotateMenu();
		addGLWidgetLengthMenu();
		addGLWidgetGeometryMenu();
	}
	addFrameButtonCounter++;

}

void MainWindow::addFrameMenu(int i){
    int j=0;
    bordersWidgetsVector.push_back(new QWidget(this));
	frameIDVector.push_back(new QLabel(this));
    glWidgetsVector.push_back(new GLWidget(this));
	horizontalGLWidgetMenuVector.push_back(new QHBoxLayout());
	hMenuWidgetVector.push_back(new QWidget(this));
	slidersVector.push_back(new QSlider(this));
    minButtonsVector.push_back(new QPushButton(this));
    maxButtonsVector.push_back(new QPushButton(this));
    reduceButtonsVector.push_back(new QPushButton(this));
    increaseButtonsVector.push_back(new QPushButton(this));
    countersVector.push_back(new QLabel(this));
    imagesListIterator.push_back(0);
	menuButtonsVector.push_back(new QPushButton("Menu"));

	/*dodanie obslugi przyciskow*/
	addFrameMenuMappers(addFrameButtonCounter);

	/*rozmiar label wyswietlajacy numer obiektu*/
	QFont font = frameIDVector[addFrameButtonCounter]->font();
	font.setPointSize(25);

	bordersWidgetsVector[addFrameButtonCounter]->setStyleSheet("border: 1px solid black");
	frameIDVector[addFrameButtonCounter]->setText(" " + QString::number(addFrameButtonCounter));
	frameIDVector[addFrameButtonCounter]->setFont(font);

	if (i == 0) 
		frameIDVector[addFrameButtonCounter]->setStyleSheet("color: LawnGreen");
	else 
		frameIDVector[addFrameButtonCounter]->setStyleSheet("color: red");

	/*rozmiar przyciskow menu*/
	minButtonsVector[addFrameButtonCounter]->setFixedSize(30,20);
	maxButtonsVector[addFrameButtonCounter]->setFixedSize(30, 20);
	reduceButtonsVector[addFrameButtonCounter]->setFixedSize(30, 20);
	increaseButtonsVector[addFrameButtonCounter]->setFixedSize(30, 20);
	menuButtonsVector[addFrameButtonCounter]->setFixedSize(60, 30);

	/*dolny panel menu - przyciski do iteracji klatek, slider, menubutton*/
	horizontalGLWidgetMenuVector[addFrameButtonCounter]->addWidget(minButtonsVector[addFrameButtonCounter]);
	horizontalGLWidgetMenuVector[addFrameButtonCounter]->addWidget(reduceButtonsVector[addFrameButtonCounter]);
	horizontalGLWidgetMenuVector[addFrameButtonCounter]->addWidget(countersVector[addFrameButtonCounter]);
	horizontalGLWidgetMenuVector[addFrameButtonCounter]->addWidget(increaseButtonsVector[addFrameButtonCounter]);
	horizontalGLWidgetMenuVector[addFrameButtonCounter]->addWidget(maxButtonsVector[addFrameButtonCounter]);
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
        gridChildLayout->addWidget(glWidgetsVector[addFrameButtonCounter], (i/2)*10+j, 0, 10, 10);
		gridChildLayout->addWidget(hMenuWidgetVector[addFrameButtonCounter], (i / 2) * 10 + 11 + j, 0, 1, 9);
		gridChildLayout->addWidget(menuButtonsVector[addFrameButtonCounter], (i/2)*10+11+j, 9, 1, 1);
		gridChildLayout->addWidget(frameIDVector[addFrameButtonCounter], (i / 2) * 10 + j, 0, 2, 2);
    }else {
        if(i>1) j=(i/2)*2;
		gridChildLayout->addWidget(bordersWidgetsVector[addFrameButtonCounter], (i / 2) * 10 + j, 11, 12, 10);
        gridChildLayout->addWidget(glWidgetsVector[addFrameButtonCounter], (i/2)*10+j, 11, 10, 10);
		gridChildLayout->addWidget(hMenuWidgetVector[addFrameButtonCounter], (i / 2) * 10 + 11 + j, 11, 1, 9);
		gridChildLayout->addWidget(menuButtonsVector[addFrameButtonCounter], (i / 2) * 10 + 11 + j, 20, 1, 1);
		gridChildLayout->addWidget(frameIDVector[addFrameButtonCounter], (i / 2) * 10 + j, 11, 2, 2);
    }

	if (comboBoxFrameSelector->count() == 0) {
		saveAmcAction->setEnabled(true);
		manageModelBonesAction->setEnabled(true);
	}

	if (comboBoxFrameSelector->count() == 1)
		saveAmcSequenceAction->setEnabled(true);


	/*ustawienie kosci dodanego modelu*/
	FileHandler *fileHandler = new FileHandler();
	fileHandler->reloadParams(glWidgetsVector[addFrameButtonCounter]);

	glWidgetsVector[addFrameButtonCounter]->limits = limits;
	glWidgetsVector[addFrameButtonCounter]->usedBones = usedBones;
	glWidgetsVector[addFrameButtonCounter]->allBones = allBones;
	glWidgetsVector[addFrameButtonCounter]->asfBones = asfBones;
	glWidgetsVector[addFrameButtonCounter]->idxBonesMap = idxBonesMap;

	update();
	delete fileHandler;
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

	glWidgetsVector[i]->update();
}

void MainWindow::manageBonesPressed() {
	BoneManagment *boneManagmentWindow = new BoneManagment(this);

	boneManagmentWindow->setWindowTitle("Zarządzaj modelem");

	vector<int> usedBonesIDs = boneManagmentWindow->getUsedBonesIDs(bonesGeometry, allBones);
	boneManagmentWindow->setUsedBones(asfBones, usedBonesIDs);
	
	boneManagmentWindow->resize(500, 400);
	boneManagmentWindow->exec();

	vector<int> bonesIDs = boneManagmentWindow->selectedBonesIDs;
	vector<string> bonesNames = boneManagmentWindow->selectedBonesNames;

	if (!bonesIDs.empty() && !bonesNames.empty()) {

		FileHandler *fileHandler = new FileHandler();
		/*aktualizacja map*/
		for (int i = 0; i < glWidgetsVector.size(); i++) {
			glWidgetsVector[i]->saveModelStateToMap(glWidgetsVector[i]->bonesRotations, glWidgetsVector[i]->modelState, glWidgetsVector[i]->bonesConf);
			glWidgetsVector[i]->saveBonesLengthToMap(glWidgetsVector[i]->bonesLength, glWidgetsVector[i]->bonesGeometry);
			glWidgetsVector[i]->saveRadiusToMap(glWidgetsVector[i]->modelRadius, glWidgetsVector[i]->bonesGeometry);
			glWidgetsVector[i]->saveDOFToMap(glWidgetsVector[i]->modelDOF, glWidgetsVector[i]->bonesConf);
			glWidgetsVector[i]->saveLimitsToMap(glWidgetsVector[i]->modelLimits, glWidgetsVector[i]->bonesConf);
			glWidgetsVector[i]->saveVelocityToMap(glWidgetsVector[i]->modelVelocity, glWidgetsVector[i]->bonesConf);
		}
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
					bGeo.length = glWidgetsVector[i]->bonesLength.find(bGeo.name)->second;
					glWidgetsVector[i]->model->updateBoneGeometry(bGeo.name, bGeo);
				}
				else {}
			}
		}

		/*aktualizacja model state*/
		for (int i = 0; i < glWidgetsVector.size(); i++) {
			glWidgetsVector[i]->updateModelStateFromMap(glWidgetsVector[i]->modelState, glWidgetsVector[i]->bonesRotations, glWidgetsVector[i]->bonesConf);
			glWidgetsVector[i]->model->updateModelState(glWidgetsVector[i]->modelState[0]);
			glWidgetsVector[i]->updateBonesLengthFromMap(glWidgetsVector[i]->bonesLength);
			glWidgetsVector[i]->updateBonesRadiusFromMap(glWidgetsVector[i]->modelRadius);
			glWidgetsVector[i]->updateDOFFromMap(glWidgetsVector[i]->modelDOF, glWidgetsVector[i]->bonesConf);
			glWidgetsVector[i]->updateLimitsFromMap(glWidgetsVector[i]->modelLimits, glWidgetsVector[i]->bonesConf);
			glWidgetsVector[i]->updateVelocityFromMap(glWidgetsVector[i]->modelVelocity, glWidgetsVector[i]->bonesConf);
			//glWidgetsVector[i]->setLimitsVector(glWidgetsVector[i]->limits, glWidgetsVector[i]->bonesConf, glWidgetsVector[i]->bonesGeometry);
		}

		reloadGLWidgetMenu();
		updateBoneConfigFromCB();
		updateBoneConfigFromLimitsSB();
		updateBoneConfigFromVelocitySB();
		delete fileHandler;
	}
	delete boneManagmentWindow;
}

void MainWindow::openCameraConfigPressed() {
	CamerasConfig *cfg = new CamerasConfig(this);

	cfg->resize(500, 400);
	cfg->exec();

	delete cfg;
}

void MainWindow::showModelInDialogPressed(int i) {
	ModelInDialog *modelInDialog = new ModelInDialog(this);

	modelInDialog->setWindowTitle("model -" + i);

	modelInDialog->resize(500, 400);
	modelInDialog->setGLWidget(glWidgetsVector[i]);
	modelInDialog->exec();

	delete modelInDialog;
}

void MainWindow::setConfigFromModelPressed(int i) {
	SelectModelID *selectCfg = new SelectModelID(this);
	selectCfg->setID(i);

	selectCfg->resize(100, 100);
	vector<int> ids;
	for (int j = 0; j < glWidgetsVector.size(); j++) {
		ids.push_back(j);
	}

	selectCfg->setComboBox(ids, i);
	selectCfg->exec();

	int selectedID = selectCfg->getID();
	glWidgetsVector[i]->copyConfigToGlWidget(glWidgetsVector[i], glWidgetsVector[selectedID]);

	/*zmiena dlugosci kosci aktualizuje dlugosci kosci wyswietlane na ekranie*/
	if (glWidgetsVector[i]->bonesGeometry.size() != 0) 
		glWidgetsVector[i]->updateLength(glWidgetsVector[i]->bonesGeometry[0].name, 1.0, 0);

	/*rotacja modelu o 0 stopni aktualizuje obrot na ekranie */
	if (glWidgetsVector[i]->bonesConf.size() != 0) 
		glWidgetsVector[i]->rotate(glWidgetsVector[i]->bonesConf[0].name, 1.0, pf::Model3D::axisX, 0);

	glWidgetsVector[i]->update();

	delete selectCfg;
	refreshRotateSideMenu();
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
				if (glWidgetsVector[j]->bonesGeometry.size() != 0)
					glWidgetsVector[j]->updateLength(glWidgetsVector[i]->bonesGeometry[0].name, 1.0, 0);

				/*rotacja modelu o 0 stopni aktualizuje obrot na ekranie */
				if (glWidgetsVector[j]->bonesConf.size() != 0)
					glWidgetsVector[j]->rotate(glWidgetsVector[i]->bonesConf[0].name, 1.0, pf::Model3D::axisX, 0);

			}
		}
	}
	else {
		cout << "no clicked" << endl;
	}
}

void MainWindow::selectAviBackgroundPressed(int i) {
	glWidgetsVector[i]->loadAviFile();
	setTextIteratorLabel(countersVector[i], imagesListIterator[i], glWidgetsVector[i]->aviFrames.size());
	glWidgetsVector[i]->aviFrames[imagesListIterator[i]-1].copyTo(glWidgetsVector[i]->aviFrame);
	glWidgetsVector[i]->drawBckg = true;

	prepareSlider(slidersVector[i], i);

	hMenuWidgetVector[i]->show();
}

void MainWindow::selectImagesBackgroundPressed(int i) {
	glWidgetsVector[i]->loadFiles();
	setTextIteratorLabel(countersVector[i], imagesListIterator[i], glWidgetsVector[i]->list.size());
	glWidgetsVector[i]->imgPath = glWidgetsVector[i]->list[imagesListIterator[i]-1].absoluteFilePath().toUtf8().constData();
	glWidgetsVector[i]->drawBckg = true;

	prepareSlider(slidersVector[i], i);

	hMenuWidgetVector[i]->show();
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

void MainWindow::mapManageBonesSlot(int i) {
}

void MainWindow::mapShowModelInDialog(int i) {
	QObject::connect(showModelInDialogMapperVector[i], SIGNAL(mapped(int)), this, SLOT(showModelInDialogPressed(int)));
	showModelInDialogMapperVector[i]->setMapping(showModelInDialogVector[i], i);
	connect(showModelInDialogVector[i], SIGNAL(triggered()), showModelInDialogMapperVector[i], SLOT(map()));
}

void MainWindow::mapSetConfigFromModel(int i) {
	QObject::connect(setConfigFromModelMapperVector[i], SIGNAL(mapped(int)), this, SLOT(setConfigFromModelPressed(int)));
	setConfigFromModelMapperVector[i]->setMapping(setConfigFromModelVector[i], i);
	connect(setConfigFromModelVector[i], SIGNAL(triggered()), setConfigFromModelMapperVector[i], SLOT(map()));
}

void MainWindow::mapSetConfigToAllModels(int i) {
	QObject::connect(setConfigToAllModelsMapperVector[i], SIGNAL(mapped(int)), this, SLOT(setConfigToAllModelsPressed(int)));
	setConfigToAllModelsMapperVector[i]->setMapping(setConfigToAllModelsVector[i], i);
	connect(setConfigToAllModelsVector[i], SIGNAL(triggered()), setConfigToAllModelsMapperVector[i], SLOT(map()));
}

void MainWindow::mapAddImagesToBackground(int i) {
	QObject::connect(addImagesBackgroundMapperVector[i], SIGNAL(mapped(int)), this, SLOT(selectImagesBackgroundPressed(int)));
	addImagesBackgroundMapperVector[i]->setMapping(selectImagesBackgroundVector[i], i);
	connect(selectImagesBackgroundVector[i], SIGNAL(triggered()), addImagesBackgroundMapperVector[i], SLOT(map()));
}

void MainWindow::mapAddAviToBackground(int i) {
	QObject::connect(addAviMapperVector[i], SIGNAL(mapped(int)), this, SLOT(selectAviBackgroundPressed(int)));
	addAviMapperVector[i]->setMapping(selectAviBackgroundVector[i], i);
	connect(selectAviBackgroundVector[i], SIGNAL(triggered()), addAviMapperVector[i], SLOT(map()));
}

void MainWindow::updateGlobalListIterator(int value){
    globalListIterator = value;
    /*for(int i=0;i<imagesListIterator.size();i++){
        if(checkBoxesVector[i]->isChecked()){
            imagesListIterator[i]=value;

			if (!isAvi) {
				setTextIteratorLabel(countersVector[i], value, list.size());
				QString imgString = list[imagesListIterator[i] - 1].absoluteFilePath();
				glWidgetsVector[i]->imgPath = imgString.toUtf8().constData();
			}

			else {
				setTextIteratorLabel(countersVector[i], value, aviFrames.size());
				cout << imagesListIterator[i] - 1 << endl;
				glWidgetsVector[i]->aviFrame = aviFrames[imagesListIterator[i] - 1];
			}
            
			glWidgetsVector[i]->update();
        }
    }*/
    update();
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
	glWidgetsVector[i]->update();
}

void MainWindow::prepareFrameMenu(int i) {
	imagesListIterator[i] = globalListIterator;

	mapIncreaseButtonSlot(i);
	mapReduceButtonSlot(i);
	mapMaxButtonSlot(i);
	mapMinButtonSlot(i);
	//mapManageBonesSlot(i);
	mapShowModelInDialog(i);
	mapAddAviToBackground(i);
	mapAddImagesToBackground(i);
	mapSetConfigFromModel(i);
	mapSetConfigToAllModels(i);
}

void MainWindow::prepareSlider(QSlider *slider, int i) {
	slider->setOrientation(Qt::Horizontal);
	if(!glWidgetsVector[i]->isAvi)
		slider->setMaximum(glWidgetsVector[i]->list.size());
	else 
		slider->setMaximum(glWidgetsVector[i]->aviFrames.size());

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

		if (axis == "X")
			glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate(boneName, direction, pf::Model3D::axisX, sideRotateMenuSpinBoxesVector[qSpinBoxID]->value());
		if (axis == "Y")
			glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate(boneName, direction, pf::Model3D::axisY, sideRotateMenuSpinBoxesVector[qSpinBoxID]->value());
		if (axis == "Z")
			glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate(boneName, direction, pf::Model3D::axisZ, sideRotateMenuSpinBoxesVector[qSpinBoxID]->value());

		sideRotateMenuLineEditsVector[qLineEditID]->setText(QString::number(glWidgetsVector[comboBoxFrameSelector->currentIndex()]->modelState[0][qLineEditID+3]));
	}

	else {
		//Okno błedu
		cout << "BLAD" << endl;
	}
}

void MainWindow::updateBoneLength(QString str) {
	QStringList values = str.split(";");
	if (values.count() == 4) {
		string boneName = values.at(0).toStdString();
		float direction = values.at(1).toFloat();
		int qLineEditID = values.at(2).toInt();
		int qSpinBoxID = values.at(3).toInt();

		glWidgetsVector[comboBoxFrameSelector->currentIndex()]->updateLength(boneName, direction, sideLengthMenuSpinBoxesVector[qSpinBoxID]->value());

		int frameNumber = comboBoxFrameSelector->currentIndex();
		sideLengthMenuLineEditsVector[qLineEditID]->setText(QString::number(glWidgetsVector[frameNumber]->getBoneLength(glWidgetsVector[frameNumber]->bonesGeometry[qLineEditID].name)));
		sideGeometryMenuLineEditsVector[qLineEditID*3]->setText(QString::number(glWidgetsVector[frameNumber]->getBoneLength(glWidgetsVector[frameNumber]->bonesGeometry[qLineEditID].name)));
	}
	else {
		//Okno błedu
		cout << "BLAD" << endl;
	}
}

void MainWindow::updateBoneLengthGeometry(QString str) {
	QStringList values = str.split(";");
	if (values.count() == 4) {
		string boneName = values.at(0).toStdString();
		float direction = values.at(1).toFloat();
		int qLineEditID = values.at(2).toInt();
		int qSpinBoxID = values.at(3).toInt();

		glWidgetsVector[comboBoxFrameSelector->currentIndex()]->updateLength(boneName, direction, sideGeometryMenuSpinBoxesVector[qSpinBoxID]->value());

		int frameNumber = comboBoxFrameSelector->currentIndex();
		sideGeometryMenuLineEditsVector[qLineEditID]->setText(QString::number(glWidgetsVector[frameNumber]->getBoneLength(glWidgetsVector[frameNumber]->bonesGeometry[qLineEditID/3].name)));
		sideLengthMenuLineEditsVector[qLineEditID/3]->setText(QString::number(glWidgetsVector[frameNumber]->getBoneLength(glWidgetsVector[frameNumber]->bonesGeometry[qLineEditID/3].name)));
	}
	else {
		//Okno błedu
		cout << "BLAD" << endl;
	}
}

void MainWindow::updateBoneTopRadius(QString str) {
	QStringList values = str.split(";");
	if (values.count() == 4) {
		string boneName = values.at(0).toStdString();
		float direction = values.at(1).toFloat();
		int qLineEditID = values.at(2).toInt();
		int qSpinBoxID = values.at(3).toInt();

		glWidgetsVector[comboBoxFrameSelector->currentIndex()]->updateTopRadius(boneName, direction, sideGeometryMenuSpinBoxesVector[qSpinBoxID]->value());

		int frameNumber = comboBoxFrameSelector->currentIndex();
		sideGeometryMenuLineEditsVector[qLineEditID]->setText(QString::number(glWidgetsVector[frameNumber]->getBoneTopRadius(glWidgetsVector[frameNumber]->bonesGeometry[qLineEditID/3].name)));

		update();
	}
	else {
		//Okno błedu
		cout << "BLAD" << endl;
	}
}

void MainWindow::updateBoneBottomRadius(QString str) {
	QStringList values = str.split(";");
	if (values.count() == 4) {
		string boneName = values.at(0).toStdString();
		float direction = values.at(1).toFloat();
		int qLineEditID = values.at(2).toInt();
		int qSpinBoxID = values.at(3).toInt();

		glWidgetsVector[comboBoxFrameSelector->currentIndex()]->updateBottomRadius(boneName, direction, sideGeometryMenuSpinBoxesVector[qSpinBoxID]->value());
		//cout << sideGeometryMenuSpinBoxesVector[qSpinBoxID]->value() << endl;
		int frameNumber = comboBoxFrameSelector->currentIndex();
		sideGeometryMenuLineEditsVector[qLineEditID]->setText(QString::number(glWidgetsVector[frameNumber]->getBoneBottomRadius(glWidgetsVector[frameNumber]->bonesGeometry[qLineEditID/3].name)));
	
		update();
	}
	else {
		//Okno błedu
		cout << "BLAD" << endl;
	}
}

void MainWindow::translate(QString str) {
	QStringList values = str.split(";");
	if (values.count() == 4) {
		float direction = values.at(0).toFloat();
		string axis = values.at(1).toStdString();
		int qLineEditID = values.at(2).toInt();
		int qSpinBoxID = values.at(3).toInt();

		if (axis == "X")
			glWidgetsVector[comboBoxFrameSelector->currentIndex()]->translate(direction, pf::Model3D::axisX, sideLengthMenuSpinBoxesExtraVector[qSpinBoxID]->value());
		if (axis == "Y")
			glWidgetsVector[comboBoxFrameSelector->currentIndex()]->translate(direction, pf::Model3D::axisY, sideLengthMenuSpinBoxesExtraVector[qSpinBoxID]->value());
		if (axis == "Z")
			glWidgetsVector[comboBoxFrameSelector->currentIndex()]->translate(direction, pf::Model3D::axisZ, sideLengthMenuSpinBoxesExtraVector[qSpinBoxID]->value());

		sideLengthMenuLineEditsExtraVector[qLineEditID]->setText(QString::number(glWidgetsVector[comboBoxFrameSelector->currentIndex()]->modelState[0][qLineEditID]));
	}
	else {
		//Okno błedu
		cout << "BLAD" << endl;
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
		cout << "BLAD" << endl;
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

	for (int i = 0; i < comboBoxFrameSelector->count(); i++) {
		if(comboBoxFrameSelector->itemText(i).toInt() == comboBoxFrameSelector->currentIndex())
			frameIDVector[comboBoxFrameSelector->currentIndex()]->setStyleSheet("color: LawnGreen");
		else 
			frameIDVector[i]->setStyleSheet("color: red");
	}
	
}

void MainWindow::reloadGLRotateMenu() {
	qDeleteAll(sideRotateMenuLineEditsVector);
	qDeleteAll(sideRotateMenuLabelsVector);
	qDeleteAll(sideRotateMenuButtonsVector);
	qDeleteAll(sideRotateMenuHLayoutsVector);
	qDeleteAll(sideRotateMenuSpinBoxesVector);

	qDeleteAll(rotateMapperVector);

	sideRotateMenuLineEditsVector.clear();
	sideRotateMenuLabelsVector.clear();
	sideRotateMenuButtonsVector.clear();
	sideRotateMenuHLayoutsVector.clear();
	sideRotateMenuSpinBoxesVector.clear();

	rotateMapperVector.clear();

	addGLWidgetRotateMenu();
}

void MainWindow::refreshRotateSideMenu() {
	//numer wybranego okna
	int frameNumber = comboBoxFrameSelector->currentIndex();

	//pf::Model3D *modelTMP = glWidgetsVector[frameNumber]->model;
	int rootID = glWidgetsVector[frameNumber]->getModelStateID(glWidgetsVector[frameNumber]->model, "root", pf::Model3D::axisX);
	for (int i=0; i<sideRotateMenuLineEditsVector.size(); i++)
		sideRotateMenuLineEditsVector[i]->setText(QString::number(glWidgetsVector[frameNumber]->modelState[0][rootID+i]));

	//delete modelTMP;
}

void MainWindow::refreshLengthSideMenu() {
	//numer wybranego okna
	int frameNumber = comboBoxFrameSelector->currentIndex();

	//pf::Model3D *model = glWidgetsVector[frameNumber]->model;

	sideLengthMenuLineEditsExtraVector[0]->setText(QString::number(glWidgetsVector[frameNumber]->modelState[0][0]));
	sideLengthMenuLineEditsExtraVector[1]->setText(QString::number(glWidgetsVector[frameNumber]->modelState[0][1]));

	for (int i = 0; i < sideLengthMenuLineEditsVector.size(); i++)
		sideLengthMenuLineEditsVector[i]->setText(QString::number(glWidgetsVector[frameNumber]->getBoneLength(glWidgetsVector[frameNumber]->bonesGeometry[i].name)));

	//delete model;
}

void MainWindow::refreshGeometrySideMenu() {
	//numer wybranego okna
	int frameNumber = comboBoxFrameSelector->currentIndex();

	for (int i = 0; i < sideLengthMenuLineEditsVector.size(); i++) {
		sideGeometryMenuLineEditsVector[i*3]->setText(QString::number(glWidgetsVector[frameNumber]->getBoneLength(glWidgetsVector[frameNumber]->bonesGeometry[i].name)));
		sideGeometryMenuLineEditsVector[i*3+1]->setText(QString::number(glWidgetsVector[frameNumber]->getBoneTopRadius(glWidgetsVector[frameNumber]->bonesGeometry[i].name)));
		sideGeometryMenuLineEditsVector[i*3+2]->setText(QString::number(glWidgetsVector[frameNumber]->getBoneBottomRadius(glWidgetsVector[frameNumber]->bonesGeometry[i].name)));
	}
}

void MainWindow::addGLWidgetRotateMenu() {
	int rotateMapperCounter = 0;
	vboxRotateLayout->setAlignment(Qt::AlignTop);

	int usedBonesSize = glWidgetsVector[comboBoxFrameSelector->currentIndex()]->bonesConf.size();
	vector<pf::boneConfig> usedBones = glWidgetsVector[comboBoxFrameSelector->currentIndex()]->bonesConf;

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
		sideRotateMenuLabelsVector[labelsCounter]->setText("Obrót " + QString::fromStdString(usedBones[i - 1].name));
		sideRotateMenuLabelsVector[labelsCounter]->setStyleSheet("font-weight: bold");
		sideRotateMenuHLayoutsVector[hLayoutCounter]->addWidget(sideRotateMenuLabelsVector[labelsCounter]);
		labelsCounter++;
		hLayoutCounter++;

		sideRotateMenuLabelsVector[labelsCounter]->setText("Oś X");
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

		sideRotateMenuLabelsVector[labelsCounter]->setText("Oś Y");
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

		sideRotateMenuLabelsVector[labelsCounter]->setText("Oś Z");
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
	glWidgetsVector[frameNumber]->setLimitsVector(glWidgetsVector[frameNumber]->limits, glWidgetsVector[frameNumber]->bonesConf, glWidgetsVector[frameNumber]->bonesGeometry);
	glWidgetsVector[frameNumber]->setVelocityVector(glWidgetsVector[frameNumber]->velocity, glWidgetsVector[frameNumber]->bonesConf, glWidgetsVector[frameNumber]->bonesGeometry);

	/*parametry qspinbox*/
	for (int i = 0; i < sideRotateMenuLineEditsVector.size(); i++) {
		setQSpinBoxSettings(sideRotateMenuSpinBoxesVector[i], 0, 20, 5);
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
}

void MainWindow::addGLWidgetLengthMenu() {
	int lengthMapperCounter = 0;
	vboxLengthLayout->setAlignment(Qt::AlignTop);

	/*przesuniecie modelu*/
	for (int i = 0; i < 4; i++) {
		sideLengthMenuHLayoutsExtraVector.push_back(new QHBoxLayout());
		sideLengthMenuHLayoutsExtraVector[i]->setAlignment(Qt::AlignLeft);
		sideLengthMenuLabelsExtraVector.push_back(new QLabel());
	}

	for (int i = 1; i < 3; i++) {
		sideLengthMenuLabelsExtraVector[i]->setFixedWidth(30);
	}

	for (int i = 0; i < 3; i++){
		sideLengthMenuButtonsExtraVector.push_back(new QPushButton());
		sideLengthMenuButtonsExtraVector.push_back(new QPushButton());
		sideLengthMenuLineEditsExtraVector.push_back(new QLineEdit());
		sideLengthMenuLineEditsExtraVector[i]->setFixedWidth(50);
		sideLengthMenuSpinBoxesExtraVector.push_back(new QSpinBox());
		sideLengthMenuSpinBoxesExtraVector[i]->setFixedWidth(50);
	}

	for(int i=0;i<sideLengthMenuButtonsExtraVector.size(); i++)
		sideLengthMenuButtonsExtraVector[i]->setFixedWidth(30);

	sideLengthMenuLabelsExtraVector[0]->setText("Przesunięcie modelu");
	sideLengthMenuHLayoutsExtraVector[0]->addWidget(sideLengthMenuLabelsExtraVector[0]);

	sideLengthMenuLabelsExtraVector[1]->setText("Oś X");
	sideLengthMenuHLayoutsExtraVector[1]->addWidget(sideLengthMenuLabelsExtraVector[1]);
	sideLengthMenuHLayoutsExtraVector[1]->addWidget(sideLengthMenuButtonsExtraVector[0]);
	sideLengthMenuHLayoutsExtraVector[1]->addWidget(sideLengthMenuLineEditsExtraVector[0]);
	sideLengthMenuHLayoutsExtraVector[1]->addWidget(sideLengthMenuButtonsExtraVector[1]);
	sideLengthMenuHLayoutsExtraVector[1]->addWidget(sideLengthMenuSpinBoxesExtraVector[0]);

	sideLengthMenuLabelsExtraVector[2]->setText("Oś Y");
	sideLengthMenuHLayoutsExtraVector[2]->addWidget(sideLengthMenuLabelsExtraVector[2]);
	sideLengthMenuHLayoutsExtraVector[2]->addWidget(sideLengthMenuButtonsExtraVector[2]);
	sideLengthMenuHLayoutsExtraVector[2]->addWidget(sideLengthMenuLineEditsExtraVector[1]);
	sideLengthMenuHLayoutsExtraVector[2]->addWidget(sideLengthMenuButtonsExtraVector[3]);
	sideLengthMenuHLayoutsExtraVector[2]->addWidget(sideLengthMenuSpinBoxesExtraVector[1]);
	

	/*Skalowanie*/
	sideLengthMenuLabelsExtraVector[3]->setText("Skaluj model");
	sideLengthMenuHLayoutsExtraVector[3]->addWidget(sideLengthMenuLabelsExtraVector[3]);
	sideLengthMenuHLayoutsExtraVector[3]->addWidget(sideLengthMenuButtonsExtraVector[4]);
	sideLengthMenuHLayoutsExtraVector[3]->addWidget(sideLengthMenuButtonsExtraVector[5]);
	sideLengthMenuHLayoutsExtraVector[3]->addWidget(sideLengthMenuSpinBoxesExtraVector[2]);

	for (int j = 0; j < 4; j++) {
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

		default:
			str = " ";
			cout << "BLAD" << endl;
			break;
		}

		QSignalMapper *mapper = new QSignalMapper(this);
		QObject::connect(mapper, SIGNAL(mapped(QString)), this, SLOT(translate(QString)));
		mapper->setMapping(sideLengthMenuButtonsExtraVector[j], str);
		connect(sideLengthMenuButtonsExtraVector[j], SIGNAL(released()), mapper, SLOT(map()));
		//QString().swap(str);
	}

	for (int j = 0; j < 2; j++) {
		QString str;
		switch (j) {
		case 0:
			str = QString::fromStdString("-1.0;2");
			break;

		case 1:
			str = QString::fromStdString("1.0;2");
			break;

		default:
			str = " ";
			cout << "BLAD" << endl;
			break;
		}

		QSignalMapper *mapper = new QSignalMapper(this);
		QObject::connect(mapper, SIGNAL(mapped(QString)), this, SLOT(scale(QString)));
		mapper->setMapping(sideLengthMenuButtonsExtraVector[j+4], str);
		connect(sideLengthMenuButtonsExtraVector[j+4], SIGNAL(released()), mapper, SLOT(map()));
		//QString().swap(str);
	}

	for (int hLayouts = 0; hLayouts < sideLengthMenuHLayoutsExtraVector.size(); hLayouts++)
		vboxLengthLayout->addLayout(sideLengthMenuHLayoutsExtraVector[hLayouts]);
		
	/*dlugosci kosci*/
	int usedBonesSize = glWidgetsVector[comboBoxFrameSelector->currentIndex()]->bonesGeometry.size();
	vector<pf::boneGeometry> usedBones = glWidgetsVector[comboBoxFrameSelector->currentIndex()]->bonesGeometry;

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

	/*parametry qspinbox - translacja*/
	for (int i = 0; i < sideLengthMenuSpinBoxesExtraVector.size() - 1; i++) {
		setQSpinBoxSettings(sideLengthMenuSpinBoxesExtraVector[i], 0, 50, 25);
	}

	/*parametry qspinbox - skalowanie*/
	setQSpinBoxSettings(sideLengthMenuSpinBoxesExtraVector[2], 0, 25, 10);

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
				cout << "BLAD" << endl;
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

	for (int hLayouts = 0; hLayouts < sideLengthMenuHLayoutsVector.size(); hLayouts++) {
		vboxLengthLayout->addLayout(sideLengthMenuHLayoutsVector[hLayouts]);
	}

	setRotCheckBoxes(comboBoxFrameSelector->currentIndex());
	setLimitsSpinBoxes(comboBoxFrameSelector->currentIndex());
	setVelocitySpinBoxes(comboBoxFrameSelector->currentIndex());

	for (int i = 0; i < sideLengthMenuRotCheckBoxesVector.size(); i++) {
		connect(sideLengthMenuRotCheckBoxesVector[i], SIGNAL(stateChanged(int)), this, SLOT(updateBoneConfigFromCB()));
	}

	for (int i = 0; i < sideLengthMenuLimVelSpinBoxesVector.size(); i+=3) {
		connect(sideLengthMenuLimVelSpinBoxesVector[i], SIGNAL(valueChanged(int)), this, SLOT(updateBoneConfigFromLimitsSB()));
		connect(sideLengthMenuLimVelSpinBoxesVector[i+1], SIGNAL(valueChanged(int)), this, SLOT(updateBoneConfigFromLimitsSB()));
		connect(sideLengthMenuLimVelSpinBoxesVector[i + 2], SIGNAL(valueChanged(int)), this, SLOT(updateBoneConfigFromVelocitySB()));
	}

	//updateBoneConfigFromCB();
	refreshLengthSideMenu();

	//vector<pf::boneGeometry>().swap(usedBones);

}

void MainWindow::addGLWidgetGeometryMenu() {
	int geometryMapperCounter = 0;
	vboxGeometryLayout->setAlignment(Qt::AlignTop);

	int usedBonesSize = glWidgetsVector[comboBoxFrameSelector->currentIndex()]->bonesGeometry.size();
	vector<pf::boneGeometry> usedBones = glWidgetsVector[comboBoxFrameSelector->currentIndex()]->bonesGeometry;

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

	for (int lineEdits = 0; lineEdits < sideGeometryMenuLineEditsVector.size(); lineEdits++)
		sideGeometryMenuLineEditsVector[lineEdits]->setReadOnly(true);

	for (int hLayouts = 0; hLayouts < sideGeometryMenuHLayoutsVector.size(); hLayouts++)
		vboxGeometryLayout->addLayout(sideGeometryMenuHLayoutsVector[hLayouts]);

	refreshGeometrySideMenu();
}

void MainWindow::addFrameScrollingMenu(QPushButton *button, int id) {
	frameScrollingMenu.push_back(new QMenu());
	selectBackgroundMenuVector.push_back(new QMenu("Dodaj tlo"));
	showModelInDialogVector.push_back(new QAction("Wyświetl model w nowym oknie"));

	selectImagesBackgroundVector.push_back(new QAction("Wybierz folder zdjęć"));
	selectAviBackgroundVector.push_back(new QAction("Wybierz plik avi"));

	setConfigFromModelVector.push_back(new QAction("Wykorzystaj konfiguracje innego modelu"));
	setConfigToAllModelsVector.push_back(new QAction("Ustaw konfiguracje dla wszystkich pozostalych modeli"));

	selectBackgroundMenuVector[id]->addAction(selectImagesBackgroundVector[id]);
	selectBackgroundMenuVector[id]->addAction(selectAviBackgroundVector[id]);

	frameScrollingMenu[id]->addMenu(selectBackgroundMenuVector[id]);
	frameScrollingMenu[id]->addSeparator();
	frameScrollingMenu[id]->addAction(showModelInDialogVector[id]);
	frameScrollingMenu[id]->addSeparator();
	frameScrollingMenu[id]->addAction(setConfigFromModelVector[id]);
	frameScrollingMenu[id]->addAction(setConfigToAllModelsVector[id]);
	button->setMenu(frameScrollingMenu[id]);
}

void MainWindow::setQSpinBoxSettings(QSpinBox * qspinbox, int minRange, int maxRange, int defaultValue) {
	qspinbox->setRange(minRange, maxRange);
	qspinbox->setValue(defaultValue);
}

void MainWindow::setRotCheckBoxes(int id) {
	for (int i = 0; i < sideLengthMenuRotCheckBoxesVector.size(); i++) {
		sideLengthMenuRotCheckBoxesVector[i]->setChecked(false);
	}

	for (int j = 0; j < glWidgetsVector[id]->bonesGeometry.size(); j++) {
		
		for (int i = 0; i < glWidgetsVector[id]->bonesConf.size(); i++) {
			if (glWidgetsVector[id]->bonesConf[i].name == glWidgetsVector[id]->bonesGeometry[j].name) {
				
				string name = glWidgetsVector[id]->bonesConf[i].name;

				if (glWidgetsVector[id]->modelDOF.find(name)->second[0]) {
					sideLengthMenuRotCheckBoxesVector[j * 3]->setChecked(true);
				}
				else {
					sideLengthMenuRotCheckBoxesVector[j * 3]->setChecked(false);
				}

				if (glWidgetsVector[id]->modelDOF.find(name)->second[1])
					sideLengthMenuRotCheckBoxesVector[j * 3 + 1]->setChecked(true);
				else
					sideLengthMenuRotCheckBoxesVector[j * 3 + 1]->setChecked(false);

				if (glWidgetsVector[id]->modelDOF.find(name)->second[2])
					sideLengthMenuRotCheckBoxesVector[j * 3 + 2]->setChecked(true);
				else
					sideLengthMenuRotCheckBoxesVector[j * 3 + 2]->setChecked(false);

				break;
				//std::string().swap(name);
			}
		}
	}
}

void MainWindow::setLimitsSpinBoxes(int id) {
	vector<vector<int>> lims;
	for (int i = 0; i < glWidgetsVector[id]->bonesGeometry.size(); i++) {
		vector<int> tmp;
		string name = glWidgetsVector[id]->bonesGeometry[i].name;
		tmp = glWidgetsVector[id]->modelLimits.find(name)->second;
		lims.push_back(tmp);
	}

	for (int j = 0; j < glWidgetsVector[id]->bonesGeometry.size(); j++) {
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
	for (int j = 0; j < glWidgetsVector[id]->bonesGeometry.size(); j++) {
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

void MainWindow::setVelocitySpinBoxes(int id) {
	vector<vector<float>> velocities;
	for (int i = 0; i < glWidgetsVector[id]->bonesGeometry.size(); i++) {
		vector<float> tmp;
		string name = glWidgetsVector[id]->bonesGeometry[i].name;
		tmp = glWidgetsVector[id]->modelVelocity.find(name)->second;
		velocities.push_back(tmp);
	}

	for (int j = 0; j < glWidgetsVector[id]->bonesGeometry.size(); j++) {
		string name = glWidgetsVector[id]->bonesGeometry[j].name;
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
	
	glWidgetsVector[frameID]->saveModelStateToMap(glWidgetsVector[frameID]->bonesRotations, glWidgetsVector[frameID]->modelState, glWidgetsVector[frameID]->bonesConf);

	glWidgetsVector[frameID]->bonesConf.clear();

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
	bConf.minRotX = glWidgetsVector[frameID]->modelLimits.find(name)->second[0];
	bConf.maxRotX = glWidgetsVector[frameID]->modelLimits.find(name)->second[1];
	bConf.minRotY = glWidgetsVector[frameID]->modelLimits.find(name)->second[2];
	bConf.maxRotY = glWidgetsVector[frameID]->modelLimits.find(name)->second[3];
	bConf.minRotZ = glWidgetsVector[frameID]->modelLimits.find(name)->second[4];
	bConf.maxRotZ = glWidgetsVector[frameID]->modelLimits.find(name)->second[5];

	/*velocity*/
	bConf.vTransX = glWidgetsVector[frameID]->modelVelocity.find(name)->second[0];
	bConf.vTransY = glWidgetsVector[frameID]->modelVelocity.find(name)->second[1];
	bConf.vTransZ = glWidgetsVector[frameID]->modelVelocity.find(name)->second[2];
	bConf.vRotX = glWidgetsVector[frameID]->modelVelocity.find(name)->second[3];
	bConf.vRotY = glWidgetsVector[frameID]->modelVelocity.find(name)->second[4];
	bConf.vRotZ = glWidgetsVector[frameID]->modelVelocity.find(name)->second[5];

	glWidgetsVector[frameID]->bonesConf.push_back(bConf);

	for (int i = 0; i < glWidgetsVector[frameID]->bonesGeometry.size(); i++) {
		/*jesli wszystkie 3 checkboxy nie zaznaczone*/

		if (sideLengthMenuRotCheckBoxesVector[i*3]->isChecked() || sideLengthMenuRotCheckBoxesVector[i * 3 + 1]->isChecked() || sideLengthMenuRotCheckBoxesVector[i * 3 + 2]->isChecked()) {
			pf::boneConfig bConf;

			string name = glWidgetsVector[frameID]->bonesGeometry[i].name;
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
			bConf.minRotX = glWidgetsVector[frameID]->modelLimits.find(name)->second[0];
			bConf.maxRotX = glWidgetsVector[frameID]->modelLimits.find(name)->second[1];
			bConf.minRotY = glWidgetsVector[frameID]->modelLimits.find(name)->second[2];
			bConf.maxRotY = glWidgetsVector[frameID]->modelLimits.find(name)->second[3];
			bConf.minRotZ = glWidgetsVector[frameID]->modelLimits.find(name)->second[4];
			bConf.maxRotZ = glWidgetsVector[frameID]->modelLimits.find(name)->second[5];

			/*velocity*/
			bConf.vRotX = glWidgetsVector[frameID]->modelVelocity.find(name)->second[0];
			bConf.vRotY = glWidgetsVector[frameID]->modelVelocity.find(name)->second[1];
			bConf.vRotZ = glWidgetsVector[frameID]->modelVelocity.find(name)->second[2];
			glWidgetsVector[frameID]->bonesConf.push_back(bConf);
			
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

				glWidgetsVector[frameID]->updateBonesDOFMap(glWidgetsVector[frameID]->modelDOF, bConf.name, tmpDOFs);
			}


		}
		else {}
	}

	glWidgetsVector[frameID]->setLimitsVector(glWidgetsVector[frameID]->limits, glWidgetsVector[frameID]->bonesConf, glWidgetsVector[frameID]->bonesGeometry);
	glWidgetsVector[frameID]->setVelocityVector(glWidgetsVector[frameID]->velocity, glWidgetsVector[frameID]->bonesConf, glWidgetsVector[frameID]->bonesGeometry);

	FileHandler *fileHandler = new FileHandler();
	fileHandler->reloadParams(glWidgetsVector[frameID]);

	reloadGLRotateMenu();
	delete fileHandler;
}

void MainWindow::updateBoneConfigFromLimitsSB() {
	int frameID = comboBoxFrameSelector->currentIndex();

	for (int i = 0; i < glWidgetsVector[frameID]->bonesGeometry.size(); i++) {
		vector<int> tmp;
		string name = glWidgetsVector[frameID]->bonesGeometry[i].name;

		tmp.push_back(sideLengthMenuLimVelSpinBoxesVector[i * 9]->value());
		tmp.push_back(sideLengthMenuLimVelSpinBoxesVector[i * 9 + 1]->value());
		tmp.push_back(sideLengthMenuLimVelSpinBoxesVector[i * 9 + 3]->value());
		tmp.push_back(sideLengthMenuLimVelSpinBoxesVector[i * 9 + 4]->value());
		tmp.push_back(sideLengthMenuLimVelSpinBoxesVector[i * 9 + 6]->value());
		tmp.push_back(sideLengthMenuLimVelSpinBoxesVector[i * 9 + 7]->value());
		glWidgetsVector[frameID]->modelLimits.at(name) = tmp;
	}

	for (int i = 1; i < glWidgetsVector[frameID]->bonesConf.size(); i++) {
		string name = glWidgetsVector[frameID]->bonesConf[i].name;
		vector <int> tmp = glWidgetsVector[frameID]->modelLimits.find(name)->second;

		glWidgetsVector[frameID]->bonesConf[i].minRotX = tmp[0];
		glWidgetsVector[frameID]->bonesConf[i].maxRotX = tmp[1];
		glWidgetsVector[frameID]->bonesConf[i].minRotY = tmp[2];
		glWidgetsVector[frameID]->bonesConf[i].maxRotY = tmp[3];
		glWidgetsVector[frameID]->bonesConf[i].minRotZ = tmp[4];
		glWidgetsVector[frameID]->bonesConf[i].maxRotZ = tmp[5];
	}

	/* nie moze byc sytuacji gdy min > max oraz max < min */
	for (int j = 0; j < glWidgetsVector[frameID]->bonesGeometry.size(); j++) {
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

	glWidgetsVector[frameID]->setLimitsVector(glWidgetsVector[frameID]->limits, glWidgetsVector[frameID]->bonesConf, glWidgetsVector[frameID]->bonesGeometry);

	reloadGLRotateMenu();
}

void MainWindow::updateBoneConfigFromVelocitySB() {
	int frameID = comboBoxFrameSelector->currentIndex();

	for (int i = 0; i < glWidgetsVector[frameID]->bonesGeometry.size(); i++) {
		vector<float> tmp;
		string name = glWidgetsVector[frameID]->bonesGeometry[i].name;

		tmp.push_back(sideLengthMenuLimVelSpinBoxesVector[i * 9 + 2]->value());
		tmp.push_back(sideLengthMenuLimVelSpinBoxesVector[i * 9 + 5]->value());
		tmp.push_back(sideLengthMenuLimVelSpinBoxesVector[i * 9 + 8]->value());
		glWidgetsVector[frameID]->modelVelocity.at(name) = tmp;
	}

	for (int i = 1; i < glWidgetsVector[frameID]->bonesConf.size(); i++) {
		string name = glWidgetsVector[frameID]->bonesConf[i].name;
		vector<float> tmp = glWidgetsVector[frameID]->modelVelocity.find(name)->second;

		glWidgetsVector[frameID]->bonesConf[i].vRotX = tmp[0];
		glWidgetsVector[frameID]->bonesConf[i].vRotY = tmp[1];
		glWidgetsVector[frameID]->bonesConf[i].vRotZ = tmp[2];

	}

	glWidgetsVector[frameID]->setVelocityVector(glWidgetsVector[frameID]->velocity, glWidgetsVector[frameID]->bonesConf, glWidgetsVector[frameID]->bonesGeometry);
	//reloadGLRotateMenu();
}

void MainWindow::disableRotatingButtons() {
	int frameID = comboBoxFrameSelector->currentIndex();

	/*limity kosci wybranego modelu*/
	vector<pf::range2> bonesLimits = glWidgetsVector[frameID]->limits;

	/*wylaczenie przyciskow*/
	vector<pf::boneConfig> bConf = glWidgetsVector[frameID]->bonesConf;
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

void MainWindow::tabSelected() {
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
	showModelInDialogMapperVector.push_back(new QSignalMapper(this));
	setConfigFromModelMapperVector.push_back(new QSignalMapper(this));
	setConfigToAllModelsMapperVector.push_back(new QSignalMapper(this));
	addImagesBackgroundMapperVector.push_back(new QSignalMapper(this));
	addAviMapperVector.push_back(new QSignalMapper(this));
	sliderMapperVector.push_back(new QSignalMapper(this));
}