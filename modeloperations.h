#pragma once

#include <QObject>
#include "mainwindow.h"

class ModelOperations : public QObject
{
	Q_OBJECT

public:
	ModelOperations(QObject *parent);
	~ModelOperations();
	MainWindow mWindow = new MainWindow();
	QComboBox comboBoxFrameSelector =

	/*Operacje na modelu*/
	void rotateModelXUp();
	void rotateModelXDown();
	void rotateModelYLeft();
	void rotateModelYRight();
	void rotateLeftUpLegX1();
	void rotateLeftUpLegX2();
};
