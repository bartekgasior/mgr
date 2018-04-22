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
    gridLayout = new QGridLayout;
    vboxLayout = new QVBoxLayout;
	//sideGridLayout = new QGridLayout;
    gridChildLayout = new QGridLayout;
	
    windowLayout = new QWidget;

    scrollWidget = new QWidget;
	scrollSideWidget = new QWidget;

    scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollWidget->setLayout(gridChildLayout);

	scrollSideArea = new QScrollArea;
	scrollSideArea->setWidgetResizable(true);
	scrollSideWidget->setLayout(vboxLayout);

    globalSlider = new QSlider;
    globalSlider->setVisible(false);
    addFrameButton = new QPushButton;
    addFrameButton->setVisible(false);

	comboBoxFrameSelector = new QComboBox;
	connect(comboBoxFrameSelector, SIGNAL(activated(int)), this, SLOT(reloadGLWidgetMenu()));

    ui->setupUi(this);

    QMenuBar * menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    QMenu * menu;
    menu = menuBar->addMenu("&Plik");
    menu->addSeparator();

    QAction * action;
    action = new QAction("Wczytaj pliki", this);
    connect(action, &QAction::triggered, this, &MainWindow::loadFiles);
    menu->addAction(action);

    connect(addFrameButton ,SIGNAL(released()), this, SLOT(buttonAddFramePressed()));
    connect(globalSlider, SIGNAL(valueChanged(int)), this, SLOT(updateGlobalListIterator(int)));

    scrollArea->setWidget(scrollWidget);
	scrollSideArea->setWidget(scrollSideWidget);

    gridLayout->addWidget(addFrameButton,0,0,1,1);
    gridLayout->addWidget(globalSlider, 0,1,1,8);
	gridLayout->addWidget(scrollArea, 1, 0, 9, 9);
	gridLayout->addWidget(scrollSideArea, 0, 9, 10, 2);
    
    windowLayout->setLayout(gridLayout);

    setCentralWidget(windowLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadFiles(){
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Img folder"));
    if(!folderPath.isEmpty()){
        QDir dir(folderPath);
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
		
    }
}

void MainWindow::prepareFrameMenu(int i){
    //globalListIterator = 1;

    QString imageFile = list[0].absoluteFilePath();
    pixVector[i]->load(imageFile);
    picLabelsVector[i]->setPixmap(*pixVector[i]);
    picLabelsVector[i]->setScaledContents(true);
    picLabelsVector[i]->setSizeIncrement(QSizePolicy::Ignored, QSizePolicy::Ignored);

    setTextIteratorLabel(countersVector[i], globalListIterator, list.size());

    imagesListIterator[i] = globalListIterator;

    mapIncreaseButtonSlot(i);
    mapReduceButtonSlot(i);
    mapMaxButtonSlot(i);
    mapMinButtonSlot(i);
}

void MainWindow::increaseIteratorPressed(int i){
    int listIterator = imagesListIterator[i];
    if(listIterator < list.size())
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

void MainWindow::reduceIteratorPressed(int i){
    int listIterator = imagesListIterator[i];
    if(listIterator > 1)
        listIterator--;
    else
        listIterator = 1;

    setTextIteratorLabel(countersVector[i], listIterator, list.size());
    imagesListIterator[i] = listIterator;
    /*QString imageFile = list[listIterator-1].absoluteFilePath();
    setImage1Label(ui->label_pic1, imageFile);*/
}

void MainWindow::minIteratorPressed(int i){
    imagesListIterator[i] = 1;
    setTextIteratorLabel(countersVector[i], imagesListIterator[i], list.size());

   /* QString imageFile = list[listIterator-1].absoluteFilePath();
    setImage1Label(ui->label_pic1, imageFile);*/
}

void MainWindow::maxIteratorPressed(int i){
    imagesListIterator[i] = list.size();
    setTextIteratorLabel(countersVector[i], imagesListIterator[i], list.size());

   /*QString imageFile = list[listIterator-1].absoluteFilePath();
    setImage1Label(ui->label_pic1, imageFile);*/
}

void MainWindow::setTextIteratorLabel(QLabel *label, int iterator, int listSize){
    QString string = QString("%1 / %2").arg(iterator).arg(listSize);
    label->setText(string);
}

void MainWindow::setImage1Label(QLabel *label, QString file){
    pix.load(file);
    label->setPixmap(pix);
    label->setScaledContents(true);
    label->setSizeIncrement(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

void MainWindow::prepareGlobalSlider(QSlider *slider){
    slider->setOrientation(Qt::Horizontal);
    slider->setMaximum(list.size());
	slider->setMinimum(1);
    slider->setSingleStep(1);
    slider->setVisible(true);
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
    pixVector.push_back(new QPixmap());
    picLabelsVector.push_back(new QLabel());
    imagesListIterator.push_back(0);

    glWidgetsVector[addFrameButtonCounter]->setMinimumSize(100,300);
    glWidgetsVector[addFrameButtonCounter]->setMaximumHeight(400);
    glWidgetsVector[addFrameButtonCounter]->setMaximumWidth(1200);

    checkBoxesVector[addFrameButtonCounter]->setChecked(true);

    prepareFrameMenu(addFrameButtonCounter);
    /*rows, cols, rowspan, colspan*/
    if(i%2==0){
        if(i>1) j=(i/2)*2;
        gridChildLayout->addWidget(borderWidget, (i/2)*10+j, 0, 12, 10);
        gridChildLayout->addWidget(glWidgetsVector[addFrameButtonCounter], (i/2)*10+j, 0, 10, 10);
        //gridChildLayout->addWidget(picLabelsVector[addFrameButtonCounter], (i/2)*10+j, 0, 10, 10); // tu nie dziala
        gridChildLayout->addWidget(checkBoxesVector[addFrameButtonCounter], (i/2)*10+11+j, 0, 1, 1);
        gridChildLayout->addWidget(minButtonsVector[addFrameButtonCounter], (i/2)*10+11+j, 1, 1, 1);
        gridChildLayout->addWidget(reduceButtonsVector[addFrameButtonCounter], (i/2)*10+11+j, 2, 1, 1);
        gridChildLayout->addWidget(increaseButtonsVector[addFrameButtonCounter], (i/2)*10+11+j, 7, 1, 1);
        gridChildLayout->addWidget(maxButtonsVector[addFrameButtonCounter], (i/2)*10+11+j, 8, 1, 1);
        gridChildLayout->addWidget(countersVector[addFrameButtonCounter], (i/2)*10+11+j, 5, 1, 2);
    }else {
        if(i>1) j=(i/2)*2;
        gridChildLayout->addWidget(borderWidget, (i/2)*10+j, 11, 12, 10);
        gridChildLayout->addWidget(glWidgetsVector[addFrameButtonCounter], (i/2)*10+j, 11, 10, 10);
        //gridChildLayout->addWidget(picVector[addFrameButtonCounter], (i/2)*10+j, 11, 10, 10);
        gridChildLayout->addWidget(checkBoxesVector[addFrameButtonCounter], (i/2)*10+11+j, 11, 1, 1);
        gridChildLayout->addWidget(minButtonsVector[addFrameButtonCounter], (i/2)*10+11+j, 12, 1, 1);
        gridChildLayout->addWidget(reduceButtonsVector[addFrameButtonCounter], (i/2)*10+11+j, 13, 1, 1);
        gridChildLayout->addWidget(increaseButtonsVector[addFrameButtonCounter], (i/2)*10+11+j, 18, 1, 1);
        gridChildLayout->addWidget(maxButtonsVector[addFrameButtonCounter], (i/2)*10+11+j, 19, 1, 1);
        gridChildLayout->addWidget(countersVector[addFrameButtonCounter], (i/2)*10+11+j, 16, 1, 2);
    }
	QString imgString = list[globalListIterator-1].absoluteFilePath();
	glWidgetsVector[addFrameButtonCounter]->imgPath = imgString.toUtf8().constData();
	update();
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

void MainWindow::rotate(QString str) {
	QStringList values = str.split(";");
	if (values.count() == 4) {
		string boneName = values.at(0).toStdString();
		float direction = values.at(1).toFloat();
		string axis = values.at(2).toStdString();
		int qLineEditID = values.at(3).toInt();
		if (axis == "X")
			glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate(boneName, direction, pf::Model3D::axisX);
		if (axis == "Y")
			glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate(boneName, direction, pf::Model3D::axisY);
		if (axis == "Z")
			glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate(boneName, direction, pf::Model3D::axisZ);

		sideMenuLineEditsVector[qLineEditID]->setText(QString::number(glWidgetsVector[comboBoxFrameSelector->currentIndex()]->modelState[0][qLineEditID+3]));
	}

	else {
		//Okno błedu
		cout << "BLAD" << endl;
	}
}

void MainWindow::reloadGLWidgetMenu() {
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
	
	sideMenuLabelsVector[0]->setText("Wybierz okno");
	sideMenuHLayoutsVector[0]->addWidget(sideMenuLabelsVector[0]);
	sideMenuHLayoutsVector[0]->addWidget(comboBoxFrameSelector);

	int usedBonesSize = glWidgetsVector[comboBoxFrameSelector->currentIndex()]->usedBones.size();
	vector<string> usedBones = glWidgetsVector[comboBoxFrameSelector->currentIndex()]->usedBones;

	for (int i = 0; i < usedBonesSize * 4; i++)
		sideMenuHLayoutsVector.push_back(new QHBoxLayout());
	
	for (int i = 0; i < usedBonesSize * 4; i++)
		sideMenuLabelsVector.push_back(new QLabel());

	for (int i = 0; i < usedBonesSize * 6; i++)
		sideMenuButtonsVector.push_back(new QPushButton());

	for (int i = 0; i < usedBonesSize * 3; i++)
		sideMenuLineEditsVector.push_back(new QLineEdit());

	int hLayoutCounter = 1, labelsCounter = 1, buttonsCounter = 0, lineEditsCounter = 0;
	for (int i = 1; i <= usedBonesSize; i++) {
		sideMenuLabelsVector[labelsCounter]->setText("Obrót " + QString::fromStdString(usedBones[i - 1]));
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuLabelsVector[labelsCounter]);
		labelsCounter++;
		hLayoutCounter++;

		sideMenuLabelsVector[labelsCounter]->setText("Oś X");
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuLabelsVector[labelsCounter]);
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuButtonsVector[buttonsCounter]);
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuLineEditsVector[lineEditsCounter]);
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuButtonsVector[buttonsCounter+1]);
		lineEditsCounter++;
		buttonsCounter++;
		buttonsCounter++;
		labelsCounter++;
		hLayoutCounter++;

		sideMenuLabelsVector[labelsCounter]->setText("Oś Y");
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuLabelsVector[labelsCounter]);
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuButtonsVector[buttonsCounter]);
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuLineEditsVector[lineEditsCounter]);
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuButtonsVector[buttonsCounter + 1]);
		lineEditsCounter++;
		buttonsCounter++;
		buttonsCounter++;
		labelsCounter++;
		hLayoutCounter++;

		sideMenuLabelsVector[labelsCounter]->setText("Oś Z");
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuLabelsVector[labelsCounter]);
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuButtonsVector[buttonsCounter]);
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuLineEditsVector[lineEditsCounter]);
		sideMenuHLayoutsVector[hLayoutCounter]->addWidget(sideMenuButtonsVector[buttonsCounter + 1]);
		lineEditsCounter++;
		buttonsCounter++;
		buttonsCounter++;
		labelsCounter++;
		hLayoutCounter++;
	}
	
	for (int i = 0; i < usedBonesSize; i++) {
		//kazda z kosci moze byc obracana w 3 osiach w 2 strony
		for (int j = 0; j < 6; j++) {
			QString str;
			switch (j) {
			case 0:
				str = QString::fromStdString(usedBones[i] + ";-1.0;X");
				break;

			case 1:
				str = QString::fromStdString(usedBones[i] + ";1.0;X");
				break;

			case 2:
				str = QString::fromStdString(usedBones[i] + ";-1.0;Y");
				break;

			case 3:
				str = QString::fromStdString(usedBones[i] + ";1.0;Y");
				break;

			case 4:
				str = QString::fromStdString(usedBones[i] + ";-1.0;Z");
				break;

			case 5:
				str = QString::fromStdString(usedBones[i] + ";1.0;Z");
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
			str += QString::fromStdString(";"+std::to_string(i*3+tmpVal));
			cout << str.toUtf8().constData() << endl;
			QSignalMapper *mapper = new QSignalMapper(this);
			QObject::connect(mapper, SIGNAL(mapped(QString)), this, SLOT(rotate(QString)));
			mapper->setMapping(sideMenuButtonsVector[(i * 6) + j], str);
			connect(sideMenuButtonsVector[(i * 6) + j], SIGNAL(released()), mapper, SLOT(map()));
		}
	}

	for (int lineEdits = 0; lineEdits < sideMenuLineEditsVector.size(); lineEdits++)
		sideMenuLineEditsVector[lineEdits]->setReadOnly(true);

	for (int hLayouts = 0; hLayouts < sideMenuHLayoutsVector.size(); hLayouts++)
		vboxLayout->addLayout(sideMenuHLayoutsVector[hLayouts]);

	reloadGLWidgetMenu();
}