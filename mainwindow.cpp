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
	hideLayouts();
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

	loadImagesAction = new QAction("Wczytaj pliki", this);
	connect(loadImagesAction, &QAction::triggered, this, &MainWindow::loadFiles);
	plikMenu->addAction(loadImagesAction);

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

	loadFromAmcAction = new QAction("Wczytaj plik .amc", this);
	connect(loadFromAmcAction, &QAction::triggered, this, &MainWindow::loadFromAmc);
	plikMenu->addAction(loadFromAmcAction);
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
	vboxLayout = new QVBoxLayout;
	//sideGridLayout = new QGridLayout;
	gridChildLayout = new QGridLayout;
	selectFrameLayout = new QHBoxLayout;
	selectFrameLabel = new QLabel;

	windowLayout = new QWidget;

	scrollWidget = new QWidget;
	scrollSideWidget = new QWidget;
	tabWidget = new QWidget;
	sideManuTabWidget = new QTabWidget(tabWidget);

	scrollArea = new QScrollArea;
	scrollArea->setWidgetResizable(true);
	scrollWidget->setLayout(gridChildLayout);

	scrollSideArea = new QScrollArea;
	scrollSideArea->setWidgetResizable(true);
	scrollSideWidget->setLayout(vboxLayout);

	scrollArea->setWidget(scrollWidget);
	scrollSideArea->setWidget(scrollSideWidget);

	sideManuTabWidget->addTab(scrollSideArea, "rotacje");
	sideManuTabWidget->addTab(new QWidget, "kalibracja");

	selectFrameLabel->setText("Wybierz okno");
	selectFrameLayout->addWidget(selectFrameLabel);
	selectFrameLayout->addWidget(comboBoxFrameSelector);

	

	gridLayout->addWidget(addFrameButton, 0, 0, 1, 1);
	gridLayout->addWidget(globalSlider, 0, 1, 1, 8);
	gridLayout->addLayout(selectFrameLayout, 0, 9, 1, 1);
	gridLayout->addWidget(scrollArea, 1, 0, 9, 9);
	gridLayout->addWidget(sideManuTabWidget, 1, 9, 9, 2);

	windowLayout->setLayout(gridLayout);

	setCentralWidget(windowLayout);
}

void MainWindow::resetVariables() {
	list.clear();

	glWidgetsVector.clear();
	minButtonsVector.clear();
	reduceButtonsVector.clear();
	maxButtonsVector.clear();
	increaseButtonsVector.clear();
	countersVector.clear();
	checkBoxesVector.clear();
	imagesListIterator.clear();
	menuButtonsVector.clear();
	frameScrollingMenu.clear();
	manageBonesVector.clear();

	sideMenuLabelsVector.clear();
	sideMenuHLayoutsVector.clear();
	sideMenuButtonsVector.clear();
	sideMenuLineEditsVector.clear();
	sideMenuSpinBoxesVector.clear();

	globalListIterator = 1;
	addFrameButtonCounter = 0;

	//loadedImagesFolderPath->clear();

	showLayouts();
}

void MainWindow::hideLayouts() {
	windowLayout->hide();
}

void MainWindow::showLayouts() {
	windowLayout->show();
}

void MainWindow::loadFiles(){
	loadedImagesFolderPath = new QString(QFileDialog::getExistingDirectory(this, tr("Img folder")));
    if(!loadedImagesFolderPath->isEmpty()){

		if(!windowLayout->layout()->isEmpty())
			delete windowLayout->layout();
		prepareLayouts();
		resetVariables();

        QDir dir(*loadedImagesFolderPath);
        QStringList filter;
        filter << QLatin1String("*.png");
        filter << QLatin1String("*.jpg");
        filter << QLatin1String("*.jpeg");
		filter << QLatin1String("*.bmp");
        dir.setNameFilters(filter);
        list = dir.entryInfoList();
        prepareGlobalSlider(globalSlider);
        addFrameButton->setVisible(true);
        addFrameButton->setText(QString("Dodaj okno"));			
		
		QMessageBox::information(NULL, QObject::tr("app"),
			QObject::tr("Wczytano pomyślnie"),
			QMessageBox::Cancel,
			QMessageBox::Cancel);
    }
}

