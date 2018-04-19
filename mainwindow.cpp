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
		addGLWidgetMenu();
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

	QString imgString = list[listIterator-1].absoluteFilePath();
	glWidgetsVector[i]->imgPath = imgString.toUtf8().constData();
	update();

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

void MainWindow::rotateModelXUp() {
	//idx od 0
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("root", 1.0, pf::Model3D::axisX);
}

void MainWindow::rotateModelXDown() {
	//idx od 0
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("root", -1.0, pf::Model3D::axisX);
}

void MainWindow::rotateModelYLeft() {
	//idx od 0
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("root", 1.0, pf::Model3D::axisY);
}

void MainWindow::rotateModelYRight() {
	//idx od 0
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("root", -1.0, pf::Model3D::axisY);
}

/*LeftUpLeg*/

void MainWindow::rotateLeftUpLegX1() {
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("LeftUpLeg", 1.0, pf::Model3D::axisX);
}

void MainWindow::rotateLeftUpLegX2() {
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("LeftUpLeg", -1.0, pf::Model3D::axisX);
}

void MainWindow::rotateLeftUpLegY1() {
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("LeftUpLeg", 1.0, pf::Model3D::axisY);
}

void MainWindow::rotateLeftUpLegY2() {
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("LeftUpLeg", -1.0, pf::Model3D::axisY);
}

void MainWindow::rotateLeftUpLegZ1() {
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("LeftUpLeg", 1.0, pf::Model3D::axisZ);
}

void MainWindow::rotateLeftUpLegZ2() {
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("LeftUpLeg", -1.0, pf::Model3D::axisZ);
}

/*LeftLeg*/

void MainWindow::rotateLeftLegX1() {
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("LeftLeg", 1.0, pf::Model3D::axisX);
}

void MainWindow::rotateLeftLegX2() {
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("LeftLeg", -1.0, pf::Model3D::axisX);
}

void MainWindow::rotateLeftLegY1() {
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("LeftLeg", 1.0, pf::Model3D::axisY);
}

void MainWindow::rotateLeftLegY2() {
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("LeftLeg", -1.0, pf::Model3D::axisY);
}

void MainWindow::rotateLeftLegZ1() {
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("LeftLeg", 1.0, pf::Model3D::axisZ);
}

void MainWindow::rotateLeftLegZ2() {
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("LeftLeg", -1.0, pf::Model3D::axisZ);
}

/*RightUpLeg*/

void MainWindow::rotateRightUpLegX1() {
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("RightUpLeg", 1.0, pf::Model3D::axisX);
}

void MainWindow::rotateRightUpLegX2() {
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("RightUpLeg", -1.0, pf::Model3D::axisX);
}

void MainWindow::rotateRightUpLegY1() {
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("RightUpLeg", 1.0, pf::Model3D::axisY);
}

void MainWindow::rotateRightUpLegY2() {
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("RightUpLeg", -1.0, pf::Model3D::axisY);
}

void MainWindow::rotateRightUpLegZ1() {
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("RightUpLeg", 1.0, pf::Model3D::axisZ);
}

void MainWindow::rotateRightUpLegZ2() {
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("RightUpLeg", -1.0, pf::Model3D::axisZ);
}

/*RightLeg*/

void MainWindow::rotateRightLegX1() {
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("RightLeg", 1.0, pf::Model3D::axisX);
}

void MainWindow::rotateRightLegX2() {
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("RightLeg", -1.0, pf::Model3D::axisX);
}

void MainWindow::rotateRightLegY1() {
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("RightLeg", 1.0, pf::Model3D::axisY);
}

void MainWindow::rotateRightLegY2() {
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("RightLeg", -1.0, pf::Model3D::axisY);
}

void MainWindow::rotateRightLegZ1() {
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("RightLeg", 1.0, pf::Model3D::axisZ);
}

void MainWindow::rotateRightLegZ2() {
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotate("RightLeg", -1.0, pf::Model3D::axisZ);
}


