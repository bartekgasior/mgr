/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <glwidget.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *label_pic1;
    QPushButton *button_min;
    QPushButton *button_reduce;
    QPushButton *button_increase;
    QPushButton *button_max;
    QLabel *label_imgCounter;
    GLWidget *glSkeleton;
    QSlider *rootXPosSlider;
    QLabel *rootXPosLabel;
    QPushButton *button_addFrame;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1305, 680);
        MainWindow->setAutoFillBackground(false);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label_pic1 = new QLabel(centralWidget);
        label_pic1->setObjectName(QStringLiteral("label_pic1"));
        label_pic1->setGeometry(QRect(30, 20, 461, 341));
        label_pic1->setFrameShape(QFrame::Box);
        button_min = new QPushButton(centralWidget);
        button_min->setObjectName(QStringLiteral("button_min"));
        button_min->setGeometry(QRect(60, 370, 31, 31));
        button_reduce = new QPushButton(centralWidget);
        button_reduce->setObjectName(QStringLiteral("button_reduce"));
        button_reduce->setGeometry(QRect(100, 370, 31, 31));
        button_increase = new QPushButton(centralWidget);
        button_increase->setObjectName(QStringLiteral("button_increase"));
        button_increase->setGeometry(QRect(390, 370, 31, 31));
        button_increase->setIconSize(QSize(64, 64));
        button_increase->setCheckable(false);
        button_max = new QPushButton(centralWidget);
        button_max->setObjectName(QStringLiteral("button_max"));
        button_max->setGeometry(QRect(430, 370, 31, 31));
        label_imgCounter = new QLabel(centralWidget);
        label_imgCounter->setObjectName(QStringLiteral("label_imgCounter"));
        label_imgCounter->setGeometry(QRect(170, 370, 181, 31));
        label_imgCounter->setFrameShape(QFrame::StyledPanel);
        glSkeleton = new GLWidget(centralWidget);
        glSkeleton->setObjectName(QStringLiteral("glSkeleton"));
        glSkeleton->setGeometry(QRect(510, 20, 541, 341));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(glSkeleton->sizePolicy().hasHeightForWidth());
        glSkeleton->setSizePolicy(sizePolicy);
        rootXPosSlider = new QSlider(centralWidget);
        rootXPosSlider->setObjectName(QStringLiteral("rootXPosSlider"));
        rootXPosSlider->setGeometry(QRect(1080, 40, 211, 21));
        rootXPosSlider->setMaximum(10);
        rootXPosSlider->setPageStep(1);
        rootXPosSlider->setValue(0);
        rootXPosSlider->setSliderPosition(0);
        rootXPosSlider->setOrientation(Qt::Horizontal);
        rootXPosLabel = new QLabel(centralWidget);
        rootXPosLabel->setObjectName(QStringLiteral("rootXPosLabel"));
        rootXPosLabel->setGeometry(QRect(1080, 20, 211, 16));
        button_addFrame = new QPushButton(centralWidget);
        button_addFrame->setObjectName(QStringLiteral("button_addFrame"));
        button_addFrame->setGeometry(QRect(970, 380, 80, 21));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1305, 20));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        label_pic1->setText(QString());
        button_min->setText(QString());
        button_reduce->setText(QString());
        button_increase->setText(QString());
        button_max->setText(QString());
        label_imgCounter->setText(QString());
        rootXPosLabel->setText(QApplication::translate("MainWindow", "model X position", nullptr));
        button_addFrame->setText(QApplication::translate("MainWindow", "Dodaj okno", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