void MainWindow::saveOneFrameToFile() {
	FileHandler * fileHandler = new FileHandler();
	int id = comboBoxFrameSelector->currentIndex();

	QString folderPath = QFileDialog::getExistingDirectory(this, tr("Model files folder"));
	if (!folderPath.isEmpty()) {
		/*scieżka utworzonego folderu zapisywanej klatki*/
		folderPath += ("/" + list[imagesListIterator[id] - 1].baseName());
		/*sciezki plikow*/
		QString amcFileName = folderPath + "/" + list[imagesListIterator[id] - 1].baseName() + ".amc"; 
		QString asfFileName = folderPath + "/" + list[imagesListIterator[id] - 1].baseName() + ".asf";
		QString datFileName = folderPath + "/" + list[imagesListIterator[id] - 1].baseName() + ".dat";
		QString helperFile = folderPath + "/" + "helperFile.dat";

		if (QDir(folderPath).exists()) {
			QMessageBox::StandardButton reply;
			reply = QMessageBox::question(this,
				"?",
				"Konfiguracja dla wybranej klatki została wcześniej zapisana. Nadpisać?",
				QMessageBox::Yes | QMessageBox::No);
			if (reply = QMessageBox::Yes) {
				QDir().mkdir(folderPath);

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
				QString imagePath = list[imagesListIterator[0] - 1].absoluteFilePath();
				fileHandler->saveHelperFile(*loadedImagesFolderPath, helperFile, imagePath);

				QMessageBox::information(NULL, QObject::tr("app"),
					QObject::tr("Zapisano pomyślnie"),
					QMessageBox::Cancel,
					QMessageBox::Cancel);
			}
			else {}
		}
		else {
			QDir().mkdir(folderPath);

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
			QString imagePath = list[imagesListIterator[0] - 1].absoluteFilePath();
			fileHandler->saveHelperFile(*loadedImagesFolderPath, helperFile, imagePath);

			QMessageBox::information(NULL, QObject::tr("app"),
				QObject::tr("Zapisano pomyślnie"),
				QMessageBox::Cancel,
				QMessageBox::Cancel);
		}
	}
}

void MainWindow::saveSequenceToFile() {
	FileHandler * fileHandler = new FileHandler();
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Zapisz model w formacie .amc"), "",
		tr("AMC (*.amc);;All Files(*)"));
	
	if (!fileName.isEmpty()) {
		QString folderPath = QFileInfo(fileName).absolutePath();
		folderPath += ("/" + QFileInfo(fileName).baseName());
		QString helperFile = folderPath + "/" + "helperFile.dat";
		QString amcFileName = folderPath + "/" + QFileInfo(fileName).baseName() + ".amc";

		if (QDir(folderPath).exists()) {
			QMessageBox::StandardButton reply;
			reply = QMessageBox::question(this,
				"?",
				"Konfiguracja dla wybranej sekwencji została wcześniej zapisana. Nadpisać?",
				QMessageBox::Yes | QMessageBox::No);
			if (reply = QMessageBox::Yes) {
				QDir().mkdir(folderPath);

				vector<int> framesID; /* wektor indeksów utworzonych glWidget */
				vector<QString> imagesPath; /* wektor sciezek do plikow tla */

				/*Stany kolejnych modeli*/
				vector<vector<float>> states;
				int frames = comboBoxFrameSelector->count();
				for (int i = 0; i < frames; i++) {
					states.push_back(glWidgetsVector[i]->modelState[0]);
				}
				//tu jest zle, nie wszystkie modele musza miec tyle samo kosci !!
				int id = comboBoxFrameSelector->currentIndex();

				/*Zapis plików*/
				/*Zapis pliku AMC*/
				fileHandler->saveAMCSeq(
					states,
					glWidgetsVector[id]->allBones,
					glWidgetsVector[id]->usedBones,
					amcFileName
				);

				vector <QString> asfFiles;
				vector <QString> datFiles;

				for (int i = 0; i < frames; i++) {
					QString asfFileName = folderPath + "/" + list[imagesListIterator[i] - 1].baseName();
					QString datFileName = folderPath + "/" + list[imagesListIterator[i] - 1].baseName();

					//asfFiles.push_back(asfFileName);
					//datFiles.push_back(datFileName);
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
					framesID.push_back(i);
					imagesPath.push_back(list[imagesListIterator[i] - 1].absoluteFilePath());
				}
				/*Zapis pliku pomocniczego*/
				fileHandler->saveHelperFile(*loadedImagesFolderPath, helperFile, framesID, imagesPath);
				/*Koniec zapisu plików*/

				QMessageBox::information(NULL, QObject::tr("app"),
					QObject::tr("Zapisano pomyślnie"),
					QMessageBox::Cancel,
					QMessageBox::Cancel);
			}
			else {}
		}
		else {
			QDir().mkdir(folderPath);

			vector<int> framesID; /* wektor indeksów utworzonych glWidget */
			vector<QString> imagesPath; /* wektor sciezek do plikow tla */

			/*Stany kolejnych modeli*/
			vector<vector<float>> states;
			int frames = comboBoxFrameSelector->count();
			for (int i = 0; i < frames; i++) {
				states.push_back(glWidgetsVector[i]->modelState[0]);
			}
			//tu jest zle, nie wszystkie modele musza miec tyle samo kosci !!
			int id = comboBoxFrameSelector->currentIndex();

			/*Zapis plików*/
			/*Zapis pliku AMC*/
			fileHandler->saveAMCSeq(
				states,
				glWidgetsVector[id]->allBones,
				glWidgetsVector[id]->usedBones,
				amcFileName
			);

			vector <QString> asfFiles;
			vector <QString> datFiles;

			for (int i = 0; i < frames; i++) {
				QString asfFileName = folderPath + "/" + list[imagesListIterator[i] - 1].baseName();
				QString datFileName = folderPath + "/" + list[imagesListIterator[i] - 1].baseName();

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
				framesID.push_back(i);
				imagesPath.push_back(list[imagesListIterator[i] - 1].absoluteFilePath());
			}
			/*Zapis pliku pomocniczego*/
			fileHandler->saveHelperFile(*loadedImagesFolderPath, helperFile, framesID, imagesPath);
			/*Koniec zapisu plików*/

			QMessageBox::information(NULL, QObject::tr("app"),
				QObject::tr("Zapisano pomyślnie"),
				QMessageBox::Cancel,
				QMessageBox::Cancel);
		}
	}
}

