#pragma once

#include <QDialog>
#include <QGridLayout>
#include <QDialogButtonBox>

#include "glwidget.h"

using namespace std;

class ModelInDialog : public QDialog
{
	Q_OBJECT

public:
	ModelInDialog(QWidget *parent);
	~ModelInDialog();

	QGridLayout *gridLayout;
	QDialogButtonBox *okButton;

	void setGLWidget(vector<vector<float>> modelState, pf::Model3D *model, map<string, int> idxBonesMap, vector<pf::boneGeometry> boneGeometry, vector<pf::boneConfig> boneConfig);

	GLWidget *tmpGLWidget;
	bool closed = false;


private slots:
	void okButtonAccepted();
	void okButtonRejected();
};
