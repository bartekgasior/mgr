#include "modeloperations.h"

ModelOperations::ModelOperations(QObject *parent)
	: QObject(parent)
{
}

ModelOperations::~ModelOperations()
{
}

void ModelOperations::rotateModelXUp() {
	//idx od 0
	ModelOperations[comboBoxFrameSelector->currentIndex()]->rotateModelXUp();
}

void ModelOperations::rotateModelXDown() {
	//idx od 0
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotateModelXDown();
}

void ModelOperations::rotateModelYLeft() {
	//idx od 0
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotateModelYLeft();
}

void ModelOperations::rotateModelYRight() {
	//idx od 0
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotateModelYRight();
}

void ModelOperations::rotateLeftUpLegX1() {
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotateLeftUpLegX1();
}

void ModelOperations::rotateLeftUpLegX2() {
	glWidgetsVector[comboBoxFrameSelector->currentIndex()]->rotateLeftUpLegX2();
}