void MainWindow::loadFromAmc() {
	QString amcFilePath = QFileDialog::getOpenFileName(this,
		tr("Wczytaj .amc"), "",
		tr("AMC (*.amc);;All Files(*)"));

	FileHandler * fileHandler = new FileHandler();

	/*wektor nazw plików asf*/
	vector<QString> asfFilesPaths;

	/*wektor nazw plików dat*/
	vector<QString> datFilesPaths;

	/*ścieżka folderu ze zdjęciami wykorzystanymi do tworzenia tła*/
	QString imgFolderPath;

	/*ściezka do pliku pomocniczego*/
	QString helperFilePath;
	if (!amcFilePath.isEmpty()) {

		if (!windowLayout->layout()->isEmpty())
			delete windowLayout->layout();
		prepareLayouts();
		resetVariables();

		fileHandler->getFilesAsQStrings(amcFilePath, asfFilesPaths, datFilesPaths, imgFolderPath);

		QString folderPath = QFileInfo(amcFilePath).absolutePath();
		helperFilePath = folderPath + "/helperFile.dat";

		//wektor zawierajacy sciezki do zdjec tla kolejnych klatek
		vector<QString> loadedImages = fileHandler->getGlWidgetBackgroudFromFile(helperFilePath);

		/*wczytanie pliku amc do wektora stanow*/
		vector<vector<float>> states = fileHandler->loadAmcFromFile(amcFilePath, asfFilesPaths, datFilesPaths);

		if (!states.empty()) {
			QDir dir(imgFolderPath);
			if (dir.exists()) {
				loadedImagesFolderPath = new QString(imgFolderPath);

				prepareGlobalSlider(globalSlider);
				addFrameButton->setVisible(true);
				addFrameButton->setText(QString("Dodaj okno"));

				QStringList filter;
				filter << QLatin1String("*.png");
				filter << QLatin1String("*.jpg");
				filter << QLatin1String("*.jpeg");
				filter << QLatin1String("*.bmp");
				dir.setNameFilters(filter);
				list = dir.entryInfoList();

				vector<int> imagesIDs = fileHandler->getImagesIDs(list, loadedImages);
				for (int i = 0; i < states.size(); i++) {
					addFrameButton->click();
				}

				for (int i = 0; i < glWidgetsVector.size(); i++) {
					glWidgetsVector[i]->bonesConf.clear();
					glWidgetsVector[i]->bonesGeometry.clear();

					fileHandler->loadDatFromFile(datFilesPaths[i].toUtf8().constData(), glWidgetsVector[i]->bonesConf, glWidgetsVector[i]->bonesGeometry);
					glWidgetsVector[i]->setLimitsVector(glWidgetsVector[i]->limits, glWidgetsVector[i]->bonesConf);
					glWidgetsVector[i]->model = new pf::Model3D(pf::Model3D::Cylinder, asfFilesPaths[i].toUtf8().constData(), datFilesPaths[i].toUtf8().constData());
					glWidgetsVector[i]->modelState[0] = states[i];

					imagesListIterator[i] = imagesIDs[i];
					setTextIteratorLabel(countersVector[i], imagesIDs[i], list.size());
					/*odswiezenie wartosci qlineedit*/
					refreshSideMenu();
					glWidgetsVector[i]->rotate("root", 1.0, pf::Model3D::axisX, 0);
					glWidgetsVector[i]->rotate("root", -1.0, pf::Model3D::axisX, 0);
					//update();

					cout << glWidgetsVector[i]->limits.size() << " " << glWidgetsVector[i]->bonesConf.size() << endl;
					for (int j = 0; j < glWidgetsVector[i]->limits.size(); j++) {
						//cout << glWidgetsVector[i]->bonesConf[j].name << endl;
						cout << glWidgetsVector[i]->limits[j].min << " " << glWidgetsVector[i]->limits[j].max << endl;
						cout << endl;
					}
				}
			}
			else {
				QMessageBox::warning(NULL, QObject::tr("app"),
					QObject::tr("Folder z wykorzystanymi zdjęciami nie istnieje!"),
					QMessageBox::Cancel,
					QMessageBox::Cancel);
			}
		}
		reloadGLWidgetMenu();
	}

	delete fileHandler;
}

