#pragma once

#include <QDialog>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QLabel>
#include <QSpinBox>

using namespace std;

class CamerasConfig : public QDialog
{
	Q_OBJECT

public:
	CamerasConfig(QWidget *parent);
	~CamerasConfig();

private:
	/*glowny layout*/
	QHBoxLayout *mainLayout;

	/*layout zawierajacy parametry kamer*/
	QHBoxLayout *parametersLayout;
	/*kazda wiersz layoutu*/
	QVector<QVBoxLayout*> hLayouts;

	QVector<QLabel*> labels;
	QVector<QSpinBox*> spinBoxes;

	/*combobox do wyboru typu kamery*/
	QComboBox *cameraTypeComboBox;

	void addTsai1Layout();

private slots:
	void showCameraParameters();
	void okButtonAccepted();
	void okButtonRejected();
};
