#pragma once

#include <QObject>
#include <QDialog>
#include <QComboBox>
#include <QLabel>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QSpinBox>
#include <QGroupBox>

class ConfigDialog : public QDialog
{
	Q_OBJECT

public:
	ConfigDialog(QWidget *parent);
	~ConfigDialog();
	bool okClicked = false;
	bool drawJointsInGlWidgets = false;
	bool resizeBackgroundRes = false;

	void setJointRadiusSpinBoxValue(int val);
	int getJointsRadius();
	void setRadiusCheckBox(bool val);
	void disableJointSpinBox();

	void setTimerSpinBox(int val);
	int getTimerSpinBoxValue();

	void setResolutionSpinBoxes(int w, int h);
	int getWidthSpinBox();
	int getHeightSpinBox();
	void setResolutionCheckBox(bool val);
private:
	QVBoxLayout *mainLayout;
	
	QHBoxLayout *jointsRadiusLayout;
	QGroupBox *jointsRadiusGroupBox;
	QCheckBox *jointsRadiusCheckBox;
	QLabel *jointsRadiusLabel;
	QSpinBox *jointRadiusSpinBox;

	QHBoxLayout *timerLayout;
	QGroupBox *timerGroupBox;
	QLabel *timerLabel;
	QSpinBox *timerSpinBox;

	QHBoxLayout *backgroundResolutionLayout;
	QGroupBox *backgroundResolutionGroupBox;
	QCheckBox *backgroundResolutionCheckBox;
	QLabel *backgroundResolutionWidthLabel;
	QLabel *backgroundResolutionHeightLabel;
	QSpinBox *backgroundResolutionWidthSpinBox;
	QSpinBox *backgroundResolutionHeightSpinBox;

	QDialogButtonBox *okButton;

	void okButtonAccepted();
	void okButtonRejected();

	void addWidgetsToJointsLayout();
	void addWidgetsToTimerLayout();
	void addWidgetsToBackgroundResolutionLayout();
private slots:
	void drawJoints();
	void resizeBackgroundResolution();
};