void MainWindow::buttonAddFramePressed(){
    addFrameMenu(addFrameButtonCounter);
	comboBoxFrameSelector->addItem(tr(std::to_string(addFrameButtonCounter).c_str()));
	if (addFrameButtonCounter == 0)
		addGLWidgetMenu();
	addFrameButtonCounter++;

}

void MainWindow::addFrameMenu(int i){
    int j=0;
    QWidget *borderWidget = new QWidget();
    borderWidget->setStyleSheet("border: 1px solid gray");

    glWidgetsVector.push_back(new GLWidget(this));
    minButtonsVector.push_back(new QPushButton());
    maxButtonsVector.push_back(new QPushButton());
    reduceButtonsVector.push_back(new QPushButton());
    increaseButtonsVector.push_back(new QPushButton());
    countersVector.push_back(new QLabel());
    checkBoxesVector.push_back(new QCheckBox());
    imagesListIterator.push_back(0);
	menuButtonsVector.push_back(new QPushButton("menu"));

    glWidgetsVector[addFrameButtonCounter]->setMinimumSize(100,300);
    glWidgetsVector[addFrameButtonCounter]->setMaximumHeight(400);
    glWidgetsVector[addFrameButtonCounter]->setMaximumWidth(1200);

    checkBoxesVector[addFrameButtonCounter]->setChecked(true);

	addFrameScrollingMenu(menuButtonsVector[addFrameButtonCounter], addFrameButtonCounter);

    prepareFrameMenu(addFrameButtonCounter);
    /*rows, cols, rowspan, colspan*/
    if(i%2==0){
        if(i>1) j=(i/2)*2;
        gridChildLayout->addWidget(borderWidget, (i/2)*10+j, 0, 12, 10);
        gridChildLayout->addWidget(glWidgetsVector[addFrameButtonCounter], (i/2)*10+j, 0, 10, 10);
        gridChildLayout->addWidget(checkBoxesVector[addFrameButtonCounter], (i/2)*10+11+j, 0, 1, 1);
        gridChildLayout->addWidget(minButtonsVector[addFrameButtonCounter], (i/2)*10+11+j, 1, 1, 1);
        gridChildLayout->addWidget(reduceButtonsVector[addFrameButtonCounter], (i/2)*10+11+j, 2, 1, 1);
        gridChildLayout->addWidget(increaseButtonsVector[addFrameButtonCounter], (i/2)*10+11+j, 7, 1, 1);
        gridChildLayout->addWidget(maxButtonsVector[addFrameButtonCounter], (i/2)*10+11+j, 8, 1, 1);
        gridChildLayout->addWidget(countersVector[addFrameButtonCounter], (i/2)*10+11+j, 5, 1, 2);
		gridChildLayout->addWidget(menuButtonsVector[addFrameButtonCounter], (i/2)*10+11+j, 9, 1, 1);
    }else {
        if(i>1) j=(i/2)*2;
        gridChildLayout->addWidget(borderWidget, (i/2)*10+j, 11, 12, 10);
        gridChildLayout->addWidget(glWidgetsVector[addFrameButtonCounter], (i/2)*10+j, 11, 10, 10);
        gridChildLayout->addWidget(checkBoxesVector[addFrameButtonCounter], (i/2)*10+11+j, 11, 1, 1);
        gridChildLayout->addWidget(minButtonsVector[addFrameButtonCounter], (i/2)*10+11+j, 12, 1, 1);
        gridChildLayout->addWidget(reduceButtonsVector[addFrameButtonCounter], (i/2)*10+11+j, 13, 1, 1);
        gridChildLayout->addWidget(increaseButtonsVector[addFrameButtonCounter], (i/2)*10+11+j, 18, 1, 1);
        gridChildLayout->addWidget(maxButtonsVector[addFrameButtonCounter], (i/2)*10+11+j, 19, 1, 1);
        gridChildLayout->addWidget(countersVector[addFrameButtonCounter], (i/2)*10+11+j, 16, 1, 2);
		gridChildLayout->addWidget(menuButtonsVector[addFrameButtonCounter], (i / 2) * 10 + 11 + j, 20, 1, 1);
    }
	//QString imgString = list[globalListIterator-1].absoluteFilePath();
	//glWidgetsVector[addFrameButtonCounter]->imgPath = imgString.toUtf8().constData();

	if (comboBoxFrameSelector->count() == 0)
		saveAmcAction->setEnabled(true);

	if (comboBoxFrameSelector->count() == 1)
		saveAmcSequenceAction->setEnabled(true);

	update();
}

