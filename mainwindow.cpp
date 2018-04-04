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
    gridChildLayout = new QGridLayout;
    windowLayout = new QWidget;

    scrollWidget = new QWidget;
    scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollWidget->setLayout(gridChildLayout);

    globalSlider = new QSlider;
    globalSlider->setVisible(false);
    addFrameButton = new QPushButton;
    addFrameButton->setVisible(false);

    ui->setupUi(this);

    //setMinimumSize(1600, 800);
    //resize(480, 320);

    QMenuBar * menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    QMenu * menu;
    menu = menuBar->addMenu("&Plik");
    menu->addSeparator();

    QAction * action;
    action = new QAction("Wczytaj pliki", this);
    connect(action, &QAction::triggered, this, &MainWindow::loadFiles);
    menu->addAction(action);

    //connect(ui->rootXPosSlider, SIGNAL(valueChanged(int)), ui->glSkeleton, SLOT(setRootXPosition(int)));

    connect(addFrameButton ,SIGNAL(released()), this, SLOT(buttonAddFramePressed()));
    connect(globalSlider, SIGNAL(valueChanged(int)), this, SLOT(updateGlobalListIterator(int)));

    scrollArea->setWidget(scrollWidget);

    QWidget * myWidget = new QWidget;
    myWidget->setStyleSheet("border: 1px solid red");
    vboxLayout->addWidget(myWidget);

    gridLayout->addWidget(addFrameButton,0,0,1,1);
    gridLayout->addWidget(globalSlider, 0,1,1,6);
    gridLayout->addLayout(vboxLayout, 0,7,10,3);
    gridLayout->addWidget(scrollArea,1,0,9,7);
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
    slider->setSingleStep(1);
    slider->setVisible(true);
}

void MainWindow::buttonAddFramePressed(){
    addFrameMenu(addFrameButtonCounter);
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