void MainWindow::addGLWidgetMenu() {
	vboxLayout->setAlignment(Qt::AlignTop);
	QWidget *borderWidget = new QWidget();
	borderWidget->setStyleSheet("border: 1px solid gray");

	QFrame *frame = new QFrame();
	frame->setFrameShape(QFrame::Panel);
	frame->setFrameShadow(QFrame::Raised);
	//sideGridLayout->addWidget(borderWidget, 0, 0, 20, 10);

	QLabel *comboBoxLabel = new QLabel();
	comboBoxLabel->setText("Wybierz okno");
	QHBoxLayout *selectFrameLayout = new QHBoxLayout();
	//sideGridLayout->addWidget(comboBoxLabel, 0, 8, 1, 1);
	//sideGridLayout->addWidget(comboBoxFrameSelector, 0, 9, 1, 1);
	selectFrameLayout->addWidget(comboBoxLabel);
	selectFrameLayout->addWidget(comboBoxFrameSelector);
	vboxLayout->addLayout(selectFrameLayout);

	QHBoxLayout *modelRootLayout = new QHBoxLayout();
	QHBoxLayout *modelRootXLayout = new QHBoxLayout();
	QHBoxLayout *modelRootYLayout = new QHBoxLayout();

	QLabel *rootLabel = new QLabel();
	rootLabel->setText("Obrót modelu");
	modelRootLayout->addWidget(rootLabel);
	vboxLayout->addLayout(modelRootLayout);

	//buttony do obracania modelu po osi X
	QLabel *rotateModelXLabel = new QLabel();
	rotateModelXLabel->setText("Oś X");
	QPushButton *rotateModelXUpButton = new QPushButton();
	QPushButton *rotateModelXDownButton = new QPushButton();
	modelRootXLayout->addWidget(rotateModelXLabel);
	modelRootXLayout->addWidget(rotateModelXUpButton);
	modelRootXLayout->addWidget(rotateModelXDownButton);
	connect(rotateModelXUpButton, SIGNAL(released()), this, SLOT(rotateModelXUp()));
	connect(rotateModelXDownButton, SIGNAL(released()), this, SLOT(rotateModelXDown()));
	vboxLayout->addLayout(modelRootXLayout);

	//obrot modelu w osi Y
	QLabel *rotateModelYLabel = new QLabel();
	rotateModelYLabel->setText("Oś Y");
	QPushButton *rotateModelYLeftButton = new QPushButton();
	QPushButton *rotateModelYRightButton = new QPushButton();
	modelRootYLayout->addWidget(rotateModelYLabel);
	modelRootYLayout->addWidget(rotateModelYLeftButton);
	modelRootYLayout->addWidget(rotateModelYRightButton);
	connect(rotateModelYLeftButton, SIGNAL(released()), this, SLOT(rotateModelYLeft()));
	connect(rotateModelYRightButton, SIGNAL(released()), this, SLOT(rotateModelYRight()));
	vboxLayout->addLayout(modelRootYLayout);
	/*
	Operacje na górnej części lewej nogi
	*/
	QHBoxLayout *leftUpLegLayout = new QHBoxLayout();
	QHBoxLayout *leftUpLegXLayout = new QHBoxLayout();
	QHBoxLayout *leftUpLegYLayout = new QHBoxLayout();
	QHBoxLayout *leftUpLegZLayout = new QHBoxLayout();

	QLabel *leftUpLegLabel = new QLabel();
	leftUpLegLabel->setText("Obrót LeftUpLeg");
	leftUpLegLabel->setAlignment(Qt::AlignCenter);
	leftUpLegLayout->addWidget(leftUpLegLabel);
	vboxLayout->addLayout(leftUpLegLayout);

	/*oś X*/
	QLabel *rotateLeftUpLegXLabel = new QLabel();
	rotateLeftUpLegXLabel->setText("Oś X");
	QPushButton *rotateLeftUpLegX1Button = new QPushButton();
	QPushButton *rotateLeftUpLegX2Button = new QPushButton();
	leftUpLegXLayout->addWidget(rotateLeftUpLegXLabel);
	leftUpLegXLayout->addWidget(rotateLeftUpLegX1Button);
	leftUpLegXLayout->addWidget(rotateLeftUpLegX2Button);
	connect(rotateLeftUpLegX1Button, SIGNAL(released()), this, SLOT(rotateLeftUpLegX1()));
	connect(rotateLeftUpLegX2Button, SIGNAL(released()), this, SLOT(rotateLeftUpLegX2()));
	vboxLayout->addLayout(leftUpLegXLayout);

	/*oś Y*/
	QLabel *rotateLeftUpLegYLabel = new QLabel();
	rotateLeftUpLegYLabel->setText("Oś Y");
	QPushButton *rotateLeftUpLegY1Button = new QPushButton();
	QPushButton *rotateLeftUpLegY2Button = new QPushButton();
	leftUpLegYLayout->addWidget(rotateLeftUpLegYLabel);
	leftUpLegYLayout->addWidget(rotateLeftUpLegY1Button);
	leftUpLegYLayout->addWidget(rotateLeftUpLegY2Button);
	connect(rotateLeftUpLegY1Button, SIGNAL(released()), this, SLOT(rotateLeftUpLegY1()));
	connect(rotateLeftUpLegY2Button, SIGNAL(released()), this, SLOT(rotateLeftUpLegY2()));
	vboxLayout->addLayout(leftUpLegYLayout);

	/*oś Z*/
	QLabel *rotateLeftUpLegZLabel = new QLabel();
	rotateLeftUpLegZLabel->setText("Oś Z");
	QPushButton *rotateLeftUpLegZ1Button = new QPushButton();
	QPushButton *rotateLeftUpLegZ2Button = new QPushButton();
	leftUpLegZLayout->addWidget(rotateLeftUpLegZLabel);
	leftUpLegZLayout->addWidget(rotateLeftUpLegZ1Button);
	leftUpLegZLayout->addWidget(rotateLeftUpLegZ2Button);
	connect(rotateLeftUpLegZ1Button, SIGNAL(released()), this, SLOT(rotateLeftUpLegZ1()));
	connect(rotateLeftUpLegZ2Button, SIGNAL(released()), this, SLOT(rotateLeftUpLegZ2()));
	vboxLayout->addLayout(leftUpLegZLayout);

	/*
	Operacje na dolnej części lewej nogi - LeftLeg
	*/

	QHBoxLayout *leftLegLayout = new QHBoxLayout();
	QHBoxLayout *leftLegXLayout = new QHBoxLayout();
	QHBoxLayout *leftLegYLayout = new QHBoxLayout();
	QHBoxLayout *leftLegZLayout = new QHBoxLayout();

	QLabel *leftLegLabel = new QLabel();
	leftLegLabel->setText("Obrót LeftLeg");
	leftLegLabel->setAlignment(Qt::AlignCenter);
	leftLegLayout->addWidget(leftLegLabel);
	vboxLayout->addLayout(leftLegLayout);

	/*oś X*/
	QLabel *rotateLeftLegXLabel = new QLabel();
	rotateLeftLegXLabel->setText("Oś X");
	QPushButton *rotateLeftLegX1Button = new QPushButton();
	QPushButton *rotateLeftLegX2Button = new QPushButton();
	leftLegXLayout->addWidget(rotateLeftLegXLabel);
	leftLegXLayout->addWidget(rotateLeftLegX1Button);
	leftLegXLayout->addWidget(rotateLeftLegX2Button);
	connect(rotateLeftLegX1Button, SIGNAL(released()), this, SLOT(rotateLeftLegX1()));
	connect(rotateLeftLegX2Button, SIGNAL(released()), this, SLOT(rotateLeftLegX2()));
	vboxLayout->addLayout(leftLegXLayout);

	/*oś Y*/
	QLabel *rotateLeftLegYLabel = new QLabel();
	rotateLeftLegYLabel->setText("Oś Y");
	QPushButton *rotateLeftLegY1Button = new QPushButton();
	QPushButton *rotateLeftLegY2Button = new QPushButton();
	leftLegYLayout->addWidget(rotateLeftLegYLabel);
	leftLegYLayout->addWidget(rotateLeftLegY1Button);
	leftLegYLayout->addWidget(rotateLeftLegY2Button);
	connect(rotateLeftLegY1Button, SIGNAL(released()), this, SLOT(rotateLeftLegY1()));
	connect(rotateLeftLegY2Button, SIGNAL(released()), this, SLOT(rotateLeftLegY2()));
	vboxLayout->addLayout(leftLegYLayout);

	/*oś Z*/
	QLabel *rotateLeftLegZLabel = new QLabel();
	rotateLeftLegZLabel->setText("Oś Z");
	QPushButton *rotateLeftLegZ1Button = new QPushButton();
	QPushButton *rotateLeftLegZ2Button = new QPushButton();
	leftLegZLayout->addWidget(rotateLeftLegZLabel);
	leftLegZLayout->addWidget(rotateLeftLegZ1Button);
	leftLegZLayout->addWidget(rotateLeftLegZ2Button);
	connect(rotateLeftLegZ1Button, SIGNAL(released()), this, SLOT(rotateLeftLegZ1()));
	connect(rotateLeftLegZ2Button, SIGNAL(released()), this, SLOT(rotateLeftLegZ2()));
	vboxLayout->addLayout(leftLegZLayout);

	/*
	Operacje na górnej części prawej nogi - RightUpLeg
	*/

	QHBoxLayout *rightUpLegLayout = new QHBoxLayout();
	QHBoxLayout *rightUpLegXLayout = new QHBoxLayout();
	QHBoxLayout *rightUpLegYLayout = new QHBoxLayout();
	QHBoxLayout *rightUpLegZLayout = new QHBoxLayout();

	QLabel *rightUpLegLabel = new QLabel();
	rightUpLegLabel->setText("Obrót RightUpLeg");
	rightUpLegLabel->setAlignment(Qt::AlignCenter);
	rightUpLegLayout->addWidget(rightUpLegLabel);
	vboxLayout->addLayout(rightUpLegLayout);

	/*oś X*/
	QLabel *rotateRightUpLegXLabel = new QLabel();
	rotateRightUpLegXLabel->setText("Oś X");
	QPushButton *rotateRightUpLegX1Button = new QPushButton();
	QPushButton *rotateRightUpLegX2Button = new QPushButton();
	rightUpLegXLayout->addWidget(rotateRightUpLegXLabel);
	rightUpLegXLayout->addWidget(rotateRightUpLegX1Button);
	rightUpLegXLayout->addWidget(rotateRightUpLegX2Button);
	connect(rotateRightUpLegX1Button, SIGNAL(released()), this, SLOT(rotateRightUpLegX1()));
	connect(rotateRightUpLegX2Button, SIGNAL(released()), this, SLOT(rotateRightUpLegX2()));
	vboxLayout->addLayout(rightUpLegXLayout);

	/*oś Y*/
	QLabel *rotateRightUpLegYLabel = new QLabel();
	rotateRightUpLegYLabel->setText("Oś Y");
	QPushButton *rotateRightUpLegY1Button = new QPushButton();
	QPushButton *rotateRightUpLegY2Button = new QPushButton();
	rightUpLegYLayout->addWidget(rotateRightUpLegYLabel);
	rightUpLegYLayout->addWidget(rotateRightUpLegY1Button);
	rightUpLegYLayout->addWidget(rotateRightUpLegY2Button);
	connect(rotateRightUpLegY1Button, SIGNAL(released()), this, SLOT(rotateRightUpLegY1()));
	connect(rotateRightUpLegY2Button, SIGNAL(released()), this, SLOT(rotateRightUpLegY2()));
	vboxLayout->addLayout(rightUpLegYLayout);

	/*oś Z*/
	QLabel *rotateRightUpLegZLabel = new QLabel();
	rotateRightUpLegZLabel->setText("Oś Z");
	QPushButton *rotateRightUpLegZ1Button = new QPushButton();
	QPushButton *rotateRightUpLegZ2Button = new QPushButton();
	rightUpLegZLayout->addWidget(rotateRightUpLegZLabel);
	rightUpLegZLayout->addWidget(rotateRightUpLegZ1Button);
	rightUpLegZLayout->addWidget(rotateRightUpLegZ2Button);
	connect(rotateRightUpLegZ1Button, SIGNAL(released()), this, SLOT(rotateRightUpLegZ1()));
	connect(rotateRightUpLegZ2Button, SIGNAL(released()), this, SLOT(rotateRightUpLegZ2()));
	vboxLayout->addLayout(rightUpLegZLayout);

	/*
	Operacje na dolnej części prawej nogi - RightLeg
	*/

	QHBoxLayout *rightLegLayout = new QHBoxLayout();
	QHBoxLayout *rightLegXLayout = new QHBoxLayout();
	QHBoxLayout *rightLegYLayout = new QHBoxLayout();
	QHBoxLayout *rightLegZLayout = new QHBoxLayout();

	QLabel *rightLegLabel = new QLabel();
	rightLegLabel->setText("Obrót RightLeg");
	rightLegLabel->setAlignment(Qt::AlignCenter);
	rightLegLayout->addWidget(rightLegLabel);
	vboxLayout->addLayout(rightLegLayout);

	/*oś X*/
	QLabel *rotateRightLegXLabel = new QLabel();
	rotateRightLegXLabel->setText("Oś X");
	QPushButton *rotateRightLegX1Button = new QPushButton();
	QPushButton *rotateRightLegX2Button = new QPushButton();
	rightLegXLayout->addWidget(rotateRightLegXLabel);
	rightLegXLayout->addWidget(rotateRightLegX1Button);
	rightLegXLayout->addWidget(rotateRightLegX2Button);
	connect(rotateRightLegX1Button, SIGNAL(released()), this, SLOT(rotateRightLegX1()));
	connect(rotateRightLegX2Button, SIGNAL(released()), this, SLOT(rotateRightLegX2()));
	vboxLayout->addLayout(rightLegXLayout);

	/*oś Y*/
	QLabel *rotateRightLegYLabel = new QLabel();
	rotateRightLegYLabel->setText("Oś Y");
	QPushButton *rotateRightLegY1Button = new QPushButton();
	QPushButton *rotateRightLegY2Button = new QPushButton();
	rightLegYLayout->addWidget(rotateRightLegYLabel);
	rightLegYLayout->addWidget(rotateRightLegY1Button);
	rightLegYLayout->addWidget(rotateRightLegY2Button);
	connect(rotateRightLegY1Button, SIGNAL(released()), this, SLOT(rotateRightLegY1()));
	connect(rotateRightLegY2Button, SIGNAL(released()), this, SLOT(rotateRightLegY2()));
	vboxLayout->addLayout(rightLegYLayout);

	/*oś Z*/
	QLabel *rotateRightLegZLabel = new QLabel();
	rotateRightLegZLabel->setText("Oś Z");
	QPushButton *rotateRightLegZ1Button = new QPushButton();
	QPushButton *rotateRightLegZ2Button = new QPushButton();
	rightLegZLayout->addWidget(rotateRightLegZLabel);
	rightLegZLayout->addWidget(rotateRightLegZ1Button);
	rightLegZLayout->addWidget(rotateRightLegZ2Button);
	connect(rotateRightLegZ1Button, SIGNAL(released()), this, SLOT(rotateRightLegZ1()));
	connect(rotateRightLegZ2Button, SIGNAL(released()), this, SLOT(rotateRightLegZ2()));
	vboxLayout->addLayout(rightLegZLayout);

	/*
	Operacje na Spine1
	*/

	QHBoxLayout *spine1Layout = new QHBoxLayout();
	QHBoxLayout *spine1XLayout = new QHBoxLayout();
	QHBoxLayout *spine1YLayout = new QHBoxLayout();
	QHBoxLayout *spine1ZLayout = new QHBoxLayout();

	QLabel *spine1Label = new QLabel();
	spine1Label->setText("Obrót Spine1");
	spine1Label->setAlignment(Qt::AlignCenter);
	spine1Layout->addWidget(spine1Label);
	vboxLayout->addLayout(spine1Layout);

	/*oś X*/
	QLabel *rotateSpine1XLabel = new QLabel();
	rotateSpine1XLabel->setText("Oś X");
	QPushButton *rotateSpine1X1Button = new QPushButton();
	QPushButton *rotateSpine1X2Button = new QPushButton();
	spine1XLayout->addWidget(rotateSpine1XLabel);
	spine1XLayout->addWidget(rotateSpine1X1Button);
	spine1XLayout->addWidget(rotateSpine1X2Button);
	connect(rotateSpine1X1Button, SIGNAL(released()), this, SLOT(rotateSpine1X1()));
	connect(rotateSpine1X2Button, SIGNAL(released()), this, SLOT(rotateSpine1X2()));
	vboxLayout->addLayout(spine1XLayout);

	/*oś Y*/
	QLabel *rotateSpine1YLabel = new QLabel();
	rotateSpine1YLabel->setText("Oś Y");
	QPushButton *rotateSpine1Y1Button = new QPushButton();
	QPushButton *rotateSpine1Y2Button = new QPushButton();
	spine1YLayout->addWidget(rotateSpine1YLabel);
	spine1YLayout->addWidget(rotateSpine1Y1Button);
	spine1YLayout->addWidget(rotateSpine1Y2Button);
	connect(rotateSpine1Y1Button, SIGNAL(released()), this, SLOT(rotateSpine1Y1()));
	connect(rotateSpine1Y2Button, SIGNAL(released()), this, SLOT(rotateSpine1Y2()));
	vboxLayout->addLayout(spine1YLayout);

	/*oś Z*/
	QLabel *rotateSpine1YZLabel = new QLabel();
	rotateSpine1YZLabel->setText("Oś Z");
	QPushButton *rotateSpine1YZ1Button = new QPushButton();
	QPushButton *rotateSpine1YZ2Button = new QPushButton();
	spine1ZLayout->addWidget(rotateSpine1YZLabel);
	spine1ZLayout->addWidget(rotateSpine1YZ1Button);
	spine1ZLayout->addWidget(rotateSpine1YZ2Button);
	connect(rotateSpine1YZ1Button, SIGNAL(released()), this, SLOT(rotateSpine1YZ1()));
	connect(rotateSpine1YZ2Button, SIGNAL(released()), this, SLOT(rotateSpine1YZ2()));
	vboxLayout->addLayout(spine1ZLayout);

	/*
	Operacje na Head
	*/

	QHBoxLayout *headLayout = new QHBoxLayout();
	QHBoxLayout *headXLayout = new QHBoxLayout();
	QHBoxLayout *headYLayout = new QHBoxLayout();
	QHBoxLayout *headZLayout = new QHBoxLayout();

	QLabel *headLabel = new QLabel();
	headLabel->setText("Obrót Head");
	headLabel->setAlignment(Qt::AlignCenter);
	headLayout->addWidget(headLabel);
	vboxLayout->addLayout(headLayout);

	/*oś X*/
	QLabel *rotateHeadXLabel = new QLabel();
	rotateHeadXLabel->setText("Oś X");
	QPushButton *rotateHeadX1Button = new QPushButton();
	QPushButton *rotateHeadX2Button = new QPushButton();
	headXLayout->addWidget(rotateHeadXLabel);
	headXLayout->addWidget(rotateHeadX1Button);
	headXLayout->addWidget(rotateHeadX2Button);
	connect(rotateHeadX1Button, SIGNAL(released()), this, SLOT(rotateHeadX1()));
	connect(rotateHeadX2Button, SIGNAL(released()), this, SLOT(rotateHeadX2()));
	vboxLayout->addLayout(headXLayout);

	/*oś Y*/
	QLabel *rotateHeadYLabel = new QLabel();
	rotateHeadYLabel->setText("Oś Y");
	QPushButton *rotateHeadY1Button = new QPushButton();
	QPushButton *rotateHeadY2Button = new QPushButton();
	headYLayout->addWidget(rotateHeadYLabel);
	headYLayout->addWidget(rotateHeadY1Button);
	headYLayout->addWidget(rotateHeadY2Button);
	connect(rotateHeadY1Button, SIGNAL(released()), this, SLOT(rotateHeadY1()));
	connect(rotateHeadY2Button, SIGNAL(released()), this, SLOT(rotateHeadY2()));
	vboxLayout->addLayout(headYLayout);

	/*oś Z*/
	QLabel *rotateHeadYZLabel = new QLabel();
	rotateHeadYZLabel->setText("Oś Z");
	QPushButton *rotateHeadYZ1Button = new QPushButton();
	QPushButton *rotateHeadYZ2Button = new QPushButton();
	headZLayout->addWidget(rotateHeadYZLabel);
	headZLayout->addWidget(rotateHeadYZ1Button);
	headZLayout->addWidget(rotateHeadYZ2Button);
	connect(rotateHeadYZ1Button, SIGNAL(released()), this, SLOT(rotateHeadYZ1()));
	connect(rotateHeadYZ2Button, SIGNAL(released()), this, SLOT(rotateHeadYZ2()));
	vboxLayout->addLayout(headZLayout);

	/*
	Operacje na LeftArm
	*/

	QHBoxLayout *leftArmLayout = new QHBoxLayout();
	QHBoxLayout *leftArmXLayout = new QHBoxLayout();
	QHBoxLayout *leftArmYLayout = new QHBoxLayout();
	QHBoxLayout *leftArmZLayout = new QHBoxLayout();

	QLabel *leftArmLabel = new QLabel();
	leftArmLabel->setText("Obrót LeftArm");
	leftArmLabel->setAlignment(Qt::AlignCenter);
	leftArmLayout->addWidget(leftArmLabel);
	vboxLayout->addLayout(leftArmLayout);

	/*oś X*/
	QLabel *rotateLeftArmXLabel = new QLabel();
	rotateLeftArmXLabel->setText("Oś X");
	QPushButton *rotateLeftArmX1Button = new QPushButton();
	QPushButton *rotateLeftArmX2Button = new QPushButton();
	leftArmXLayout->addWidget(rotateLeftArmXLabel);
	leftArmXLayout->addWidget(rotateLeftArmX1Button);
	leftArmXLayout->addWidget(rotateLeftArmX2Button);
	connect(rotateLeftArmX1Button, SIGNAL(released()), this, SLOT(rotateLeftArmX1()));
	connect(rotateLeftArmX2Button, SIGNAL(released()), this, SLOT(rotateLeftArmX2()));
	vboxLayout->addLayout(leftArmXLayout);

	/*oś Y*/
	QLabel *rotateLeftArmYLabel = new QLabel();
	rotateLeftArmYLabel->setText("Oś Y");
	QPushButton *rotateLeftArmY1Button = new QPushButton();
	QPushButton *rotateLeftArmY2Button = new QPushButton();
	leftArmYLayout->addWidget(rotateLeftArmYLabel);
	leftArmYLayout->addWidget(rotateLeftArmY1Button);
	leftArmYLayout->addWidget(rotateLeftArmY2Button);
	connect(rotateLeftArmY1Button, SIGNAL(released()), this, SLOT(rotateLeftArmY1()));
	connect(rotateLeftArmY2Button, SIGNAL(released()), this, SLOT(rotateLeftArmY2()));
	vboxLayout->addLayout(leftArmYLayout);

	/*oś Z*/
	QLabel *rotateLeftArmYZLabel = new QLabel();
	rotateLeftArmYZLabel->setText("Oś Z");
	QPushButton *rotateLeftArmYZ1Button = new QPushButton();
	QPushButton *rotateLeftArmYZ2Button = new QPushButton();
	leftArmZLayout->addWidget(rotateLeftArmYZLabel);
	leftArmZLayout->addWidget(rotateLeftArmYZ1Button);
	leftArmZLayout->addWidget(rotateLeftArmYZ2Button);
	connect(rotateLeftArmYZ1Button, SIGNAL(released()), this, SLOT(rotateLeftArmYZ1()));
	connect(rotateLeftArmYZ2Button, SIGNAL(released()), this, SLOT(rotateLeftArmYZ2()));
	vboxLayout->addLayout(leftArmZLayout);

	/*
	Operacje na LeftForeArm
	*/

	QHBoxLayout *leftForeArmLayout = new QHBoxLayout();
	QHBoxLayout *leftForeArmXLayout = new QHBoxLayout();
	QHBoxLayout *leftForeArmYLayout = new QHBoxLayout();
	QHBoxLayout *leftForeArmZLayout = new QHBoxLayout();

	QLabel *leftForeArmLabel = new QLabel();
	leftForeArmLabel->setText("Obrót LeftAForeArm");
	leftForeArmLabel->setAlignment(Qt::AlignCenter);
	leftForeArmLayout->addWidget(leftForeArmLabel);
	vboxLayout->addLayout(leftForeArmLayout);

	/*oś X*/
	QLabel *rotateLeftForeArmXLabel = new QLabel();
	rotateLeftForeArmXLabel->setText("Oś X");
	QPushButton *rotateLeftForeArmX1Button = new QPushButton();
	QPushButton *rotateLeftForeArmX2Button = new QPushButton();
	leftForeArmXLayout->addWidget(rotateLeftForeArmXLabel);
	leftForeArmXLayout->addWidget(rotateLeftForeArmX1Button);
	leftForeArmXLayout->addWidget(rotateLeftForeArmX2Button);
	connect(rotateLeftForeArmX1Button, SIGNAL(released()), this, SLOT(rotateLeftForeArmX1()));
	connect(rotateLeftForeArmX2Button, SIGNAL(released()), this, SLOT(rotateLeftForeArmX2()));
	vboxLayout->addLayout(leftForeArmXLayout);

	/*oś Y*/
	QLabel *rotateLeftForeArmYLabel = new QLabel();
	rotateLeftForeArmYLabel->setText("Oś Y");
	QPushButton *rotateLeftForeArmY1Button = new QPushButton();
	QPushButton *rotateLeftForeArmY2Button = new QPushButton();
	leftForeArmYLayout->addWidget(rotateLeftForeArmYLabel);
	leftForeArmYLayout->addWidget(rotateLeftForeArmY1Button);
	leftForeArmYLayout->addWidget(rotateLeftForeArmY2Button);
	connect(rotateLeftForeArmY1Button, SIGNAL(released()), this, SLOT(rotateForeLeftArmY1()));
	connect(rotateLeftForeArmY2Button, SIGNAL(released()), this, SLOT(rotateForeLeftArmY2()));
	vboxLayout->addLayout(leftForeArmYLayout);

	/*oś Z*/
	QLabel *rotateLeftForeArmYZLabel = new QLabel();
	rotateLeftForeArmYZLabel->setText("Oś Z");
	QPushButton *rotateLeftForeArmYZ1Button = new QPushButton();
	QPushButton *rotateLeftForeArmYZ2Button = new QPushButton();
	leftForeArmZLayout->addWidget(rotateLeftForeArmYZLabel);
	leftForeArmZLayout->addWidget(rotateLeftForeArmYZ1Button);
	leftForeArmZLayout->addWidget(rotateLeftForeArmYZ2Button);
	connect(rotateLeftForeArmYZ1Button, SIGNAL(released()), this, SLOT(rotateLeftForeArmYZ1()));
	connect(rotateLeftForeArmYZ2Button, SIGNAL(released()), this, SLOT(rotateLeftForeArmYZ2()));
	vboxLayout->addLayout(leftForeArmZLayout);

	/*
	Operacje na RightArm
	*/

	QHBoxLayout *rightArmLayout = new QHBoxLayout();
	QHBoxLayout *rightArmXLayout = new QHBoxLayout();
	QHBoxLayout *rightArmYLayout = new QHBoxLayout();
	QHBoxLayout *rightArmZLayout = new QHBoxLayout();

	QLabel *rightArmLabel = new QLabel();
	rightArmLabel->setText("Obrót RightArm");
	rightArmLabel->setAlignment(Qt::AlignCenter);
	rightArmLayout->addWidget(rightArmLabel);
	vboxLayout->addLayout(rightArmLayout);

	/*oś X*/
	QLabel *rotateRightArmXLabel = new QLabel();
	rotateRightArmXLabel->setText("Oś X");
	QPushButton *rotateRightArmX1Button = new QPushButton();
	QPushButton *rotateRightArmX2Button = new QPushButton();
	rightArmXLayout->addWidget(rotateRightArmXLabel);
	rightArmXLayout->addWidget(rotateRightArmX1Button);
	rightArmXLayout->addWidget(rotateRightArmX2Button);
	connect(rotateRightArmX1Button, SIGNAL(released()), this, SLOT(rotateRightArmX1()));
	connect(rotateRightArmX2Button, SIGNAL(released()), this, SLOT(rotateRightArmX2()));
	vboxLayout->addLayout(rightArmXLayout);

	/*oś Y*/
	QLabel *rotateRightArmYLabel = new QLabel();
	rotateRightArmYLabel->setText("Oś Y");
	QPushButton *rotateRightArmY1Button = new QPushButton();
	QPushButton *rotateRightArmY2Button = new QPushButton();
	rightArmYLayout->addWidget(rotateRightArmYLabel);
	rightArmYLayout->addWidget(rotateRightArmY1Button);
	rightArmYLayout->addWidget(rotateRightArmY2Button);
	connect(rotateRightArmY1Button, SIGNAL(released()), this, SLOT(rotateRightArmY1()));
	connect(rotateRightArmY2Button, SIGNAL(released()), this, SLOT(rotateRightArmY2()));
	vboxLayout->addLayout(rightArmYLayout);

	/*oś Z*/
	QLabel *rotateRightArmYZLabel = new QLabel();
	rotateRightArmYZLabel->setText("Oś Z");
	QPushButton *rotateRightArmYZ1Button = new QPushButton();
	QPushButton *rotateRightArmYZ2Button = new QPushButton();
	rightArmZLayout->addWidget(rotateRightArmYZLabel);
	rightArmZLayout->addWidget(rotateRightArmYZ1Button);
	rightArmZLayout->addWidget(rotateRightArmYZ2Button);
	connect(rotateRightArmYZ1Button, SIGNAL(released()), this, SLOT(rotateRightArmYZ1()));
	connect(rotateRightArmYZ2Button, SIGNAL(released()), this, SLOT(rotateRightArmYZ2()));
	vboxLayout->addLayout(rightArmZLayout);

	/*
	Operacje na RightForeArm
	*/

	QHBoxLayout *rightForeArmLayout = new QHBoxLayout();
	QHBoxLayout *rightForeArmXLayout = new QHBoxLayout();
	QHBoxLayout *rightForeArmYLayout = new QHBoxLayout();
	QHBoxLayout *rightForeArmZLayout = new QHBoxLayout();

	QLabel *rightForeArmLabel = new QLabel();
	rightForeArmLabel->setText("Obrót RightForeArm");
	rightForeArmLabel->setAlignment(Qt::AlignCenter);
	rightForeArmLayout->addWidget(rightForeArmLabel);
	vboxLayout->addLayout(rightForeArmLayout);

	/*oś X*/
	QLabel *rotateRightForeArmXLabel = new QLabel();
	rotateRightForeArmXLabel->setText("Oś X");
	QPushButton *rotateRightForeArmX1Button = new QPushButton();
	QPushButton *rotateRightForeArmX2Button = new QPushButton();
	rightForeArmXLayout->addWidget(rotateRightForeArmXLabel);
	rightForeArmXLayout->addWidget(rotateRightForeArmX1Button);
	rightForeArmXLayout->addWidget(rotateRightForeArmX2Button);
	connect(rotateRightForeArmX1Button, SIGNAL(released()), this, SLOT(rotateRightForeArmX1()));
	connect(rotateRightForeArmX2Button, SIGNAL(released()), this, SLOT(rotateRightForeArmX2()));
	vboxLayout->addLayout(rightForeArmXLayout);

	/*oś Y*/
	QLabel *rotateRightForeArmYLabel = new QLabel();
	rotateRightForeArmYLabel->setText("Oś Y");
	QPushButton *rotateRightForeArmY1Button = new QPushButton();
	QPushButton *rotateRightForeArmY2Button = new QPushButton();
	rightForeArmYLayout->addWidget(rotateRightForeArmYLabel);
	rightForeArmYLayout->addWidget(rotateRightForeArmY1Button);
	rightForeArmYLayout->addWidget(rotateRightForeArmY2Button);
	connect(rotateRightForeArmY1Button, SIGNAL(released()), this, SLOT(rotateRightForeArmY1()));
	connect(rotateRightForeArmY2Button, SIGNAL(released()), this, SLOT(rotateRightForeArmY2()));
	vboxLayout->addLayout(rightForeArmYLayout);

	/*oś Z*/
	QLabel *rotateRightForeArmYZLabel = new QLabel();
	rotateRightForeArmYZLabel->setText("Oś Z");
	QPushButton *rotateRightForeArmYZ1Button = new QPushButton();
	QPushButton *rotateRightForeArmYZ2Button = new QPushButton();
	rightForeArmZLayout->addWidget(rotateRightForeArmYZLabel);
	rightForeArmZLayout->addWidget(rotateRightForeArmYZ1Button);
	rightForeArmZLayout->addWidget(rotateRightForeArmYZ2Button);
	connect(rotateRightForeArmYZ1Button, SIGNAL(released()), this, SLOT(rotateRightForeArmYZ1()));
	connect(rotateRightForeArmYZ2Button, SIGNAL(released()), this, SLOT(rotateRightForeArmYZ2()));
	vboxLayout->addLayout(rightForeArmZLayout);
}