void MainWindow::setTextIteratorLabel(QLabel *label, int iterator, int listSize) {
	QString string = QString("%1 / %2").arg(iterator).arg(listSize);
	label->setText(string);
}

void MainWindow::increaseIteratorPressed(int i) {
	int listIterator = imagesListIterator[i];
	if (listIterator < list.size())
		listIterator++;
	else
		listIterator = list.size();

	setTextIteratorLabel(countersVector[i], listIterator, list.size());
	imagesListIterator[i] = listIterator;

	/*QString imgString = list[listIterator-1].absoluteFilePath();
	glWidgetsVector[i]->imgPath = imgString.toUtf8().constData();
	update();*/

	/*QString imageFile = list[listIterator-1].absoluteFilePath();
	setImage1Label(ui->label_pic1, imageFile);*/
}

void MainWindow::reduceIteratorPressed(int i) {
	int listIterator = imagesListIterator[i];
	if (listIterator > 1)
		listIterator--;
	else
		listIterator = 1;

	setTextIteratorLabel(countersVector[i], listIterator, list.size());
	imagesListIterator[i] = listIterator;
}

void MainWindow::minIteratorPressed(int i) {
	imagesListIterator[i] = 1;
	setTextIteratorLabel(countersVector[i], imagesListIterator[i], list.size());

	/* QString imageFile = list[listIterator-1].absoluteFilePath();
	setImage1Label(ui->label_pic1, imageFile);*/
}

void MainWindow::maxIteratorPressed(int i) {
	imagesListIterator[i] = list.size();
	setTextIteratorLabel(countersVector[i], imagesListIterator[i], list.size());

	/*QString imageFile = list[listIterator-1].absoluteFilePath();
	setImage1Label(ui->label_pic1, imageFile);*/
}

