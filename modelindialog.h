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

	void setGLWidget(GLWidget *glWidget);

	GLWidget *tmpGLWidget;

private slots:
	void okButtonAccepted();
	void okButtonRejected();
};