void MainWindow::manageBonesPressed(int i) {
	boneManagmentWindow = new BoneManagment(this);

	boneManagmentWindow->setWindowTitle("Zarządzaj modelem");
	
	vector<int> usedBonesIDs = boneManagmentWindow->getUsedBonesIDs(glWidgetsVector[i]->bonesConf, glWidgetsVector[i]->allBones);
	boneManagmentWindow->getUsedBones(glWidgetsVector[i]->asfBones, usedBonesIDs);
	
	boneManagmentWindow->resize(500, 400);
	boneManagmentWindow->exec();

	vector<int> bonesIDs = boneManagmentWindow->selectedBonesIDs;
	vector<string> bonesNames = boneManagmentWindow->selectedBonesNames;

	if (!bonesIDs.empty() && !bonesNames.empty()) {
		FileHandler *fileHandler = new FileHandler();

		/*aktualizacja mapy bonesRotations*/
		glWidgetsVector[i]->saveModelStateToMap(glWidgetsVector[i]->bonesRotations, glWidgetsVector[i]->modelState, glWidgetsVector[i]->bonesConf);

		/*Do zapisu dodatkowych kosci w :geometry*/
		vector<pf::boneGeometry> bonesGeometryTmp = glWidgetsVector[i]->bonesGeometry;

		glWidgetsVector[i]->bonesConf.clear();
		
		for (int j = 0; j < bonesIDs.size(); j++) {
			/*Dodanie nowego obiektu boneConfig - wykorzystywany do tworzenia pliku dat*/
			fileHandler->addBoneConf(glWidgetsVector[i]->bonesConf,
				glWidgetsVector[i]->idxBonesMap,
				glWidgetsVector[i]->asfBones,
				bonesIDs[j]);

			/*Dodanie nowego obiektu boneGeometry*/
			/*fileHandler->addBoneGeo(glWidgetsVector[i]->bonesGeometry,
				glWidgetsVector[i]->idxBonesMap,
				glWidgetsVector[i]->asfBones,
				bonesIDs[j]);*/
		}

		for (int j = 0; j < glWidgetsVector[i]->bonesGeometry.size(); j++) {

			pf::boneGeometry bGeo = glWidgetsVector[i]->bonesGeometry[j];

			if (fileHandler->isBoneChecked(bGeo.name, bonesNames)) {
				glWidgetsVector[i]->bonesGeometry[j].length = 200;// ##########################################################
				glWidgetsVector[i]->model->updateBoneGeometry(bGeo.name, glWidgetsVector[i]->bonesGeometry[j]);
			}
			else {
				glWidgetsVector[i]->bonesGeometry[j].length = 0;
				glWidgetsVector[i]->model->updateBoneGeometry(bGeo.name, glWidgetsVector[i]->bonesGeometry[j]);
			}
		}

		/*aktualizacja model state*/
		glWidgetsVector[i]->updateModelStateFromMap(glWidgetsVector[i]->modelState, glWidgetsVector[i]->bonesRotations, glWidgetsVector[i]->bonesConf);
		//glWidgetsVector[i]->model->updateModelState(glWidgetsVector[i]->modelState[0]);
		reloadGLWidgetMenu();
	}

}

void MainWindow::mapIncreaseButtonSlot(int i){
    QSignalMapper * mapper = new QSignalMapper(this);
    QObject::connect(mapper,SIGNAL(mapped(int)),this,SLOT(increaseIteratorPressed(int)));
    mapper->setMapping(increaseButtonsVector[i], i);
    connect(increaseButtonsVector[i],SIGNAL(released()),mapper,SLOT(map()));
}

void MainWindow::mapReduceButtonSlot(int i){
    QSignalMapper * mapper1 = new QSignalMapper(this);
    QObject::connect(mapper1,SIGNAL(mapped(int)),this,SLOT(reduceIteratorPressed(int)));
    mapper1->setMapping(reduceButtonsVector[i], i);
    connect(reduceButtonsVector[i],SIGNAL(released()),mapper1,SLOT(map()));
}

void MainWindow::mapMaxButtonSlot(int i){
    QSignalMapper * mapper2 = new QSignalMapper(this);
    QObject::connect(mapper2,SIGNAL(mapped(int)),this,SLOT(minIteratorPressed(int)));
    mapper2->setMapping(minButtonsVector[i], i);
    connect(minButtonsVector[i],SIGNAL(released()),mapper2,SLOT(map()));
}

void MainWindow::mapMinButtonSlot(int i){
    QSignalMapper * mapper3 = new QSignalMapper(this);
    QObject::connect(mapper3,SIGNAL(mapped(int)),this,SLOT(maxIteratorPressed(int)));
    mapper3->setMapping(maxButtonsVector[i], i);
    connect(maxButtonsVector[i],SIGNAL(released()),mapper3,SLOT(map()));
}

void MainWindow::mapManageBonesSlot(int i) {
	QSignalMapper * mapper = new QSignalMapper(this);
	QObject::connect(mapper, SIGNAL(mapped(int)), this, SLOT(manageBonesPressed(int)));
	mapper->setMapping(manageBonesVector[i], i);
	connect(manageBonesVector[i], SIGNAL(triggered()), mapper, SLOT(map()));
}

void MainWindow::updateGlobalListIterator(int value){
    globalListIterator = value;
    for(int i=0;i<imagesListIterator.size();i++){
        if(checkBoxesVector[i]->isChecked()){
            imagesListIterator[i]=value;
            setTextIteratorLabel(countersVector[i], value, list.size());
        }
    }
    update();
}

void MainWindow::prepareFrameMenu(int i) {
	//globalListIterator = 1;

	setTextIteratorLabel(countersVector[i], globalListIterator, list.size());

	imagesListIterator[i] = globalListIterator;

	mapIncreaseButtonSlot(i);
	mapReduceButtonSlot(i);
	mapMaxButtonSlot(i);
	mapMinButtonSlot(i);
	mapManageBonesSlot(i);
}

void MainWindow::prepareGlobalSlider(QSlider *slider) {
	slider->setOrientation(Qt::Horizontal);
	slider->setMaximum(list.size());
	slider->setMinimum(1);
	slider->setSingleStep(1);
	slider->setVisible(true);
}

void MainWindow::rotate(QString str) {
	cout << str.toUtf8().constData() << endl;
	cout << endl;
	QStringList values = str.split(";");
	if (values.count() == 5) {
		string boneName = values.at(0).toStdString();
		float direction = values.at(1).toFloat();
		string axis = values.at(2).toStdString();
		int qLineEditID = values.at(3).toInt();
		int qSpinBoxID = values.at(4).toInt();

		if (axis == "X")
			glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate(boneName, direction, pf::Model3D::axisX, sideMenuSpinBoxesVector[qSpinBoxID]->value());
		if (axis == "Y")
			glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate(boneName, direction, pf::Model3D::axisY, sideMenuSpinBoxesVector[qSpinBoxID]->value());
		if (axis == "Z")
			glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate(boneName, direction, pf::Model3D::axisZ, sideMenuSpinBoxesVector[qSpinBoxID]->value());
		cout << glWidgetsVector[comboBoxFrameSelector->currentIndex()]->modelState[0][qLineEditID + 3] << endl;
		sideMenuLineEditsVector[qLineEditID]->setText(QString::number(glWidgetsVector[comboBoxFrameSelector->currentIndex()]->modelState[0][qLineEditID+3]));
	}

	else {
		//Okno błedu
		cout << "BLAD" << endl;
	}
}

void MainWindow::reloadGLWidgetMenu() {
	qDeleteAll(sideMenuLineEditsVector);
	qDeleteAll(sideMenuLabelsVector);
	qDeleteAll(sideMenuButtonsVector);
	qDeleteAll(sideMenuHLayoutsVector);

	sideMenuLineEditsVector.clear();
	sideMenuLabelsVector.clear();
	sideMenuButtonsVector.clear();
	sideMenuHLayoutsVector.clear();

	addGLWidgetMenu();
}

void MainWindow::refreshSideMenu() {
	
	//numer wybranego okna
	int frameNumber = comboBoxFrameSelector->currentIndex();

	pf::Model3D *model = glWidgetsVector[frameNumber]->model;
	int rootID = glWidgetsVector[frameNumber]->getModelStateID(model, "root", pf::Model3D::axisX);
	for (int i=0; i<sideMenuLineEditsVector.size(); i++)
	sideMenuLineEditsVector[i]->setText(QString::number(glWidgetsVector[frameNumber]->modelState[0][rootID+i]));
}

void MainWindow::addGLWidgetMenu() {
	vboxLayout->setAlignment(Qt::AlignTop);
	QWidget *borderWidget = new QWidget();
	borderWidget->setStyleSheet("border: 1px solid gray");

	sideMenuLabelsVector.push_back(new QLabel());
	sideMenuHLayoutsVector.push_back(new QHBoxLayout());

	int usedBonesSize = glWidgetsVector[comboBoxFrameSelector->currentIndex()]->bonesConf.size();
	vector<pf::boneConfig> usedBones = glWidgetsVector[comboBoxFrameSelector->currentIndex()]->bonesConf;
	//vector<string> usedBones = glWidgetsVector[comboBoxFrameSelector->currentIndex()]->usedBones;

	for (int i = 0; i < usedBonesSize * 4; i++)
		sideMenuHLayoutsVector.push_back(new QHBoxLayout());
	
	for (int i = 0; i < usedBonesSize * 4; i++)
		sideMenuLabelsVector.push_back(new QLabel());

	for (int i = 0; i < usedBonesSize * 6; i++)
		sideMenuButtonsVector.push_back(new QPushButton());

	for (int i = 0; i < usedBonesSize * 3; i++)
		sideMenuLineEditsVector.push_back(new QLineEdit());
	for (int i = 0; i < usedBonesSize * 3; i++)
		sideMenuSpinBoxesVector.push_back(new QSpinBox());

	int hLayoutCounter = 1, labelsCounter = 1, buttonsCounter = 0, lineEditsCounter = 0, spinBoxesCounter = 0;
	for (int i = 1; i <= usedBonesSize; i++) {
		sideMenuLabelsVector[labelsCounter]->setText("Obrót " + QString::fromStdString(usedBones[i - 1].name));
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuLabelsVector[labelsCounter]);
		labelsCounter++;
		hLayoutCounter++;

		sideMenuLabelsVector[labelsCounter]->setText("Oś X");
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuLabelsVector[labelsCounter]);
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuButtonsVector[buttonsCounter]);
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuLineEditsVector[lineEditsCounter]);
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuButtonsVector[buttonsCounter+1]);
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuSpinBoxesVector[spinBoxesCounter]);

		lineEditsCounter++;
		buttonsCounter++;
		buttonsCounter++;
		labelsCounter++;
		hLayoutCounter++;
		spinBoxesCounter++;

		sideMenuLabelsVector[labelsCounter]->setText("Oś Y");
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuLabelsVector[labelsCounter]);
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuButtonsVector[buttonsCounter]);
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuLineEditsVector[lineEditsCounter]);
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuButtonsVector[buttonsCounter + 1]);
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuSpinBoxesVector[spinBoxesCounter]);

		lineEditsCounter++;
		buttonsCounter++;
		buttonsCounter++;
		labelsCounter++;
		hLayoutCounter++;
		spinBoxesCounter++;

		sideMenuLabelsVector[labelsCounter]->setText("Oś Z");
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuLabelsVector[labelsCounter]);
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuButtonsVector[buttonsCounter]);
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuLineEditsVector[lineEditsCounter]);
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuButtonsVector[buttonsCounter + 1]);
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuSpinBoxesVector[spinBoxesCounter]);

		lineEditsCounter++;
		buttonsCounter++;
		buttonsCounter++;
		labelsCounter++;
		hLayoutCounter++;
		spinBoxesCounter++;
	}
	
	/*parametru qspinbox*/
	for (int i = 0; i < sideMenuSpinBoxesVector.size(); i++) {
		setQSpinBoxSettings(sideMenuSpinBoxesVector[i]);
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

			QSignalMapper *mapper = new QSignalMapper(this);
			QObject::connect(mapper, SIGNAL(mapped(QString)), this, SLOT(rotate(QString)));
			mapper->setMapping(sideMenuButtonsVector[(i * 6) + j], str);
			connect(sideMenuButtonsVector[(i * 6) + j], SIGNAL(released()), mapper, SLOT(map()));
		}
	}

	//odswiezenie wektora limits
	int frameNumber = comboBoxFrameSelector->currentIndex();
	glWidgetsVector[frameNumber]->setLimitsVector(glWidgetsVector[frameNumber]->limits, glWidgetsVector[frameNumber]->bonesConf);

	/*wylaczenie przyciskow*/
	for (int i = 0; i < sideMenuButtonsVector.size()/2; i++) {
		vector<pf::range2> limits = glWidgetsVector[comboBoxFrameSelector->currentIndex()]->limits;
		if (disableRotatingButtons(limits, i+3)){
			sideMenuButtonsVector[i*2]->setDisabled(true);
			sideMenuButtonsVector[i * 2 + 1]->setDisabled(true); 
		}
		else {
			sideMenuButtonsVector[i * 2]->setToolTip("<b>min:</b> " + QString::number(limits[i + 3].min));
			sideMenuButtonsVector[i * 2 + 1]->setToolTip("<b>max:</b> " + QString::number(limits[i + 3].max));
		}
	}

	for (int lineEdits = 0; lineEdits < sideMenuLineEditsVector.size(); lineEdits++)
		sideMenuLineEditsVector[lineEdits]->setReadOnly(true);

	for (int hLayouts = 0; hLayouts < sideMenuHLayoutsVector.size(); hLayouts++)
		vboxLayout->addLayout(sideMenuHLayoutsVector[hLayouts]);

	refreshSideMenu();
}

bool MainWindow::disableRotatingButtons(vector<pf::range2> limits, int id) {

	if ((limits[id].max == 0) && (limits[id].min == 0)) 
		return true;
	
	else return false;
}

void MainWindow::addFrameScrollingMenu(QPushButton *button, int id) {
	frameScrollingMenu.push_back(new QMenu());
	manageBonesVector.push_back(new QAction("Kości modelu"));
	frameScrollingMenu[id]->addAction(manageBonesVector[id]);
	button->setMenu(frameScrollingMenu[id]);
}

void MainWindow::setQSpinBoxSettings(QSpinBox * qspinbox) {
	qspinbox->setRange(0, 20);
	qspinbox->setValue(5);
}